dnl  Power9 mpn_mul_2.

dnl  Contributed to the GNU project by Torbjörn Granlund.

dnl  Copyright 2018 Free Software Foundation, Inc.

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

C         cycles/limb
C power9:    1.58

C STATUS
C  * Not written with any power9 pipeline understanding.
C  * The 4x unrolling was not motivated by any timing tests.
C  * No local scheduling for performance tweaking has been done.
C  * Decrease load scheduling!

define(`rp', `r3')
define(`up', `r4')
define(`n',  `r5')		C Note: Reused as scratch
define(`vp', `r6')		C Note: Reused for v1

define(`v0', `r7')
define(`v1', `r6')


ASM_START()
PROLOGUE(mpn_mul_2)
	std	r28, -32(r1)
	std	r29, -24(r1)
	std	r30, -16(r1)
	std	r31, -8(r1)

	subfic	r0, n, 0	C clear CA
	subfo	r0, r0, r0	C clear OV and r0

	cmpdi	cr7, n, 4

	ld	v0, 0(vp)
	ld	v1, 8(vp)

	srdi	r10, n, 2
	mtctr	r10

	rldicl.	r9, n, 0, 63
	bne	cr0, L(bx1)

L(bx0):	rldicl. r9, n, 63, 63

	ld	r8, 0(up)
	ld	r9, 8(up)
	li	r11, 0
	mulld	r28, r8, v0
	mulhdu	r31, r8, v0
	blt	cr7, L(2)
	mulld	r5, r8, v1
	mulhdu	r10, r8, v1
	bne	cr0, L(b10)

L(b00):	addi	up, up, -8
	addi	rp, rp, -24
	b	L(lo0)

L(b10):	addi	up, up, 8
	addi	rp, rp, -8
	b	L(lo2)

L(2):	addi	rp, rp, -8
	mulld	r5, r8, v1
	mulhdu	r10, r8, v1
	b	L(cj2)

L(bx1):	rldicl. r9, n, 63, 63

	ld	r9, 0(up)
	ld	r8, 8(up)
	li	r10, 0
	mulld	r29, r9, v0
	mulhdu	r30, r9, v0
	mulld	r12, r9, v1
	mulhdu	r11, r9, v1
	bne	cr0, L(b11)

L(b01):	addi	rp, rp, -16
	b	L(lo1)
L(b11):	addi	up, up, 16
	blt	cr7, L(end)

L(top):	ld	r9, 0(up)
	maddld(	r28, r8, v0, r10)	C 0  4   -> adde
	maddhdu(r31, r8, v0, r10)	C 1  5
	adde	r0, r29, r0		C    7 11
	std	r0, 0(rp)
	mulld	r5, r8, v1		C 1  5   -> addex
	mulhdu	r10, r8, v1		C 2  6
	addex(	r0, r12, r30, 0)	C    8 12
L(lo2):	ld	r8, 8(up)
	maddld(	r29, r9, v0, r11)	C 1  5   -> adde
	maddhdu(r30, r9, v0, r11)	C 2  6
	adde	r0, r28, r0		C    8 12
	std	r0, 8(rp)
	mulld	r12, r9, v1		C 2  6   -> addex
	mulhdu	r11, r9, v1		C 3  7
	addex(	r0, r5, r31, 0)		C 5  9 13
L(lo1):	ld	r9, 16(up)
	maddld(	r28, r8, v0, r10)	C 2  6   -> adde
	maddhdu(r31, r8, v0, r10)	C 3  7
	adde	r0, r29, r0		C    5  9 13
	std	r0, 16(rp)
	mulld	r5, r8, v1		C 3  7   -> addex
	mulhdu	r10, r8, v1		C 4  8
	addex(	r0, r12, r30, 0)	C    6 10
L(lo0):	ld	r8, 24(up)
	maddld(	r29, r9, v0, r11)	C 3  7   -> adde
	maddhdu(r30, r9, v0, r11)	C 4  8
	adde	r0, r28, r0		C    6 10
	std	r0, 24(rp)
	mulld	r12, r9, v1		C 4  8   -> addex
	mulhdu	r11, r9, v1		C 5  9
	addex(	r0, r5, r31, 0)		C    7 11
	addi	up, up, 32
	addi	rp, rp, 32
	bdnz	L(top)

L(end):	ld	r9, 0(up)
	maddld(	r28, r8, v0, r10)	C 0  4
	maddhdu(r31, r8, v0, r10)	C 1  5
	adde	r0, r29, r0		C    7 11
	std	r0, 0(rp)		C		-4
	mulld	r5, r8, v1		C 1  5
	mulhdu	r10, r8, v1		C 2  6
	addex(	r0, r12, r30, 0)	C    8 12
L(cj2):	maddld(	r29, r9, v0, r11)	C 1  5		-2
	maddhdu(r30, r9, v0, r11)	C 2  6		-1
	adde	r0, r28, r0		C    8 12	-3
	std	r0, 8(rp)		C		-3
	mulld	r12, r9, v1		C 2  6		-1
	mulhdu	r11, r9, v1		C 3  7		0 = return limb
	addex(	r0, r5, r31, 0)		C 5  9 13
	adde	r0, r29, r0		C    5  9 13	-2
	std	r0, 16(rp)		C		-2
	addex(	r0, r12, r30, 0)	C    6 10	-1
	adde	r0, r0, r10		C		-1
	std	r0, 24(rp)		C		-1
	li	r4, 0
	addze	r3, r11
	addex(	r3, r3, r4, 0)

L(ret):	ld	r28, -32(r1)
	ld	r29, -24(r1)
	ld	r30, -16(r1)
	ld	r31, -8(r1)
	blr
EPILOGUE()
ASM_END()
