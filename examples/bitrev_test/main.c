/*
 * Author: Aurelio Colosimo, 2016
 * Released under MIT-Expat License.
 */

#include <psdrlib.h>
#include "bitrev16.h"

static inline void cpx_dump(cpx_t x)
{
	psdr_printf("{%s,%s}", intp2str(x.i), intp2str(x.q));
}

int main(int argc, char **argv)
{
	cpx_t vec[16];
	int i;
	psdr_printf("Before:\n");
	for (i = 0; i < 16; i++) {
		vec[i] = cpx_iq(i, -i);
		cpx_dump(vec[i]);
		psdr_printf(",");
	}

	cpx_bitrev_reorder(vec, 16, bitrev16);

	psdr_printf("\nAfter:\n");
	for (i = 0; i < 16; i++) {
		cpx_dump(vec[i]);
		psdr_printf(",");
	}
	psdr_printf("\n\n");

	return 0;

}
