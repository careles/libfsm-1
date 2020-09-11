#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fsm/fsm.h>
#include <fsm/options.h>

#include <re/re.h>

#include <adt/xalloc.h>

#include <unistd.h>

struct str {
	char *s;
	size_t len;
	size_t cap;
};

static const struct str init_str;

static void
str_append(struct str *str, int ch)
{
	if (str->len+1 >= str->cap) {
		size_t newcap;

		if (str->cap < 16) {
			newcap = 32;
		} else if (str->cap < 1024) {
			newcap = 2*str->cap;
		} else {
			newcap = str->cap + str->cap/2;
		}

		str->s = xrealloc(str->s, newcap);
		str->cap = newcap;
	}

	str->s[str->len++] = ch;
	str->s[str->len] = '\0';
}

static struct str *
readline(FILE *f, struct str *str)
{
	int ch;

	str->len = 0;

	while (ch = getc(f), ch != EOF) {
		str_append(str, ch);

		if (ch == '\n') {
			return str;
		}
	}

	return (str->len > 0) ? str : NULL;
}

static int
allws(const char *s)
{
	while (*s && isspace((unsigned char)*s)) {
		s++;
	}

	return *s == '\0';
}

static char *
lstrip(char *s)
{
	while (*s && isspace((unsigned char)*s)) {
		s++;
	}

	return s;
}

static char *
decode_escapes(char *s)
{
	char *b = s, *p = s;

	while (*s) {
		if (*s != '\\') {
			*p++ = *s++;
			continue;
		}

		s++;
		if (*s == '\0') {
			continue;
		}

		switch (s[0]) {
		case 'a': 
		case 'b':
		case 'e':
		case 'f':
		case 'n':
		case 'r':
		case 't':
		case 'x':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '\\':
			*p++ = '\\'; *p++ = s[0]; break;
		default:
			  *p++ = *s; break;
		}

		s++;
	}

	*p = '\0';
	return b;
}

enum state {
	ST_DEFAULT = 0,
	ST_MATCHES = 1,
	ST_NOTMATCHES = 2
};

static void
usage(const char *progname)
{
	fprintf(stderr, "%s -h\n", progname);
	fprintf(stderr, "%s [infile] [outfile]\n", progname);
}

static const struct fsm_options zero_options;

int main(int argc, char **argv)
{
	struct str l;
	enum state state;
	size_t count, nparsed, linenum;
	int re_ok = 0;
	struct fsm_options opt;

	FILE *in, *out;

	(void)argc;
	(void)argv;

	l = init_str;
	state = ST_DEFAULT;

	opt = zero_options;
	opt.comments = 0;
	opt.anonymous_states  = 1;
	opt.consolidate_edges = 1;
	opt.io = FSM_IO_GETC;

	count   = 0;
	nparsed = 0;
	linenum = 0;

	in  = stdin;
	out = stdout;

	{
		const char *progname = argv[0];

		int o;
		while (o = getopt(argc,argv, "h"), o != -1) {
			switch (o) {
			case 'h':
				usage(progname);
				exit(0);
				break;

			default:
				fprintf(stderr, "%s: unknown option '%c'\n", progname, optopt);
				usage(progname);
				exit(1);
				break;
			}
		}

		argc -= optind;
		argv += optind;

		if (argc > 2) {
			fprintf(stderr, "%s: too many arguments\n", progname);
			usage(progname);
			exit(1);
		}

		if (argc > 0) {
			in = fopen(argv[0], "r");
			if (in == NULL) {
				fprintf(stderr, "%s: error opening '%s' for input: %s\n",
					progname, argv[0], strerror(errno));
				exit(1);
			}
		}

		if (argc > 1) {
			out = fopen(argv[1], "w");
			if (out == NULL) {
				fprintf(stderr, "%s: error opening '%s' for output: %s\n",
					progname, argv[1], strerror(errno));
				exit(1);
			}
		}
	}

	while (readline(in, &l) != NULL) {
		char *s = l.s;
		size_t n = l.len;
		int reset = 0;

		linenum++;

		if (n > 0 && s[n-1] == '\n') {
			s[n-1] = '\0';
			n--;
		}

		switch (state) {
		case ST_DEFAULT:
			if (n > 1 && s[0] == '/' && s[n-1] == '/') {
				char *regexp;

				s[n-1] = '\0';
				s++;

				regexp = xstrdup(s);
				if (regexp == NULL) {
					fprintf(stderr, "line %5zu: could not allocate %zu bytes for regexp /%s/: %s\n",
						linenum, strlen(s), s, strerror(errno));
				} else {
					static const struct re_err err_zero;

					enum re_flags flags;
					struct re_err comp_err;
					struct fsm *fsm;

					comp_err = err_zero;
					flags = 0;

					fsm = re_comp(RE_PCRE, fsm_sgetc, &s, &opt, flags, &comp_err);
					re_ok = (fsm != NULL);
					if (re_ok) {
						fsm_free(fsm);
						nparsed++;
						if (count > 0) {
							fprintf(out,"\n");
						}

						fprintf(out,"%s\n", regexp);
					} else {
						fprintf(stderr, "line %5zu: could not parse regexp /%s/: %s\n",
								linenum, s, re_strerror(comp_err.e));
					}

					free(regexp);
				}

				state = ST_MATCHES;
				count++;
			}
			break;

		case ST_MATCHES:
			if (n > 1 && s[0] == '\\' && s[1] == '=') {
				state = ST_NOTMATCHES;
			} else if (n == 0 || allws(s)) {
				reset = 1;
			} else if (n > 0 && s[0] == '/') {
				fprintf(stderr, "state machine failure at line %zu: regexp before matches/notmatches ends\n", linenum);
				reset = 1;
			} else {
				if (re_ok) {
					fprintf(out, "+%s\n", decode_escapes(lstrip(s)));
				}
			}
			break;

		case ST_NOTMATCHES:
			if (n == 0 || allws(s)) {
				reset = 1;
			} else if (s[0] == '/') {
				fprintf(stderr, "state machine failure at line %zu: regexp before matches/notmatches ends\n", linenum);
				reset = 1;
			} else {
				if (re_ok) {
					fprintf(out, "-%s\n", decode_escapes(lstrip(s)));
				}
			}
			break;
		}

		if (reset) {
			state = ST_DEFAULT;
			re_ok = 0;
		}

		if ((linenum % 500) == 0) {
			fprintf(stderr, "line %5zu: %zu entries, %zu parsed correctly\n", linenum, count, nparsed);
		}
	}

	if (ferror(in)) {
		perror("reading input");
		exit(EXIT_FAILURE);
	}

	if (ferror(out)) {
		perror("writing output");
		exit(EXIT_FAILURE);
	}

	fprintf(stderr, "done.  %zu lines, %zu entries, %zu parsed correctly\n", linenum, count, nparsed);

	return EXIT_SUCCESS;
}

