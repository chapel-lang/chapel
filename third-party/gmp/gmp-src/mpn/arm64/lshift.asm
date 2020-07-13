dnl  ARM64 mpn_lshift.

dnl  Copyright 2013, 2014, 2017 Free Software Foundation, Inc.

dnl  This file is part of the GNU MP Library.

dnl  The GNU MP Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 3 of the License, or (at
dnl  your option) any later version.

dnl  The GNU MP Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.

include(`../config.m4')

C	     cycles/limb   assumed optimal c/l
C Cortex-A53	3.5-4.0		 3.25
C Cortex-A57	 2.0		 2.0
C X-Gene	 2.67		 2.5

C TODO
C  * The feed-in code used 1 ldr for odd sized and 2 ldr for even sizes.  These
C    numbers should be 1 and 0, respectively.  The str in wind-down should also
C    go.
C  * Using extr and with 63 separate loops we might reach 1.25 c/l on A57.
C  * A53's speed depends on alignment, tune/speed -w1 gives 3.5, -w0 gives 4.0.

changecom(blah)

define(`rp_arg', `x0')
define(`up',     `x1')
define(`n',      `x2')
define(`cnt',    `x3')

define(`rp',     `x16')

define(`tnc',`x8')

define(`PSHIFT', lsl)
define(`NSHIFT', lsr)

ASM_START()
PROLOGUE(mpn_lshift)
	add	rp, rp_arg, n, lsl #3
	add	up, up, n, lsl #3
	sub	tnc, xzr, cnt
	lsr	x18, n, #2
	tbz	n, #0, L(bx0)

L(bx1):	ldr	x4, [up,#-8]
	tbnz	n, #1, L(b11)

L(b01):	NSHIFT	x0, x4, tnc
	PSHIFT	x2, x4, cnt
	cbnz	x18, L(gt1)
	str	x2, [rp,#-8]
	ret
L(gt1):	ldp	x4, x5, [up,#-24]
	sub	up, up, #8
	add	rp, rp, #16
	b	L(lo2)

L(b11):	NSHIFT	x0, x4, tnc
	PSHIFT	x2, x4, cnt
	ldp	x6, x7, [up,#-24]!
	b	L(lo3)

L(bx0):	ldp	x4, x5, [up,#-16]
	tbz	n, #1, L(b00)

L(b10):	NSHIFT	x0, x5, tnc
	PSHIFT	x13, x5, cnt
	NSHIFT	x10, x4, tnc
	PSHIFT	x2, x4, cnt
	cbnz	x18, L(gt2)
	orr	x10, x10, x13
	stp	x2, x10, [rp,#-16]
	ret
L(gt2):	ldp	x4, x5, [up,#-32]
	orr	x10, x10, x13
	str	x10, [rp,#-8]
	sub	up, up, #16
	add	rp, rp, #8
	b	L(lo2)

L(b00):	NSHIFT	x0, x5, tnc
	PSHIFT	x13, x5, cnt
	NSHIFT	x10, x4, tnc
	PSHIFT	x2, x4, cnt
	ldp	x6, x7, [up,#-32]!
	orr	x10, x10, x13
	str	x10, [rp,#-8]!
	b	L(lo0)

	ALIGN(16)
L(top):	ldp	x4, x5, [up,#-16]
	orr	x10, x10, x13
	orr	x11, x12, x2
	stp	x10, x11, [rp,#-16]
	PSHIFT	x2, x6, cnt
L(lo2):	NSHIFT	x10, x4, tnc
	PSHIFT	x13, x5, cnt
	NSHIFT	x12, x5, tnc
	ldp	x6, x7, [up,#-32]!
	orr	x10, x10, x13
	orr	x11, x12, x2
	stp	x10, x11, [rp,#-32]!
	PSHIFT	x2, x4, cnt
L(lo0):	sub	x18, x18, #1
L(lo3):	NSHIFT	x10, x6, tnc
	PSHIFT	x13, x7, cnt
	NSHIFT	x12, x7, tnc
	cbnz	x18, L(top)

L(end):	orr	x10, x10, x13
	orr	x11, x12, x2
	PSHIFT	x2, x6, cnt
	stp	x10, x11, [rp,#-16]
	str	x2, [rp,#-24]
	ret
EPILOGUE()
