/*
 * Author: Aurelio Colosimo, 2016
 * Released under MIT-Expat License.
 */

/* Tool to compute Cooley–Tukey FFT twiddle factors and generate
 * a .h and a .c file which can be compiled and linked to your application
 *
 * Usage: psdr-twiddle N nbits output.h output.c
 *        N: number of taps
 *        nbits: number of bits of each real/imaginary value
 *        output.h: filename for generated .h
 *        output.c: filename for generated .c
 */

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <libgen.h>

#define EXE "psdr-twiddle"

#define COMMON_HEADER \
    "/*\n * File %s\n * Automatically generated by " EXE " tool\n" \
    " * Command line:"

#define COMMON_IDX_COMMENT "/* First index: i; second index: k */\n"

int main(int argc, char** argv)
{
	unsigned N, nbits, i, k;
	const unsigned max_nbits = sizeof(long long) * 8 - 1;
	FILE *fh = NULL, *fc = NULL;
	float factor;
	long long intfactor;

	if (argc < 5) {
		printf("\n" EXE " - Tool to compute Cooley-Tukey FFT twiddles\n\n");
		printf("Usage: " EXE " N nbits output.h output.c\n");
		printf("\tN: number of taps\n");
		printf("\tnbits: nbits: number of bits of each real/imaginary value\n");
		printf("\toutput.h: filename for generated .h\n");
		printf("\toutput.c: filename for generated .c\n");
		printf("\n");
		return 1;
	}

	N = strtoul(argv[1], NULL, 10);
	if (!N) {
		printf("Invalid N parameter: %s\n", argv[1]);
		return 1;
	}

	nbits = strtoul(argv[2], NULL, 10);
	if (!nbits || nbits > max_nbits) {
		printf("Invalid nbits parameter: %s, max %d\n", argv[2], max_nbits);
		return 2;
	}

	fh = fopen(argv[3], "w");
	if (!fh) {
		printf("Unable to open %s in write mode.\n", argv[3]);
		return 3;
	}

	fc = fopen(argv[4], "w");
	if (!fc) {
		printf("Unable to open %s in write mode.\n", argv[4]);
		fclose(fh);
		return 4;
	}

	fprintf(fh, COMMON_HEADER, argv[3]);
	fprintf(fc, COMMON_HEADER, argv[4]);

	for (i = 0; i < argc; i++) {
		fprintf(fh, " %s", argv[i]);
		fprintf(fc, " %s", argv[i]);
	}

	fprintf(fh, "\n */\n\n");
	fprintf(fc, "\n */\n\n");

	fprintf(fc, "#include \"%s\"\n\n", basename(argv[3]));

	fprintf(fc, COMMON_IDX_COMMENT "\n");

	fprintf(fh, "#include <arith.h>\n\n");
	fprintf(fh, COMMON_IDX_COMMENT);
	fprintf(fh, "extern const cpx_t twiddle%d_%db[%d][%d];\n",
	    N, nbits, N, N / 2);

	fclose(fh);

	fprintf(fc, "const cpx_t twiddle%d_%db[%d][%d] = {\n",
	    N, nbits, N, N / 2);

	factor = powf(2, nbits - 1);
	intfactor = llround(factor);

	for (i = 0; i < N; i++) {
		fprintf(fc, "\n\t{");
		if (i % 4 == 0)
			fprintf(fc, " /* i = %u */", i);
		for (k = 0; k < N / 2; k++) {
			float phi = (-2 * M_PI / N) * i * k;
			float sf = sin(phi);
			float cf = cos(phi);
			long long c, s, f;
			if (k % 4 == 0)
				fprintf(fc, "\n\t\t");
			c = llroundf(cf * factor);
			s = llroundf(sf * factor);
			if (c == intfactor)
				c--;
			if (s == intfactor)
				s--;
			fprintf(fc, "{%lld, %lld}, ", c, s);
		}
		fprintf(fc, "\n\t},\n");
	}

	fprintf(fc, "\n};\n");

	fclose(fc);

	return 0;
}
