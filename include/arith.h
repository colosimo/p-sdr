/*
 * Author: Aurelio Colosimo, 2016
 * Released under MIT-Expat License.
 */

#ifndef _ARITH_H_
#define _ARITH_H_

extern const unsigned int smp_nbits;

#ifndef INTP
typedef int intp;
typedef unsigned uintp;
#else
typedef INTP intp;
typedef UINTP uintp;
#endif

#define BPS (sizeof(intp) * 8)

#define MININTP ((intp)((uintp)1 << (BPS - 1)))
#define MAXINTP (~MININTP)

#ifndef NULL
#define NULL ((void*)0)
#endif

struct cpx {
	intp i;	/* Real */
	intp q;	/* Imaginary */
};
typedef struct cpx cpx_t;

/*
 * Real numbers arithmetic
 */

static inline intp sum(intp x, intp y)
{
	return x + y;
}

/* Correct neg of -MININTP, as MAXINTP ~= -MININTP */
static inline intp neg(intp x)
{
	if (x == MININTP)
		return MAXINTP;
	return -x;
}

static inline int sign(intp x)
{
	return x > 0 ? 1 : -1;
}

/* Safe abs: handle abs(MININTP) using safe neg (above) */
static inline intp s_abs(intp x)
{
	if (x < 0)
		return neg(x);
	return x;
}

static inline intp mul(intp x, intp y)
{
	return x * y;
}

static inline intp scale(intp x, intp num, intp den)
{
	return x * num / den;
}

/* safe log2: compute base-2 logarithm of x; return 0 if x=0 */
static inline uintp s_log2(uintp x)
{
	uintp r = 0;
	while (x >>= 1)
		r++;
	return r;
}

/*
 * Complex numbers arithmetic
 */

static inline cpx_t cpx_assign(cpx_t x)
{
	cpx_t r;
	r.i = x.i;
	r.q = x.q;
	return r;
}

static inline cpx_t cpx_iq(intp i, intp q)
{
	cpx_t r;
	r.i = i;
	r.q = q;
	return r;
}

static inline cpx_t cpx_sum(cpx_t x, cpx_t y)
{
	cpx_t r;
	r.i = sum(x.i, y.i);
	r.q = sum(x.q, y.q);
	return r;
}

static inline cpx_t cpx_neg(cpx_t x)
{
	cpx_t r;
	r.i = neg(x.i);
	r.q = neg(x.q);
	return r;
}

static inline cpx_t cpx_mul(cpx_t x, cpx_t y)
{
	cpx_t r;
	r.i = sum(mul(x.i, y.i), neg(mul(x.q, y.q)));
	r.q = sum(mul(x.i, y.q), mul(x.q, y.i));
	return r;
}

static inline cpx_t cpx_scale(cpx_t x, intp num, intp den)
{
	cpx_t r;
	r.i = scale(x.i, num, den);
	r.q = scale(x.q, num, den);
	return r;
}

static inline intp cpx_pow(cpx_t x)
{
	return sum(mul(x.i, x.i), mul(x.q, x.q));
}

/*
 * Cordic algorithm to compute phase and magnitude
 */

/* cpx_to_polar_cordic: basic cordic algorithm, working for phase < 90° */
static inline void cpx_to_polar_cordic(cpx_t x, intp *phase, uintp *mag)
{
	unsigned i;
	cpx_t x0, x1;
	x0 = cpx_assign(x);
	int sigma = -sign(x.q);
	intp step = MAXINTP / 4;
	intp ph = 0;

	for (i = 0; i < BPS - 2; i++) {
		x1.i = sum(x0.i, mul(-sigma, x0.q >> i));
		x1.q = sum(x0.q, mul(sigma, x0.i >> i));
		ph = sum(ph, neg(mul(sigma, step)));
		step = step >> 1;
		sigma = -sign(x1.q);
		x0 = x1;
	}

	/* lim(K) -> ~= 0.60725: is approximated to 3/5 */
	if (mag)
		*mag = scale(x0.i, 3, 5);

	if (phase)
		*phase = ph;
}

/* cpx_to_polar: compute any phase and magnitude properly mirroring on q-axis */
static inline void cpx_to_polar(cpx_t x, intp *phase, uintp *mag)
{
	cpx_t tmp;
	if (x.i > 0)
		cpx_to_polar_cordic(x, phase, mag);
	else {
		tmp = cpx_iq(neg(x.i), x.q);
		cpx_to_polar_cordic(tmp, phase, mag);
		if (!phase)
			return;
		if (x.q > 0)
			*phase = sum(MAXINTP, neg(*phase));
		else
			*phase = sum(MININTP, neg(*phase));
	}
}

static inline uintp cpx_mag(cpx_t x)
{
	uintp mag;
	cpx_to_polar(x, NULL, &mag);
	return mag;
}

static inline intp cpx_phase(cpx_t x)
{
	intp phase;
	cpx_to_polar(x, &phase, NULL);
	return phase;
}

/* cpx_bitrev_reorder: perform bit reversal reordering on a cpx_t array, given
 * the proper LUT as input */
static inline void cpx_bitrev_reorder(cpx_t *v, unsigned N, const int *bitrev)
{
	int i;
	cpx_t tmp;
	for (i = 0; i < N; i++) {
		if (i >= bitrev[i])
			continue;
		tmp = v[i];
		v[i] = v[bitrev[i]];
		v[bitrev[i]] = tmp;
	}
}

#endif /* _ARITH_H_ */
