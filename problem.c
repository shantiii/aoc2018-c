#include <stdio.h>
#include <stdarg.h>

#include "problem.h"

void answerf(struct solution *out, const char *fmt, ...) {
	va_list arg_list;
	if (out->answered < 0 || out->answered > 2) {
		return;
	}
	va_start(arg_list, fmt);
	vsprintf(out->answers[out->answered], fmt, arg_list);
	va_end(arg_list);
	out->answered++;
}

void print_solution(const struct solution *s) {
	for (size_t i=0; i<s->answered && i<2; ++i) {
		printf("part%lu: %s\n", i+1, s->answers[i]);
	}
}
