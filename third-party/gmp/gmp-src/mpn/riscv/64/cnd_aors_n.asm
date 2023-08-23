dnl  RISC-V/64 mpn_cnd_add_n and mpn_cnd_sub_n.

dnl  Copyright 2016, 2023 Free Software Foundation, Inc.

dnl  This file is part of the GNU MP Library.
dnl
dnl  The GNU MP Library is free software; you can redistribute it and/or modify
dnl  it under the terms of either:
dnl
dnl    * the GNU Lesser General Public License as published by the Free
dnl      Software Foundation; either version 3 of the License, or (at your
dnl      option) any later version.
dnl
dnl  or
dnl
dnl    * the GNU General Public License as published by the Free Software
dnl      Foundation; either version 2 of the License, or (at your option) any
dnl      later version.
dnl
dnl  or both in parallel, as here.
dnl
dnl  The GNU MP Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
dnl  for more details.
dnl
dnl  You should have received copies of the GNU General Public License and the
dnl  GNU Lesser General Public License along with the GNU MP Library.  If not,
dnl  see https://www.gnu.org/licenses/.

include(`../config.m4')

C  INPUT PARAMETERS
define(`cnd',	`a0')
define(`rp',	`a1')
define(`up',	`a2')
define(`vp',	`a3')
define(`n',	`a4')

define(`mask',	`t5')

ifdef(`OPERATION_cnd_add_n',`
    define(`ADDSUB',	`add')
    define(`CMPCY',	`sltu	$1, $2, $3')
    define(`func',	`mpn_cnd_add_n')
')
ifdef(`OPERATION_cnd_sub_n',`
    define(`ADDSUB',	`sub')
    define(`CMPCY',	`sltu	$1, $3, $4')
    define(`func',	`mpn_cnd_sub_n')
')

MULFUNC_PROLOGUE(mpn_cnd_add_n mpn_cnd_sub_n)

ASM_START()
PROLOGUE(func)
	li	t6, 0

	sltiu	mask, cnd, 1
	addi	mask, mask, -1

	andi	t0, n, 1
	beq	t0, x0, L(top)
	addi	up, up, 8
	addi	vp, vp, -8
	addi	rp, rp, -8
	addi	n, n, -1
	j	L(mid)

L(top):	ld	a0, 0(up)
	ld	a6, 0(vp)
	addi	n, n, -2	C bookkeeping
	addi	up, up, 16	C bookkeeping
	and	a6, a6, mask
	ADDSUB	t0, a0, a6
	CMPCY(	t2, t0, a0, a6)
	ADDSUB	t4, t0, t6	C cycle 3, 9, ...
	CMPCY(	t3, t4, t0, t6)	C cycle 4, 10, ...
	sd	t4, 0(rp)
	add	t6, t2, t3	C cycle 5, 11, ...
L(mid):	ld	a5, -8(up)
	ld	a7, 8(vp)
	addi	vp, vp, 16	C bookkeeping
	addi	rp, rp, 16	C bookkeeping
	and	a7, a7, mask
	ADDSUB	t1, a5, a7
	CMPCY(	t2, t1, a5, a7)
	ADDSUB	t4, t1, t6	C cycle 0, 6, ...
	CMPCY(	t3, t4, t1, t6)	C cycle 1, 7, ...
	sd	t4, -8(rp)
	add	t6, t2, t3	C cycle 2, 8, ...
	bne	n, x0, L(top)	C bookkeeping

L(end):	mv	a0, t6
	ret
EPILOGUE()
ASM_END()
