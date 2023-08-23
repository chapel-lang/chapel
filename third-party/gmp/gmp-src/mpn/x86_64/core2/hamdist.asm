dnl  AMD64 SSSE3 mpn_hamdist -- hamming distance.

dnl  Copyright 2010-2017 Free Software Foundation, Inc.

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

C		    cycles/limb	  good for cpu?
C AMD K8,K9		n/a
C AMD K10		n/a
C AMD bd1		 ?
C AMD bd2		 ?
C AMD bd3		 ?
C AMD bd4		 ?
C AMD zen		 ?
C AMD bobcat		 ?
C AMD jaguar		 ?
C Intel P4		n/a
C Intel CNR		 4.50		y
C Intel PNR		 3.28		y
C Intel NHM		 ?
C Intel SBR		 ?
C Intel IBR		 ?
C Intel HWL		 ?
C Intel BWL		 ?
C Intel SKL		 ?
C Intel atom		 ?
C Intel SLM		 ?
C VIA nano		 ?

C TODO
C  * This was hand-written without too much thought about optimal insn
C    selection; check to see of it can be improved.
C  * Consider doing some instruction scheduling.

define(`up',		`%rdi')
define(`vp',		`%rsi')
define(`n',		`%rdx')

ASM_START()
	TEXT
	ALIGN(32)
PROLOGUE(mpn_hamdist)
	lea	L(cnsts)(%rip), %r9

ifdef(`PIC', `define(`OFF1',32) define(`OFF2',48)',
	     `define(`OFF1',64) define(`OFF2',80)')
	movdqa	OFF1`'(%r9), %xmm7
	movdqa	OFF2`'(%r9), %xmm6
	pxor	%xmm4, %xmm4
	pxor	%xmm5, %xmm5
	pxor	%xmm8, %xmm8

	mov	R32(n), R32(%rax)
	and	$7, R32(%rax)
ifdef(`PIC',`
	movslq	(%r9,%rax,4), %rax
	add	%r9, %rax
	jmp	*%rax
',`
	jmp	*(%r9,%rax,8)
')

L(1):	movq	(up), %xmm1
	add	$8, up
	movq	(vp), %xmm10
	add	$8, vp
	pxor	%xmm10, %xmm1
	jmp	L(e1)

L(2):	add	$-48, up
	add	$-48, vp
	jmp	L(e2)

L(3):	movq	(up), %xmm1
	add	$-40, up
	movq	(vp), %xmm10
	add	$-40, vp
	pxor	%xmm10, %xmm1
	jmp	L(e3)

L(4):	add	$-32, up
	add	$-32, vp
	jmp	L(e4)

L(5):	movq	(up), %xmm1
	add	$-24, up
	movq	(vp), %xmm10
	add	$-24, vp
	pxor	%xmm10, %xmm1
	jmp	L(e5)

L(6):	add	$-16, up
	add	$-16, vp
	jmp	L(e6)

L(7):	movq	(up), %xmm1
	add	$-8, up
	movq	(vp), %xmm10
	add	$-8, vp
	pxor	%xmm10, %xmm1
	jmp	L(e7)

	ALIGN(32)
L(top):	lddqu	(up), %xmm1
	lddqu	(vp), %xmm10
	pxor	%xmm10, %xmm1
L(e7):	movdqa	%xmm6, %xmm0		C copy mask register
	movdqa	%xmm7, %xmm2		C copy count register
	movdqa	%xmm7, %xmm3		C copy count register
	pand	%xmm1, %xmm0
	psrlw	$4, %xmm1
	pand	%xmm6, %xmm1
	pshufb	%xmm0, %xmm2
	pshufb	%xmm1, %xmm3
	paddb	%xmm2, %xmm3
	paddb	%xmm3, %xmm4
L(e6):	lddqu	16(up), %xmm1
	lddqu	16(vp), %xmm10
	pxor	%xmm10, %xmm1
L(e5):	movdqa	%xmm6, %xmm0
	movdqa	%xmm7, %xmm2
	movdqa	%xmm7, %xmm3
	pand	%xmm1, %xmm0
	psrlw	$4, %xmm1
	pand	%xmm6, %xmm1
	pshufb	%xmm0, %xmm2
	pshufb	%xmm1, %xmm3
	paddb	%xmm2, %xmm3
	paddb	%xmm3, %xmm4
L(e4):	lddqu	32(up), %xmm1
	lddqu	32(vp), %xmm10
	pxor	%xmm10, %xmm1
L(e3):	movdqa	%xmm6, %xmm0
	movdqa	%xmm7, %xmm2
	movdqa	%xmm7, %xmm3
	pand	%xmm1, %xmm0
	psrlw	$4, %xmm1
	pand	%xmm6, %xmm1
	pshufb	%xmm0, %xmm2
	pshufb	%xmm1, %xmm3
	paddb	%xmm2, %xmm3
	paddb	%xmm3, %xmm4
L(e2):	lddqu	48(up), %xmm1
	add	$64, up
	lddqu	48(vp), %xmm10
	add	$64, vp
	pxor	%xmm10, %xmm1
L(e1):	movdqa	%xmm6, %xmm0
	movdqa	%xmm7, %xmm2
	movdqa	%xmm7, %xmm3
	pand	%xmm1, %xmm0
	psrlw	$4, %xmm1
	pand	%xmm6, %xmm1
	pshufb	%xmm0, %xmm2
	pshufb	%xmm1, %xmm3
	psadbw	%xmm5, %xmm4		C sum to 8 x 16-bit counts
	paddb	%xmm2, %xmm3
	paddq	%xmm4, %xmm8		C sum to 2 x 64-bit counts
	movdqa	%xmm3, %xmm4
	sub	$8, n
	jg	L(top)

	psadbw	%xmm5, %xmm4
	paddq	%xmm4, %xmm8
	pshufd	$14, %xmm8, %xmm0
	paddq	%xmm8, %xmm0
	movd	%xmm0, %rax
	ret
EPILOGUE()
DEF_OBJECT(L(cnsts),16,`JUMPTABSECT')
	JMPENT(	L(top), L(cnsts))
	JMPENT(	L(1), L(cnsts))
	JMPENT(	L(2), L(cnsts))
	JMPENT(	L(3), L(cnsts))
	JMPENT(	L(4), L(cnsts))
	JMPENT(	L(5), L(cnsts))
	JMPENT(	L(6), L(cnsts))
	JMPENT(	L(7), L(cnsts))
	.byte	0x00,0x01,0x01,0x02,0x01,0x02,0x02,0x03
	.byte	0x01,0x02,0x02,0x03,0x02,0x03,0x03,0x04
	.byte	0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f
	.byte	0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f
END_OBJECT(L(cnsts))
