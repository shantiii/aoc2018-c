
struct solution {
	char answers[256][2];
	unsigned int answered;
};

/* Add an answer to the provided solution */
void answerf(struct solution *, const char *fmt, ...);
/* Display the solution to stdout */
void print_solution(const struct solution *);
