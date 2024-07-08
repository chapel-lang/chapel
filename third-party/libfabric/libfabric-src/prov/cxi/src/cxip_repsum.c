/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2022 Hewlett Packard Enterprise Development LP
 */

/* Notes:
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "cxip.h"

/**
 * @brief REPRODUCIBLE SUM IMPLEMENATION
 *
 * - Reference:
 *   - https://www2.eecs.berkeley.edu/Pubs/TechRpts/2016/EECS-2016-121.pdf
 *     Algorithm 7
 * - Example Code:
 *   - https://github.com/peterahrens/ReproBLAS.git
 *
 * This code supplies the software component of the RSDG Reproducible Sum
 * collective reduction operation.
 *
 * Conceptually, the 52-bit mantissa of a double precision IEEE floating point
 * value, extended to 53-bits to include the "hidden" bit, is placed in a
 * register containing 2048 bits (the full possible range of IEEE double
 * exponents) and shifted so that the MSBit of the mantissa is aligned with the
 * 11-bit exponent.
 *
 * This large register is then divided into numbered "bins" of W bits. Each bin
 * is then expanded by adding (64 - W) zero bits to the most-significant end of
 * each bin, and these 64-bit quantities are copied into an array of Kt 64-bit
 * registers, along with the bin number M in which the MSBit of the value is
 * located.
 *
 * The extra space in each bin allow us to sum without carry from bin-to-bin
 * until the end of the computation. With W=40, there are 24 bits of overflow,
 * allowing at least 2^24 summations to occur before overflow can occur.
 *
 * If overflow does occur, both Rosetta and this software set an overflow flag,
 * and the final result should be treated as invalid.
 *
 * Low order bits can be discarded in the process, and this will set an inexact
 * flag. The result should still be reproducible, and accurate to within
 * round-off error.
 */

#define W	40
#define Kt	4

/* special values of M for non-numbers */
#define	MNInf	125
#define	MInf	126
#define	MNaN	127

/**
 * @Description
 *
 * BIN() converts the exponent 'e' to a W-bit bin number.
 *
 * OFF() provides the offset of exponent 'e' within the W-bit bin.
 *
 * MSK() provides a bitmask for the W LSBits.
 */
#define BIN(e)	(((e) - 1023 + 1024*W)/W - 1024)
#define OFF(e)	((e) - 1023 - W*BIN(e))
#define	MSK(w)	((1ULL << w) - 1)

/**
 * @brief Convert double to repsum
 *
 * Rosetta expects T[0] to be the LSBits of the value, so we load from Kt-1
 * downward. Because W=40, T[0] will always be zero: 53 bits of mantissa cannot
 * span more than three 40-bit registers, regardless of alignment.
 *
 * Note that injection of a sNaN will set the invalid bit.
 *
 * @param x returned repsum object
 * @param d double to convert
 */
void cxip_dbl_to_rep(struct cxip_repsum *x, double d)
{
	unsigned long m;	// double mantissa
	int e;			// double exponent
	int s;			// double sign
	int w;			// bin offset of MSbit
	int lsh;		// left-shift amount
	int rem;		// remaining bits to shift
	int siz;		// number of bits to keep
	int i;

	memset(x, 0, sizeof(*x));
	_decompose_dbl(d, &s, &e, &m);
	if (isnan(d)) {
		// NaN, bit 51 clear is sNaN, sign ignored
		x->M = MNaN;
		w = 0;
		m = 0;
		// injecting sNaN sets the invalid bit
		x->invalid = !(m & 0x0008000000000000);
	} else if (isinf(d)) {
		// inf, sign captured in x->M
		x->M = (s < 0) ? MNInf : MInf;
		w = 0;
		m = 0;
		// injecting inf sets the overflow bit
		x->overflow = true;
		x->overflow_id = 3;
	} else if (e) {
		// Normal values, extend m with implicit MSBit == 1
		x->M = BIN(e);
		w = OFF(e);
		m |= 1ULL << 52;
	} else {
		// Subnormal values, zero
		x->M = BIN(1);
		w = OFF(1);
 	}

	/**
	 * Copy the mantissa into the correct locations within T[].
	 *
	 * T[3] should contain the w+1 MSBits of m, aligned to bit 0.
	 * T[2] should contain the next W bits, aligned to bit W-1.
	 * T[1] should contain any remaining bits, aligned to bit W-1.
	 * T[0] will always be zero.
	 */
	rem = 53;	// number of bits to process
	siz = w + 1;	// bits to include in MSRegister
	lsh = 0;	// left-shift to align
	i = Kt;		// start with most significant
	while (rem) {
		x->T[--i] = s*((m >> (rem - siz)) << lsh);
		rem -= siz;	// siz MSBits consumed
		m &= MSK(rem);	// keep only rem LSBits
		siz = (rem < W) ? rem : W;
		lsh = W - siz;	// align to bit W-1
	}
	while (i)
		x->T[--i] = 0;	// clear remaining bins
}

/**
 * @brief Convert repsum back to double.
 *
 * Simply use scalbn() to scale the signed mantissas and add to the accumulator.
 *
 * @param x repsum object
 * @return double returned value
 */
void cxip_rep_to_dbl(double *d, const struct cxip_repsum *x)
{
	int i, m;

	*d = 0.0;
	switch (x->M) {
		case MNaN:	// quiet NaN only
			*d = NAN;
			return;
		case MNInf:
			*d = -INFINITY;
			return;
		case MInf:
			*d = INFINITY;
			return;
	}
	m = x->M;
	for (i = Kt-1; i >= 0; i--) {
		*d += scalbn(1.0*(int64_t)x->T[i], W*m);
		m--;
	}
}

/**
 * @brief Add two repsum objects, and return the result in x.
 *
 * @param x accumulator
 * @param y added to accumulator
 */
void cxip_rep_add(struct cxip_repsum *x, const struct cxip_repsum *y)
{
	struct cxip_repsum swap;
	int i, j;

	/* swap x and y if necessary, to make x the largest M.
	 * NaN is largest, followed by +Inf, -Inf, and numbers
	 */
	if (y->M > x->M) {
		memcpy(&swap, x, sizeof(struct cxip_repsum));
		memcpy(x, y, sizeof(struct cxip_repsum));
		y = (const struct cxip_repsum *)&swap;
	}
	/* +Inf > -Inf, and if added, promote to NaN */
	if (x->M == MInf && y->M == MNInf) {
		x->M = MNaN;
		/* subtracting infinities sets the invalid bit */
		x->invalid = true;
	}
	/* Handle the not-numbers */
	if (x->M == MNaN || x->M == MInf || x->M == MNInf)
		return;
	/* inexact always propagates, no matter how small */
	if (y->inexact)
		x->inexact = true;
	/* advance j until bins are aligned, note bits discarded */
	for (j = 0; j < Kt && j + y->M < x->M; j++) {
		if (y->T[j])
			x->inexact = true;
	}
	/* any remaining overflow propagates */
	if (y->overflow && y->overflow_id >= j) {
		x->overflow = true;
		x->overflow_id = y->overflow_id - j;
	}
	/* Add remaining y to x in each aligned bin, check for overflow */
	for (i = 0; i < Kt && j < Kt; i++, j++) {
		int sgn0, sgn1;

		sgn0 = x->T[i] >> 63;
		x->T[i] += y->T[j];
		sgn1 = x->T[i] >> 63;
		/* sign change in wrong direction */
		if (sgn0 != sgn1 && sgn1 != y->T[j] >> 63) {
			x->inexact = true;
			x->overflow = true;
			x->overflow_id = MAX(x->overflow_id, i);
		}
	}
}

/**
 * @brief Add two doubles using the repsum method.
 *
 * @param d1 : operand 1
 * @param d2 : operand 2
 * @return double result
 */
double cxip_rep_add_dbl(double d1, double d2)
{
	struct cxip_repsum x, y;

	cxip_dbl_to_rep(&x, d1);
	cxip_dbl_to_rep(&y, d2);
	cxip_rep_add(&x, &y);
	cxip_rep_to_dbl(&d1, &x);

	return d1;
}

/**
 * @brief Sum over a list of values.
 *
 * @param count   : count of values
 * @param values  : array of values to sum
 * @return double result
 */
double cxip_rep_sum(size_t count, double *values)
{
	struct cxip_repsum x, y;
	double d;
	size_t i;

	if (count <= 0)
		return 0.0;
	if (count == 1)
		return values[0];

	cxip_dbl_to_rep(&x, values[0]);
	for (i = 1; i < count; i++) {
		cxip_dbl_to_rep(&y, values[i]);
		cxip_rep_add(&x, &y);
	}
	cxip_rep_to_dbl(&d, &x);
	return d;
}
