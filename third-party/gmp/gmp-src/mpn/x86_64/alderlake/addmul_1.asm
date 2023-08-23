dnl  AMD64 mpn_addmul_1 for CPUs with mulx and adx.

dnl  Contributed to the GNU project by Torbjörn Granlund.

dnl  Copyright 2012, 2013, 2022 Free Software Foundation, Inc.

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

C	     cycles/limb
C AMD K8,K9	 -
C AMD K10	 -
C AMD bd1	 -
C AMD bd2	 -
C AMD bd3	 -
C AMD bd4	 -
C AMD zn1	 ?
C AMD zn2	 ?
C AMD zn3	 ?
C AMD bt1	 -
C AMD bt2	 -
C Intel P4	 -
C Intel CNR	 -
C Intel PNR	 -
C Intel NHM	 -
C Intel WSM	 -
C Intel SBR	 -
C Intel IBR	 -
C Intel HWL	 -
C Intel BWL	 ?
C Intel SKL	 ?
C Intel RKL	 ?
C Intel ALD	 1.29
C Intel atom	 -
C Intel SLM	 -
C Intel GLM	 -
C VIA nano	 -

define(`rp',      `%rdi')	dnl rcx
define(`up',      `%rsi')	dnl rdx
define(`n_param', `%rdx')	dnl r8
define(`v0_param',`%rcx')	dnl r9

define(`n',       `%rcx')	dnl
define(`v0',      `%rdx')	dnl


ASM_START()
	TEXT
	ALIGN(16)
PROLOGUE(mpn_addmul_1)
	mov	(up), %r8

	push	%rbx
	push	%r12
	push	%r13

	mov	%rdx, %rax
	mov	%rcx, v0
	mov	%rax, n

	and	$3, R8(%rax)
	jz	L(b0)
	cmp	$2, R8(%rax)
	jl	L(b1)
	jz	L(b2)

L(b3):	mulx(	%r8, %r11, %r10)
	mulx(	8,(up), %r13, %r12)
	mulx(	16,(up), %rbx, %rax)
	inc	n
	lea	-8(up), up
	lea	-24(rp), rp
	jmp	L(lo3)

L(b0):	mulx(	%r8, %r9, %r8)
	mulx(	8,(up), %r11, %r10)
	mulx(	16,(up), %r13, %r12)
	lea	-16(rp), rp
	jmp	L(lo0)

L(b2):	mulx(	%r8, %r13, %r12)
	mulx(	8,(up), %rbx, %rax)
	lea	-2(n), n
	jrcxz	L(n2)
	mulx(	16,(up), %r9, %r8)
	lea	16(up), up
	jmp	L(lo2)
L(n2):	jmp	L(wd2)

L(b1):	mulx(	%r8, %rbx, %rax)
	sub	$1, n
	jrcxz	L(n1)
	mulx(	8,(up), %r9, %r8)
	mulx(	16,(up), %r11, %r10)
	lea	8(up), up
	lea	-8(rp), rp
	jmp	L(lo1)
L(n1):	add	(rp), %rbx
	adc	%rcx, %rax
	mov	%rbx, (rp)
	pop	%r13
	pop	%r12
	pop	%rbx
	ret

L(top):	mulx(	(up), %r9, %r8)
	adcx(	%r10, %r13)
	mov	%r11, -8(rp)
L(lo2):	adox(	(rp), %r13)
	mulx(	8,(up), %r11, %r10)
	adcx(	%r12, %rbx)
	mov	%r13, (rp)
L(lo1):	adox(	8,(rp), %rbx)
	mulx(	16,(up), %r13, %r12)
	adcx(	%rax, %r9)
	mov	%rbx, 8(rp)
L(lo0):	adox(	16,(rp), %r9)
	mulx(	24,(up), %rbx, %rax)
	adcx(	%r8, %r11)
	mov	%r9, 16(rp)
L(lo3):	adox(	24,(rp), %r11)
	lea	32(up), up
	lea	32(rp), rp
	lea	-4(n), n
	jrcxz	L(end)
	jmp	L(top)

L(end):	adcx(	%r10, %r13)
	mov	%r11, -8(rp)
L(wd2):	adox(	(rp), %r13)
	adcx(	%r12, %rbx)
	mov	%r13, (rp)
	adox(	8,(rp), %rbx)
	adcx(	%rcx, %rax)
	adox(	%rcx, %rax)
	mov	%rbx, 8(rp)
	pop	%r13
	pop	%r12
	pop	%rbx
	ret
EPILOGUE()
ASM_END()
