/*      Definitions of target machine GNU compiler.  IA-64 version.
	Copyright (C) 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
	Contributed by James E. Wilson <wilson@cygnus.com> and
	               David Mosberger <davidm@hpl.hp.com>.

	This file is part of GNU CC.

	GNU CC is free software	;  you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation;  either version 2, or (at your option)
	any later version.

	GNU CC is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY;  without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with GNU CC	;  see the file COPYING.  If not, write to
	the Free Software Foundation, 59 Temple Place - Suite 330,
	Boston, MA 02111-1307, USA.  */
	
	
// Compute a 32-bit integer quotient.
//
// From the Intel IA-64 Optimization Guide, choose the minimum latency
// alternative.
//
// in0 holds the dividend.  in1 holds the divisor.

#if (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
	
	.text
	.align 16
	.global __divsi3
	.proc __divsi3
__divsi3:
	.regstk 2,0,0,0
	sxt4 in0 = in0
	sxt4 in1 = in1
	;;
	setf.sig f8 = in0
	setf.sig f9 = in1
	;;
	mov r2 = 0x0ffdd
	fcvt.xf f8 = f8
	fcvt.xf f9 = f9
	;;
	setf.exp f11 = r2
	frcpa.s1 f10, p6 = f8, f9
	;;
(p6)	fmpy.s1 f8 = f8, f10
(p6)	fnma.s1 f9 = f9, f10, f1
	;;
(p6)	fma.s1 f8 = f9, f8, f8
(p6)	fma.s1 f9 = f9, f9, f11
	;;
(p6)	fma.s1 f10 = f9, f8, f8
	;;
	fcvt.fx.trunc.s1 f10 = f10
	;;
	getf.sig ret0 = f10
	br.ret.sptk rp
	;;
	.endp __divsi3

// Compute a 32-bit integer modulus.
//
// From the Intel IA-64 Optimization Guide, choose the minimum latency
// alternative.
//
// in0 holds the dividend.  in1 holds the divisor.

	.text
	.align 16
	.global __modsi3
	.proc __modsi3
__modsi3:
	.regstk 2,0,0,0
	mov r2 = 0x0ffdd
	sxt4 in0 = in0
	sxt4 in1 = in1
	;;
	setf.sig f13 = r32
	setf.sig f9 = r33
	;;
	sub in1 = r0, in1
	fcvt.xf f8 = f13
	fcvt.xf f9 = f9
	;;
	setf.exp f11 = r2
	frcpa.s1 f10, p6 = f8, f9
	;;
(p6)	fmpy.s1 f12 = f8, f10
(p6)	fnma.s1 f10 = f9, f10, f1
	;;
	setf.sig f9 = in1
(p6)	fma.s1 f12 = f10, f12, f12
(p6)	fma.s1 f10 = f10, f10, f11	
	;;
(p6)	fma.s1 f10 = f10, f12, f12
	;;
	fcvt.fx.trunc.s1 f10 = f10
	;;
	xma.l f10 = f10, f9, f13
	;;
	getf.sig ret0 = f10
	br.ret.sptk rp
	;;
	.endp __modsi3
#endif
