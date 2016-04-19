/*
 * Author: Aurelio Colosimo, 2016
 * Released under MIT-Expat License.
 */

#include <psdrlib.h>

#ifdef _ARCH_UNIX_H_
#include <math.h>
#endif

int main(int argc, char **argv)
{
	int i;
	int q;
	int k;
	intp ph;
	uintp mag;
	srand(time(NULL));
	for (k = 0; k < 10; k++) {
		i = rand() % 1024 - 512;
		q = rand() % 1024 - 512;
		cpx_t x = cpx_iq(i, q);
		cpx_to_polar(x, &ph, &mag);
		psdr_printf("I=%d Q=%d\n", i, q);
#ifdef _ARCH_UNIX_H_
		psdr_printf("Exact:  Mag=%.0f Phase %.6f * pi\n",
					sqrt(i*i+q*q), atan2(q, i) / M_PI);
		psdr_printf("Cordic: Mag=%s Phase %.6f * pi\n",
					uintp2str(mag), (float)ph / MAXINTP);
#else
		psdr_printf("Cordic: Mag=%s Phase %s\n",
					uintp2str(mag), intp2str(ph));
#endif
		psdr_printf("\n");
	}

	return 0;
}
