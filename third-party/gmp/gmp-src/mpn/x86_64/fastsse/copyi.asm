dnl  AMD64 mpn_copyi optimised for CPUs with fast SSE.

dnl  Copyright 2003, 2005, 2007, 2011, 2012, 2015 Free Software Foundation,
dnl  Inc.

dnl  Contributed to the GNU project by Torbjörn Granlund.

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

C	     cycles/limb     cycles/limb     cycles/limb      good
C              aligned	      unaligned	      best seen	     for cpu?
C AMD K8,K9
C AMD K10	 0.85		 1.64				Y/N
C AMD bull	 1.4		 1.4				N
C AMD pile	 0.77		 0.93				N
C AMD steam	 ?		 ?
C AMD excavator	 ?		 ?
C AMD bobcat
C AMD jaguar	 0.65		 1.02		opt/0.93	Y/N
C Intel P4	 2.3		 2.3				Y
C Intel core	 1.0		 1.0		0.52/0.64	N
C Intel NHM	 0.5		 0.67				Y
C Intel SBR	 0.51		 0.75		opt/0.54	Y/N
C Intel IBR	 0.50		 0.57		opt/0.54	Y
C Intel HWL	 0.50		 0.57		opt/0.51	Y
C Intel BWL	 0.55		 0.62		opt/0.55	Y
C Intel atom
C Intel SLM	 1.02		 1.27		opt/1.07	Y/N
C VIA nano	 1.16		 5.16				Y/N

C We try to do as many 16-byte operations as possible.  The top-most and
C bottom-most writes might need 8-byte operations.  We can always write using
C aligned 16-byte operations, we read with both aligned and unaligned 16-byte
C operations.

C Instead of having separate loops for reading aligned and unaligned, we read
C using MOVDQU.  This seems to work great except for core2; there performance
C doubles when reading using MOVDQA (for aligned source).  It is unclear how to
C best handle the unaligned case there.

C INPUT PARAMETERS
define(`rp', `%rdi')
define(`up', `%rsi')
define(`n',  `%rdx')

ABI_SUPPORT(DOS64)
ABI_SUPPORT(STD64)

dnl define(`movdqu', lddqu)

ASM_START()
	TEXT
	ALIGN(64)
PROLOGUE(mpn_copyi)
	FUNC_ENTRY(3)

	cmp	$3, n			C NB: bc code below assumes this limit
	jc	L(bc)

	test	$8, R8(rp)		C is rp 16-byte aligned?
	jz	L(ali)			C jump if rp aligned
	movsq				C copy single limb
	dec	n

L(ali):	sub	$16, n
	jc	L(sma)

IFDOS(`	add	$-56, %rsp	')
IFDOS(`	movdqa	%xmm6, (%rsp)	')
IFDOS(`	movdqa	%xmm7, 16(%rsp)	')

	ALIGN(16)
L(top):	movdqu	(up), %xmm0
	movdqu	16(up), %xmm1
	movdqu	32(up), %xmm2
	movdqu	48(up), %xmm3
	movdqu	64(up), %xmm4
	movdqu	80(up), %xmm5
	movdqu	96(up), %xmm6
	movdqu	112(up), %xmm7
	lea	128(up), up
	movdqa	%xmm0, (rp)
	movdqa	%xmm1, 16(rp)
	movdqa	%xmm2, 32(rp)
	movdqa	%xmm3, 48(rp)
	movdqa	%xmm4, 64(rp)
	movdqa	%xmm5, 80(rp)
	movdqa	%xmm6, 96(rp)
	movdqa	%xmm7, 112(rp)
	lea	128(rp), rp
	sub	$16, n
	jnc	L(top)

IFDOS(`	movdqa	(%rsp), %xmm6	')
IFDOS(`	movdqa	16(%rsp), %xmm7	')
IFDOS(`	add	$56, %rsp	')

L(sma):	test	$8, R8(n)
	jz	1f
	movdqu	(up), %xmm0
	movdqu	16(up), %xmm1
	movdqu	32(up), %xmm2
	movdqu	48(up), %xmm3
	lea	64(up), up
	movdqa	%xmm0, (rp)
	movdqa	%xmm1, 16(rp)
	movdqa	%xmm2, 32(rp)
	movdqa	%xmm3, 48(rp)
	lea	64(rp), rp
1:
	test	$4, R8(n)
	jz	1f
	movdqu	(up), %xmm0
	movdqu	16(up), %xmm1
	lea	32(up), up
	movdqa	%xmm0, (rp)
	movdqa	%xmm1, 16(rp)
	lea	32(rp), rp
1:
	test	$2, R8(n)
	jz	1f
	movdqu	(up), %xmm0
	lea	16(up), up
	movdqa	%xmm0, (rp)
	lea	16(rp), rp
	ALIGN(16)
1:
L(end):	test	$1, R8(n)
	jz	1f
	mov	(up), %r8
	mov	%r8, (rp)
1:
	FUNC_EXIT()
	ret

C Basecase code.  Needed for good small operands speed, not for correctness as
C the above code is currently written.  The commented-out lines need to be
C reinstated if this code is to be used for n > 3, and then the post loop
C offsets need fixing.

L(bc):	sub	$2, n
	jc	L(end)
	ALIGN(16)
1:	mov	(up), %rax
	mov	8(up), %rcx
dnl	lea	16(up), up
	mov	%rax, (rp)
	mov	%rcx, 8(rp)
dnl	lea	16(rp), rp
dnl	sub	$2, n
dnl	jnc	1b

	test	$1, R8(n)
	jz	L(ret)
	mov	16(up), %rax
	mov	%rax, 16(rp)
L(ret):	FUNC_EXIT()
	ret
EPILOGUE()
