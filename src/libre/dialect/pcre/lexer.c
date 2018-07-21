/* Generated by lx */

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>

#include LX_HEADER

static enum lx_pcre_token z0(struct lx_pcre_lx *lx);
static enum lx_pcre_token z1(struct lx_pcre_lx *lx);
static enum lx_pcre_token z2(struct lx_pcre_lx *lx);
static enum lx_pcre_token z3(struct lx_pcre_lx *lx);
static enum lx_pcre_token z4(struct lx_pcre_lx *lx);
static enum lx_pcre_token z5(struct lx_pcre_lx *lx);

#if __STDC_VERSION__ >= 199901L
inline
#endif
static int
lx_getc(struct lx_pcre_lx *lx)
{
	int c;

	assert(lx != NULL);
	assert(lx->lgetc != NULL);

	if (lx->c != EOF) {
		c = lx->c, lx->c = EOF;
	} else {
		c = lx->lgetc(lx);
		if (c == EOF) {
			return EOF;
		}
	}

	lx->end.byte++;
	lx->end.col++;

	if (c == '\n') {
		lx->end.line++;
		lx->end.col = 1;
	}

	return c;
}

#if __STDC_VERSION__ >= 199901L
inline
#endif
static void
lx_pcre_ungetc(struct lx_pcre_lx *lx, int c)
{
	assert(lx != NULL);
	assert(lx->c == EOF);

	lx->c = c;


	lx->end.byte--;
	lx->end.col--;

	if (c == '\n') {
		lx->end.line--;
		lx->end.col = 0; /* XXX: lost information */
	}
}

int
lx_pcre_dynpush(void *buf_opaque, char c)
{
	struct lx_dynbuf *t = buf_opaque;

	assert(t != NULL);

	if (t->p == t->a + t->len) {
		size_t len;
		ptrdiff_t off;
		char *tmp;

		if (t->len == 0) {
			assert(LX_DYN_LOW > 0);
			len = LX_DYN_LOW;
		} else {
			len = t->len * LX_DYN_FACTOR;
			if (len < t->len) {
				errno = ERANGE;
				return -1;
			}
		}

		off = t->p - t->a;
		tmp = realloc(t->a, len);
		if (tmp == NULL) {
			return -1;
		}

		t->p   = tmp + off;
		t->a   = tmp;
		t->len = len;
	}

	assert(t->p != NULL);
	assert(t->a != NULL);

	*t->p++ = c;

	return 0;
}

int
lx_pcre_dynclear(void *buf_opaque)
{
	struct lx_dynbuf *t = buf_opaque;

	assert(t != NULL);

	if (t->len > LX_DYN_HIGH) {
		size_t len;
		char *tmp;

		len = t->len / LX_DYN_FACTOR;

		tmp = realloc(t->a, len);
		if (tmp == NULL) {
			return -1;
		}

		t->a   = tmp;
		t->len = len;
	}

	t->p = t->a;

	return 0;
}

void
lx_pcre_dynfree(void *buf_opaque)
{
	struct lx_dynbuf *t = buf_opaque;

	assert(t != NULL);

	free(t->a);
}
static enum lx_pcre_token
z0(struct lx_pcre_lx *lx)
{
	int c;

	enum {
		S0, S1, S2, S3, NONE
	} state;

	assert(lx != NULL);

	if (lx->clear != NULL) {
		lx->clear(lx->buf_opaque);
	}

	state = NONE;

	lx->start = lx->end;

	while (c = lx_getc(lx), c != EOF) {
		if (state == NONE) {
			state = S0;
		}

		switch (state) {
		case S0: /* start */
			switch ((unsigned char) c) {
			case '\\': state = S2; break;
			default: state = S1; break;
			}
			break;

		case S1: /* e.g. "a" */
			lx_pcre_ungetc(lx, c); return TOK_CHAR;

		case S2: /* e.g. "\\" */
			switch ((unsigned char) c) {
			case 'E': state = S3; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_CHAR;
			}
			break;

		case S3: /* e.g. "\\E" */
			lx_pcre_ungetc(lx, c); return lx->z = z5, lx->z(lx);

		default:
			; /* unreached */
		}

		switch (state) {
		case S3:
			break;

		default:
			if (lx->push != NULL) {
				if (-1 == lx->push(lx->buf_opaque, c)) {
					return TOK_ERROR;
				}
			}
			break;

		}
	}

	lx->lgetc = NULL;

	switch (state) {
	case NONE: return TOK_EOF;
	case S1: return TOK_CHAR;
	case S2: return TOK_CHAR;
	case S3: return TOK_EOF;
	default: errno = EINVAL; return TOK_ERROR;
	}
}

static enum lx_pcre_token
z1(struct lx_pcre_lx *lx)
{
	int c;

	enum {
		S0, S1, S2, S3, NONE
	} state;

	assert(lx != NULL);

	if (lx->clear != NULL) {
		lx->clear(lx->buf_opaque);
	}

	state = NONE;

	lx->start = lx->end;

	while (c = lx_getc(lx), c != EOF) {
		if (state == NONE) {
			state = S0;
		}

		switch (state) {
		case S0: /* start */
			switch ((unsigned char) c) {
			case '}': state = S3; break;
			case ',': state = S1; break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': state = S2; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S1: /* e.g. "," */
			lx_pcre_ungetc(lx, c); return TOK_SEP;

		case S2: /* e.g. "0" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': break;
			default:  lx_pcre_ungetc(lx, c); return TOK_COUNT;
			}
			break;

		case S3: /* e.g. "}" */
			lx_pcre_ungetc(lx, c); return lx->z = z5, TOK_CLOSECOUNT;

		default:
			; /* unreached */
		}

		if (lx->push != NULL) {
			if (-1 == lx->push(lx->buf_opaque, c)) {
				return TOK_ERROR;
			}
		}
	}

	lx->lgetc = NULL;

	switch (state) {
	case NONE: return TOK_EOF;
	case S1: return TOK_SEP;
	case S2: return TOK_COUNT;
	case S3: return TOK_CLOSECOUNT;
	default: errno = EINVAL; return TOK_ERROR;
	}
}

static enum lx_pcre_token
z2(struct lx_pcre_lx *lx)
{
	int c;

	enum {
		S0, S1, S2, S3, NONE
	} state;

	assert(lx != NULL);

	if (lx->clear != NULL) {
		lx->clear(lx->buf_opaque);
	}

	state = NONE;

	lx->start = lx->end;

	while (c = lx_getc(lx), c != EOF) {
		if (state == NONE) {
			state = S0;
		}

		switch (state) {
		case S0: /* start */
			switch ((unsigned char) c) {
			case '\\': state = S2; break;
			default: state = S1; break;
			}
			break;

		case S1: /* e.g. "a" */
			lx_pcre_ungetc(lx, c); return TOK_CHAR;

		case S2: /* e.g. "\\" */
			switch ((unsigned char) c) {
			case 'E': state = S3; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_CHAR;
			}
			break;

		case S3: /* e.g. "\\E" */
			lx_pcre_ungetc(lx, c); return lx->z = z3, lx->z(lx);

		default:
			; /* unreached */
		}

		switch (state) {
		case S3:
			break;

		default:
			if (lx->push != NULL) {
				if (-1 == lx->push(lx->buf_opaque, c)) {
					return TOK_ERROR;
				}
			}
			break;

		}
	}

	lx->lgetc = NULL;

	switch (state) {
	case NONE: return TOK_EOF;
	case S1: return TOK_CHAR;
	case S2: return TOK_CHAR;
	case S3: return TOK_EOF;
	default: errno = EINVAL; return TOK_ERROR;
	}
}

static enum lx_pcre_token
z3(struct lx_pcre_lx *lx)
{
	int c;

	enum {
		S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, 
		S10, S11, S12, S13, S14, S15, S16, S17, S18, S19, 
		S20, S21, S22, S23, S24, S25, S26, S27, S28, S29, 
		S30, S31, S32, S33, S34, S35, S36, S37, S38, S39, 
		S40, S41, S42, S43, S44, S45, S46, S47, S48, S49, 
		S50, S51, S52, S53, S54, S55, S56, S57, S58, S59, 
		S60, S61, S62, S63, S64, S65, S66, S67, S68, S69, 
		S70, NONE
	} state;

	assert(lx != NULL);

	if (lx->clear != NULL) {
		lx->clear(lx->buf_opaque);
	}

	state = NONE;

	lx->start = lx->end;

	while (c = lx_getc(lx), c != EOF) {
		if (state == NONE) {
			state = S0;
		}

		switch (state) {
		case S0: /* start */
			switch ((unsigned char) c) {
			case '\\': state = S4; break;
			case '[': state = S3; break;
			case ']': state = S5; break;
			case '^': state = S6; break;
			case '-': state = S2; break;
			default: state = S1; break;
			}
			break;

		case S1: /* e.g. "a" */
			lx_pcre_ungetc(lx, c); return TOK_CHAR;

		case S2: /* e.g. "-" */
			lx_pcre_ungetc(lx, c); return TOK_RANGE;

		case S3: /* e.g. "[" */
			switch ((unsigned char) c) {
			case ':': state = S7; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_CHAR;
			}
			break;

		case S4: /* e.g. "\\" */
			switch ((unsigned char) c) {
			case 'D':
			case 'H':
			case 'N':
			case 'S':
			case 'V':
			case 'W':
			case 'd':
			case 'h':
			case 's':
			case 'v':
			case 'w': state = S53; break;
			case '$':
			case '(':
			case '*':
			case '+':
			case '-':
			case '.':
			case '?':
			case '[':
			case '\\':
			case '^':
			case 'a':
			case 'f':
			case 'n':
			case 'r':
			case 't':
			case '{':
			case '|': state = S56; break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7': state = S57; break;
			case 'Q': state = S58; break;
			case 'c': state = S59; break;
			case 'o': state = S60; break;
			case 'x': state = S61; break;
			default: state = S55; break;
			}
			break;

		case S5: /* e.g. "]" */
			lx_pcre_ungetc(lx, c); return lx->z = z5, TOK_CLOSEGROUP;

		case S6: /* e.g. "^" */
			lx_pcre_ungetc(lx, c); return TOK_INVERT;

		case S7: /* e.g. "[:" */
			switch ((unsigned char) c) {
			case 'a': state = S8; break;
			case 'b': state = S9; break;
			case 'c': state = S10; break;
			case 'd': state = S11; break;
			case 'g': state = S12; break;
			case 'l': state = S13; break;
			case 'p': state = S14; break;
			case 's': state = S15; break;
			case 'u': state = S16; break;
			case 'w': state = S17; break;
			case 'x': state = S18; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S8: /* e.g. "[:a" */
			switch ((unsigned char) c) {
			case 'l': state = S38; break;
			case 's': state = S39; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S9: /* e.g. "[:b" */
			switch ((unsigned char) c) {
			case 'l': state = S33; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S10: /* e.g. "[:c" */
			switch ((unsigned char) c) {
			case 'n': state = S50; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S11: /* e.g. "[:d" */
			switch ((unsigned char) c) {
			case 'i': state = S44; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S12: /* e.g. "[:g" */
			switch ((unsigned char) c) {
			case 'r': state = S19; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S13: /* e.g. "[:l" */
			switch ((unsigned char) c) {
			case 'o': state = S32; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S14: /* e.g. "[:p" */
			switch ((unsigned char) c) {
			case 'r': state = S21; break;
			case 'u': state = S22; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S15: /* e.g. "[:s" */
			switch ((unsigned char) c) {
			case 'p': state = S25; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S16: /* e.g. "[:u" */
			switch ((unsigned char) c) {
			case 'p': state = S27; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S17: /* e.g. "[:w" */
			switch ((unsigned char) c) {
			case 'o': state = S29; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S18: /* e.g. "[:x" */
			switch ((unsigned char) c) {
			case 'd': state = S11; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S19: /* e.g. "[:gr" */
			switch ((unsigned char) c) {
			case 'a': state = S20; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S20: /* e.g. "[:gra" */
			switch ((unsigned char) c) {
			case 'p': state = S54; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S21: /* e.g. "[:pr" */
			switch ((unsigned char) c) {
			case 'i': state = S24; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S22: /* e.g. "[:pu" */
			switch ((unsigned char) c) {
			case 'n': state = S23; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S23: /* e.g. "[:pun" */
			switch ((unsigned char) c) {
			case 'c': state = S46; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S24: /* e.g. "[:pri" */
			switch ((unsigned char) c) {
			case 'n': state = S46; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S25: /* e.g. "[:sp" */
			switch ((unsigned char) c) {
			case 'a': state = S26; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S26: /* e.g. "[:spa" */
			switch ((unsigned char) c) {
			case 'c': state = S31; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S27: /* e.g. "[:up" */
			switch ((unsigned char) c) {
			case 'p': state = S28; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S28: /* e.g. "[:low" */
			switch ((unsigned char) c) {
			case 'e': state = S43; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S29: /* e.g. "[:wo" */
			switch ((unsigned char) c) {
			case 'r': state = S30; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S30: /* e.g. "[:wor" */
			switch ((unsigned char) c) {
			case 'd': state = S36; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S31: /* e.g. "[:spac" */
			switch ((unsigned char) c) {
			case 'e': state = S36; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S32: /* e.g. "[:lo" */
			switch ((unsigned char) c) {
			case 'w': state = S28; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S33: /* e.g. "[:bl" */
			switch ((unsigned char) c) {
			case 'a': state = S34; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S34: /* e.g. "[:bla" */
			switch ((unsigned char) c) {
			case 'n': state = S35; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S35: /* e.g. "[:blan" */
			switch ((unsigned char) c) {
			case 'k': state = S36; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S36: /* e.g. "[:word" */
			switch ((unsigned char) c) {
			case ':': state = S37; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S37: /* e.g. "[:word:" */
			switch ((unsigned char) c) {
			case ']': state = S53; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S38: /* e.g. "[:al" */
			switch ((unsigned char) c) {
			case 'n': state = S40; break;
			case 'p': state = S41; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S39: /* e.g. "[:as" */
			switch ((unsigned char) c) {
			case 'c': state = S47; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S40: /* e.g. "[:aln" */
			switch ((unsigned char) c) {
			case 'u': state = S42; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S41: /* e.g. "[:alp" */
			switch ((unsigned char) c) {
			case 'h': state = S48; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S42: /* e.g. "[:alnu" */
			switch ((unsigned char) c) {
			case 'm': state = S36; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S43: /* e.g. "[:lowe" */
			switch ((unsigned char) c) {
			case 'r': state = S36; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S44: /* e.g. "[:di" */
			switch ((unsigned char) c) {
			case 'g': state = S45; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S45: /* e.g. "[:dig" */
			switch ((unsigned char) c) {
			case 'i': state = S46; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S46: /* e.g. "[:prin" */
			switch ((unsigned char) c) {
			case 't': state = S36; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S47: /* e.g. "[:asc" */
			switch ((unsigned char) c) {
			case 'i': state = S49; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S48: /* e.g. "[:alph" */
			switch ((unsigned char) c) {
			case 'a': state = S36; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S49: /* e.g. "[:asci" */
			switch ((unsigned char) c) {
			case 'i': state = S36; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S50: /* e.g. "[:cn" */
			switch ((unsigned char) c) {
			case 't': state = S51; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S51: /* e.g. "[:cnt" */
			switch ((unsigned char) c) {
			case 'r': state = S52; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S52: /* e.g. "[:cntr" */
			switch ((unsigned char) c) {
			case 'l': state = S36; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S53: /* e.g. "\\d" */
			lx_pcre_ungetc(lx, c); return TOK_NAMED__CHAR__CLASS;

		case S54: /* e.g. "[:grap" */
			switch ((unsigned char) c) {
			case 'h': state = S36; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S55: /* e.g. "\\b" */
			lx_pcre_ungetc(lx, c); return TOK_NOESC;

		case S56: /* e.g. "\\a" */
			lx_pcre_ungetc(lx, c); return TOK_ESC;

		case S57: /* e.g. "\\0" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7': state = S62; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_OCT;
			}
			break;

		case S58: /* e.g. "\\Q" */
			lx_pcre_ungetc(lx, c); return lx->z = z2, lx->z(lx);

		case S59: /* e.g. "\\c" */
			state = S70; break;

		case S60: /* e.g. "\\o" */
			switch ((unsigned char) c) {
			case '{': state = S68; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_NOESC;
			}
			break;

		case S61: /* e.g. "\\x" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f': state = S64; break;
			case '{': state = S65; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_NOESC;
			}
			break;

		case S62: /* e.g. "\\00" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7': state = S63; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_OCT;
			}
			break;

		case S63: /* e.g. "\\000" */
			lx_pcre_ungetc(lx, c); return TOK_OCT;

		case S64: /* e.g. "\\xa" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f': state = S66; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_HEX;
			}
			break;

		case S65: /* e.g. "\\x{" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f': state = S67; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S66: /* e.g. "\\xaa" */
			lx_pcre_ungetc(lx, c); return TOK_HEX;

		case S67: /* e.g. "\\x{a" */
			switch ((unsigned char) c) {
			case '}': state = S66; break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f': break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S68: /* e.g. "\\o{" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7': state = S69; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S69: /* e.g. "\\o{0" */
			switch ((unsigned char) c) {
			case '}': state = S63; break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7': break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S70: /* e.g. "\\ca" */
			lx_pcre_ungetc(lx, c); return TOK_CONTROL;

		default:
			; /* unreached */
		}

		switch (state) {
		case S58:
			break;

		default:
			if (lx->push != NULL) {
				if (-1 == lx->push(lx->buf_opaque, c)) {
					return TOK_ERROR;
				}
			}
			break;

		}
	}

	lx->lgetc = NULL;

	switch (state) {
	case NONE: return TOK_EOF;
	case S1: return TOK_CHAR;
	case S2: return TOK_RANGE;
	case S3: return TOK_CHAR;
	case S4: return TOK_INVALID;
	case S5: return TOK_CLOSEGROUP;
	case S6: return TOK_INVERT;
	case S53: return TOK_NAMED__CHAR__CLASS;
	case S55: return TOK_NOESC;
	case S56: return TOK_ESC;
	case S57: return TOK_OCT;
	case S58: return TOK_EOF;
	case S59: return TOK_NOESC;
	case S60: return TOK_NOESC;
	case S61: return TOK_NOESC;
	case S62: return TOK_OCT;
	case S63: return TOK_OCT;
	case S64: return TOK_HEX;
	case S66: return TOK_HEX;
	case S70: return TOK_CONTROL;
	default: errno = EINVAL; return TOK_ERROR;
	}
}

static enum lx_pcre_token
z4(struct lx_pcre_lx *lx)
{
	int c;

	enum {
		S0, S1, S2, S3, S4, NONE
	} state;

	assert(lx != NULL);

	if (lx->clear != NULL) {
		lx->clear(lx->buf_opaque);
	}

	state = NONE;

	lx->start = lx->end;

	while (c = lx_getc(lx), c != EOF) {
		if (state == NONE) {
			state = S0;
		}

		switch (state) {
		case S0: /* start */
			switch ((unsigned char) c) {
			case '-': state = S2; break;
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z': state = S3; break;
			case 'i': state = S4; break;
			case ')': state = S1; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S1: /* e.g. ")" */
			lx_pcre_ungetc(lx, c); return lx->z = z5, TOK_CLOSEFLAGS;

		case S2: /* e.g. "-" */
			lx_pcre_ungetc(lx, c); return TOK_NEGATE;

		case S3: /* e.g. "a" */
			lx_pcre_ungetc(lx, c); return TOK_FLAG__UNKNOWN;

		case S4: /* e.g. "i" */
			lx_pcre_ungetc(lx, c); return TOK_FLAG__INSENSITIVE;

		default:
			; /* unreached */
		}

		if (lx->push != NULL) {
			if (-1 == lx->push(lx->buf_opaque, c)) {
				return TOK_ERROR;
			}
		}
	}

	lx->lgetc = NULL;

	switch (state) {
	case NONE: return TOK_EOF;
	case S1: return TOK_CLOSEFLAGS;
	case S2: return TOK_NEGATE;
	case S3: return TOK_FLAG__UNKNOWN;
	case S4: return TOK_FLAG__INSENSITIVE;
	default: errno = EINVAL; return TOK_ERROR;
	}
}

static enum lx_pcre_token
z5(struct lx_pcre_lx *lx)
{
	int c;

	enum {
		S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, 
		S10, S11, S12, S13, S14, S15, S16, S17, S18, S19, 
		S20, S21, S22, S23, S24, S25, S26, S27, S28, S29, 
		S30, S31, S32, NONE
	} state;

	assert(lx != NULL);

	if (lx->clear != NULL) {
		lx->clear(lx->buf_opaque);
	}

	state = NONE;

	lx->start = lx->end;

	while (c = lx_getc(lx), c != EOF) {
		if (state == NONE) {
			state = S0;
		}

		switch (state) {
		case S0: /* start */
			switch ((unsigned char) c) {
			case '(': state = S3; break;
			case ')': state = S4; break;
			case '*': state = S5; break;
			case '+': state = S6; break;
			case '.': state = S7; break;
			case '$': state = S2; break;
			case '?': state = S8; break;
			case '[': state = S9; break;
			case '\\': state = S10; break;
			case '^': state = S11; break;
			case '{': state = S12; break;
			case '|': state = S13; break;
			default: state = S1; break;
			}
			break;

		case S1: /* e.g. "a" */
			lx_pcre_ungetc(lx, c); return TOK_CHAR;

		case S2: /* e.g. "$" */
			lx_pcre_ungetc(lx, c); return TOK_END;

		case S3: /* e.g. "(" */
			switch ((unsigned char) c) {
			case '?': state = S14; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_OPENCAPTURE;
			}
			break;

		case S4: /* e.g. ")" */
			lx_pcre_ungetc(lx, c); return TOK_CLOSE;

		case S5: /* e.g. "*" */
			lx_pcre_ungetc(lx, c); return TOK_STAR;

		case S6: /* e.g. "+" */
			lx_pcre_ungetc(lx, c); return TOK_PLUS;

		case S7: /* e.g. "." */
			lx_pcre_ungetc(lx, c); return TOK_ANY;

		case S8: /* e.g. "?" */
			lx_pcre_ungetc(lx, c); return TOK_OPT;

		case S9: /* e.g. "[" */
			lx_pcre_ungetc(lx, c); return lx->z = z3, TOK_OPENGROUP;

		case S10: /* e.g. "\\" */
			switch ((unsigned char) c) {
			case '$':
			case '(':
			case ')':
			case '*':
			case '+':
			case '.':
			case '?':
			case '[':
			case '\\':
			case '^':
			case 'a':
			case 'f':
			case 'n':
			case 'r':
			case 't':
			case '{':
			case '|': state = S17; break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7': state = S18; break;
			case 'D':
			case 'H':
			case 'N':
			case 'S':
			case 'V':
			case 'W':
			case 'd':
			case 'h':
			case 's':
			case 'v':
			case 'w': state = S19; break;
			case 'Q': state = S20; break;
			case 'c': state = S21; break;
			case 'o': state = S22; break;
			case 'x': state = S23; break;
			default: state = S16; break;
			}
			break;

		case S11: /* e.g. "^" */
			lx_pcre_ungetc(lx, c); return TOK_START;

		case S12: /* e.g. "{" */
			lx_pcre_ungetc(lx, c); return lx->z = z1, TOK_OPENCOUNT;

		case S13: /* e.g. "|" */
			lx_pcre_ungetc(lx, c); return TOK_ALT;

		case S14: /* e.g. "(?" */
			switch ((unsigned char) c) {
			case ':': state = S15; break;
			default:  lx_pcre_ungetc(lx, c); return lx->z = z4, TOK_OPENFLAGS;
			}
			break;

		case S15: /* e.g. "(?:" */
			lx_pcre_ungetc(lx, c); return TOK_OPENSUB;

		case S16: /* e.g. "\\b" */
			lx_pcre_ungetc(lx, c); return TOK_NOESC;

		case S17: /* e.g. "\\a" */
			lx_pcre_ungetc(lx, c); return TOK_ESC;

		case S18: /* e.g. "\\0" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7': state = S24; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_OCT;
			}
			break;

		case S19: /* e.g. "\\d" */
			lx_pcre_ungetc(lx, c); return TOK_NAMED__CHAR__CLASS;

		case S20: /* e.g. "\\Q" */
			lx_pcre_ungetc(lx, c); return lx->z = z0, lx->z(lx);

		case S21: /* e.g. "\\c" */
			state = S26; break;

		case S22: /* e.g. "\\o" */
			switch ((unsigned char) c) {
			case '{': state = S27; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_NOESC;
			}
			break;

		case S23: /* e.g. "\\x" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f': state = S29; break;
			case '{': state = S30; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_NOESC;
			}
			break;

		case S24: /* e.g. "\\00" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7': state = S25; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_OCT;
			}
			break;

		case S25: /* e.g. "\\000" */
			lx_pcre_ungetc(lx, c); return TOK_OCT;

		case S26: /* e.g. "\\ca" */
			lx_pcre_ungetc(lx, c); return TOK_CONTROL;

		case S27: /* e.g. "\\o{" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7': state = S28; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S28: /* e.g. "\\o{0" */
			switch ((unsigned char) c) {
			case '}': state = S25; break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7': break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S29: /* e.g. "\\xa" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f': state = S32; break;
			default:  lx_pcre_ungetc(lx, c); return TOK_HEX;
			}
			break;

		case S30: /* e.g. "\\x{" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f': state = S31; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S31: /* e.g. "\\x{a" */
			switch ((unsigned char) c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f': break;
			case '}': state = S32; break;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
			break;

		case S32: /* e.g. "\\xaa" */
			lx_pcre_ungetc(lx, c); return TOK_HEX;

		default:
			; /* unreached */
		}

		switch (state) {
		case S20:
			break;

		default:
			if (lx->push != NULL) {
				if (-1 == lx->push(lx->buf_opaque, c)) {
					return TOK_ERROR;
				}
			}
			break;

		}
	}

	lx->lgetc = NULL;

	switch (state) {
	case NONE: return TOK_EOF;
	case S1: return TOK_CHAR;
	case S2: return TOK_END;
	case S3: return TOK_OPENCAPTURE;
	case S4: return TOK_CLOSE;
	case S5: return TOK_STAR;
	case S6: return TOK_PLUS;
	case S7: return TOK_ANY;
	case S8: return TOK_OPT;
	case S9: return TOK_OPENGROUP;
	case S10: return TOK_INVALID;
	case S11: return TOK_START;
	case S12: return TOK_OPENCOUNT;
	case S13: return TOK_ALT;
	case S14: return TOK_OPENFLAGS;
	case S15: return TOK_OPENSUB;
	case S16: return TOK_NOESC;
	case S17: return TOK_ESC;
	case S18: return TOK_OCT;
	case S19: return TOK_NAMED__CHAR__CLASS;
	case S20: return TOK_EOF;
	case S21: return TOK_NOESC;
	case S22: return TOK_NOESC;
	case S23: return TOK_NOESC;
	case S24: return TOK_OCT;
	case S25: return TOK_OCT;
	case S26: return TOK_CONTROL;
	case S29: return TOK_HEX;
	case S32: return TOK_HEX;
	default: errno = EINVAL; return TOK_ERROR;
	}
}

const char *
lx_pcre_name(enum lx_pcre_token t)
{
	switch (t) {
	case TOK_COUNT: return "COUNT";
	case TOK_SEP: return "SEP";
	case TOK_CLOSECOUNT: return "CLOSECOUNT";
	case TOK_OPENCOUNT: return "OPENCOUNT";
	case TOK_CHAR: return "CHAR";
	case TOK_RANGE: return "RANGE";
	case TOK_INVERT: return "INVERT";
	case TOK_CLOSEGROUP: return "CLOSEGROUP";
	case TOK_OPENGROUP: return "OPENGROUP";
	case TOK_FLAG__UNKNOWN: return "FLAG__UNKNOWN";
	case TOK_FLAG__INSENSITIVE: return "FLAG__INSENSITIVE";
	case TOK_NEGATE: return "NEGATE";
	case TOK_CLOSEFLAGS: return "CLOSEFLAGS";
	case TOK_OPENFLAGS: return "OPENFLAGS";
	case TOK_INVALID: return "INVALID";
	case TOK_NOESC: return "NOESC";
	case TOK_HEX: return "HEX";
	case TOK_OCT: return "OCT";
	case TOK_NAMED__CHAR__CLASS: return "NAMED__CHAR__CLASS";
	case TOK_CONTROL: return "CONTROL";
	case TOK_ESC: return "ESC";
	case TOK_ALT: return "ALT";
	case TOK_ANY: return "ANY";
	case TOK_PLUS: return "PLUS";
	case TOK_STAR: return "STAR";
	case TOK_OPT: return "OPT";
	case TOK_END: return "END";
	case TOK_START: return "START";
	case TOK_CLOSE: return "CLOSE";
	case TOK_OPENCAPTURE: return "OPENCAPTURE";
	case TOK_OPENSUB: return "OPENSUB";
	case TOK_EOF:     return "EOF";
	case TOK_ERROR:   return "ERROR";
	case TOK_UNKNOWN: return "UNKNOWN";
	default: return "?";
	}
}

const char *
lx_pcre_example(enum lx_pcre_token (*z)(struct lx_pcre_lx *), enum lx_pcre_token t)
{
	assert(z != NULL);

	if (z == z0) {
		switch (t) {
		case TOK_CHAR: return "a";
		default: goto error;
		}
	} else
	if (z == z1) {
		switch (t) {
		case TOK_COUNT: return "0";
		case TOK_SEP: return ",";
		case TOK_CLOSECOUNT: return "}";
		default: goto error;
		}
	} else
	if (z == z2) {
		switch (t) {
		case TOK_CHAR: return "a";
		default: goto error;
		}
	} else
	if (z == z3) {
		switch (t) {
		case TOK_CHAR: return "a";
		case TOK_RANGE: return "-";
		case TOK_INVERT: return "^";
		case TOK_CLOSEGROUP: return "]";
		case TOK_INVALID: return "\\";
		case TOK_NOESC: return "\\b";
		case TOK_HEX: return "\\xa";
		case TOK_OCT: return "\\0";
		case TOK_NAMED__CHAR__CLASS: return "\\d";
		case TOK_CONTROL: return "\\ca";
		case TOK_ESC: return "\\a";
		default: goto error;
		}
	} else
	if (z == z4) {
		switch (t) {
		case TOK_FLAG__UNKNOWN: return "a";
		case TOK_FLAG__INSENSITIVE: return "i";
		case TOK_NEGATE: return "-";
		case TOK_CLOSEFLAGS: return ")";
		default: goto error;
		}
	} else
	if (z == z5) {
		switch (t) {
		case TOK_OPENCOUNT: return "{";
		case TOK_CHAR: return "a";
		case TOK_OPENGROUP: return "[";
		case TOK_OPENFLAGS: return "(?";
		case TOK_INVALID: return "\\";
		case TOK_NOESC: return "\\b";
		case TOK_HEX: return "\\xa";
		case TOK_OCT: return "\\0";
		case TOK_NAMED__CHAR__CLASS: return "\\d";
		case TOK_CONTROL: return "\\ca";
		case TOK_ESC: return "\\a";
		case TOK_ALT: return "|";
		case TOK_ANY: return ".";
		case TOK_PLUS: return "+";
		case TOK_STAR: return "*";
		case TOK_OPT: return "?";
		case TOK_END: return "$";
		case TOK_START: return "^";
		case TOK_CLOSE: return ")";
		case TOK_OPENCAPTURE: return "(";
		case TOK_OPENSUB: return "(?:";
		default: goto error;
		}
	}

error:

	errno = EINVAL;
	return NULL;
}

void
lx_pcre_init(struct lx_pcre_lx *lx)
{
	static const struct lx_pcre_lx lx_default;

	assert(lx != NULL);

	*lx = lx_default;

	lx->c = EOF;
	lx->z = z5;

	lx->end.byte = 0;
	lx->end.line = 1;
	lx->end.col  = 1;
}

enum lx_pcre_token
lx_pcre_next(struct lx_pcre_lx *lx)
{
	enum lx_pcre_token t;

	assert(lx != NULL);
	assert(lx->z != NULL);

	if (lx->lgetc == NULL) {
		return TOK_EOF;
	}

	t = lx->z(lx);

	if (lx->push != NULL) {
		if (-1 == lx->push(lx->buf_opaque, '\0')) {
			return TOK_ERROR;
		}
	}

	return t;
}

