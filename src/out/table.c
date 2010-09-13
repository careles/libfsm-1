/* $Id$ */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <fsm/fsm.h>

#include "out/out.h"
#include "libfsm/set.h"
#include "libfsm/internal.h"

static int escputc(int c, FILE *f) {
	assert(f != NULL);

	switch (c) {
	case FSM_EDGE_EPSILON:
		fprintf(f, "epsilon");
		return 7;

	case '\"':
		fprintf(f, "\\\"");
		return 2;

	/* TODO: others */

	default:
		if (!isprint(c)) {
			return printf("0x%02X", (unsigned char) c);
		}

		putc(c, f);
		return 1;
	}
}

static void hr(FILE *f, struct fsm_state *sl) {
	struct fsm_state *x;

	assert(f != NULL);
	assert(sl != NULL);

	fprintf(f, "----------");
	for (x = sl; x; x = x->next) {
		fprintf(f, "+----");
	}

	fprintf(f, "\n");
}

static int notransitions(struct fsm_state *sl, int i) {
	struct fsm_state *x;

	assert(i >= 0);
	assert(i <= FSM_EDGE_MAX);

	for (x = sl; x; x = x->next) {
		if (x->edges[i] != NULL) {
			return 0;
		}
	}

	return 1;
}

void out_table(const struct fsm *fsm, FILE *f) {
	struct fsm_state *x;
	int i;

	/* TODO: assert! */

	fprintf(f, "%-9s ", "");
	for (x = fsm->sl; x; x = x->next) {
		fprintf(f, "| %-2u ", x->id);
	}

	fprintf(f, "\n");

	hr(f, fsm->sl);

	/* TODO: this is sideways. now we have single-char edges only, pivot */

	for (i = 0; i <= FSM_EDGE_MAX; i++) {
		{
			int n = 0;

			/* TODO: print "?" for edges which are all equal */

			/* skip edges with no transitions */
			if (notransitions(fsm->sl, i)) {
				continue;
			}

			n = escputc(i, f);
			if (n > 9) {
				n = 9;
			}

			assert(n >= 0);
			fprintf(f, "%.*s ", 9 - n, "         ");
		}

		for (x = fsm->sl; x; x = x->next) {
			if (x->edges[i] == NULL) {
				fprintf(f, "|    ");
			} else {
				assert(x->edges[i]->state != NULL);

				fprintf(f, "| %-2u ", x->edges[i]->state->id);
			}
		}

		fprintf(f, "\n");
	}

	hr(f, fsm->sl);

	fprintf(f, "%-9s ", "start/end");

	for (x = fsm->sl; x; x = x->next) {
		fprintf(f, "| %s%s ",
			x == fsm_getstart(fsm) ? "S" : " ",
			fsm_isend(fsm, x)      ? "E" : " ");
	}

	fprintf(f, "\n");
}

