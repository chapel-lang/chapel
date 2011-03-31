
/*
   IA-64 specific functions
*/

.text
.align 16
.globl switchStacks
.globl startHelper


/*
    void switchStacks (helper, arg0, arg1, new)

    As the context switch is donde throught a procedure call, no
    scratch register need to be saved. So a syncronous context switch
    must be performed without scratch registers.([1] 10.2 & table 10-1)
    [1] IA-64 Software Conventions an Runtime Architecture Guide, Intel
*/

saved_pfs	= r14
saved_rsc	= r15
saved_bsp	= r16
saved_unat	= r17
saved_rp	= r18
saved_rnat	= r19
saved_unat	= r20

ptr1		= r21
ptr2		= r22
ptr1b		= r23
temp1		= r24
temp2		= r25

old_sp		= r26
helper_addr	= r27


.proc switchStacks
switchStacks:
.prologue
.save ar.pfs, saved_pfs
	alloc saved_pfs = ar.pfs, 4, 3, 3, 0
	arg_a0 = in0
	arg_a1 = in1
	arg_newthread = in2
	arg_helper = in3
.body
	mov   saved_rsc = ar.rsc
	add   sp = -60*8,sp ;;
.prologue
	mov   saved_bsp = ar.bsp
.save ar.unat, saved_unat
	mov   saved_unat = ar.unat ;;
	flushrs
.save rp, saved_rp
	mov   saved_rp = rp
	mov   ar.rsc = 0 ;;
.body
// ***************************
// *** saving fp registers ***
// ***************************
	mov   ptr1 = sp
	add   ptr2 = 16,sp ;;
	stf.spill [ptr1] = f31,32
	stf.spill [ptr2] = f30,32 ;;
	stf.spill [ptr1] = f29,32
	stf.spill [ptr2] = f28,32 ;;
	stf.spill [ptr1] = f27,32
	stf.spill [ptr2] = f26,32 ;;
	stf.spill [ptr1] = f25,32
	stf.spill [ptr2] = f24,32 ;;
	stf.spill [ptr1] = f23,32
	stf.spill [ptr2] = f22,32 ;;
	stf.spill [ptr1] = f21,32
	stf.spill [ptr2] = f20,32 ;;
	stf.spill [ptr1] = f19,32
	stf.spill [ptr2] = f18,32 ;;
	stf.spill [ptr1] = f17,32
	stf.spill [ptr2] = f16,32 ;;
	stf.spill [ptr1] = f5,32
	stf.spill [ptr2] = f4,32 ;;
	stf.spill [ptr1] = f3,32
	stf.spill [ptr2] = f2,24 ;;
// ******************************
// *** saving fpsr and b5..b1 ***
// *** lc, pr                 ***
// ******************************
	mov temp1 = ar.fpsr
	mov temp2 = b5 ;;
	st8 [ptr1] = temp1,16
	st8 [ptr2] = temp2,16 ;;
	mov temp1 = b4
	mov temp2 = b3 ;;
	st8 [ptr1] = temp1,16
	st8 [ptr2] = temp2,16 ;;
	mov temp1 = b2
	mov temp2 = b1 ;;
	st8 [ptr1] = temp1,16
	st8 [ptr2] = temp2,16 ;;
	mov temp1 = ar.lc
	mov temp2 = pr ;;
	st8 [ptr1] = temp1,16
	st8 [ptr2] = temp2,16 ;;
// ***************************
// *** saving unat callee' ***
// *** r7, r6, r5, r4      ***
// *** saving unat caller  ***
// ***************************
	LOCAL_STACK_INDEX=0
	add  ptr1b = 16,ptr1

        .mem.offset 8,LOCAL_STACK_INDEX
	st8.spill [ptr2] = r7, 16 ;;
        .mem.offset 16,LOCAL_STACK_INDEX
	st8.spill [ptr1b] = r6,16 
        .mem.offset 24,LOCAL_STACK_INDEX
	st8.spill [ptr2] = r5, 16  ;;
        .mem.offset 32,LOCAL_STACK_INDEX
	st8.spill [ptr1b] = r4,16
	st8 [ptr2] = saved_unat,16 ;;
// ****************************
// *** saving tp, bsp, rnat ***
// *** pfs, rsc & rp        ***
// ****************************
	st8 [ptr1b] = r13,16
	st8 [ptr2] = saved_bsp,16 ;;
	mov saved_rnat = ar.rnat
	mov saved_pfs = ar.pfs ;;
	st8 [ptr1b] = saved_rnat,16
	st8 [ptr2] = saved_pfs,16 ;;
	st8 [ptr1b] = saved_rsc
	st8 [ptr2] = saved_rp ;;
// **************************
// *** saving unat callee ***
// *** ... deferred       ***
// **************************
	mov   saved_unat = ar.unat ;;
	st8 [ptr1] = saved_unat ;;
// ********************************
// *** change both stacks:      ***
// ***   - memory stack (sp)    ***
// ***   - register stack (bsp) ***
// ********************************
	mov old_sp = sp ;;
.body
	mov sp = arg_newthread
	invala ;;

	add r21 = 55*8, sp    // & bsp
	add r22 = 56*8, sp ;; // & rnat
.prologue
	ld8 r16 = [r21],16 ;;
	mov ar.bspstore = r16 ;;
// **************************************
	ld8 r19= [r22],16 ;;
	mov ar.rnat = r19 ;;
	ld8 arg_newthread = [r21],16    // reuse arg_newthread for pfs
	ld8 r15 = [r22],16 ;;
// **************************************
// *** switch on RSE (previous value) ***
// **************************************
	mov ar.rsc = r15 ;;
// **************************************
.body
	mov out0 = arg_a0 ;;
	mov out1 = arg_a1
	mov out2 = old_sp ;;

	mov arg_a0 = gp
	ld8 helper_addr = [arg_helper],8 ;;
	ld8 gp = [arg_helper]
	mov b6 = helper_addr ;;

	add sp = -16,sp ;;
	br.call.sptk.many b0 = b6 ;;
	mov gp = arg_a0
	add sp = 16,sp ;;

// ***************************
// *** restore new context ***
// ***************************

	add r21 = 40*8, sp ;;
	ld8 r24 = [r21] ;;
	mov ar.fpsr = r24 ;;

	mov ptr1 = sp
	add ptr2 = 16,sp ;;
	ldf.fill f31 = [ptr1],32
	ldf.fill f30 = [ptr2],32 ;;
	ldf.fill f29 = [ptr1],32
	ldf.fill f28 = [ptr2],32 ;;
	ldf.fill f27 = [ptr1],32
	ldf.fill f26 = [ptr2],32 ;;
	ldf.fill f25 = [ptr1],32
	ldf.fill f24 = [ptr2],32 ;;
	ldf.fill f23 = [ptr1],32
	ldf.fill f22 = [ptr2],32 ;;
	ldf.fill f21 = [ptr1],32
	ldf.fill f20 = [ptr2],32 ;;
	ldf.fill f19 = [ptr1],32
	ldf.fill f18 = [ptr2],32 ;;
	ldf.fill f17 = [ptr1],32
	ldf.fill f16 = [ptr2],32 ;;
	ldf.fill f5 = [ptr1],32
	ldf.fill f4 = [ptr2],32 ;;
	ldf.fill f3 = [ptr1],32
	ldf.fill f2 = [ptr2],24 ;;

	ld8 temp1 = [ptr1],16
	ld8 temp2 = [ptr2],16 ;;
//mov ar.fpsr = temp1
	mov b5 = temp2 ;;
	ld8 temp1 = [ptr1],16
	ld8 temp2 = [ptr2],16 ;;
	mov b4 = temp1
	mov b3 = temp2 ;;
	ld8 temp1 = [ptr1],16
	ld8 temp2 = [ptr2],16 ;;
	mov b2 = temp1
	mov b1 = temp2 ;;
	ld8 temp1 = [ptr1],16
	ld8 temp2 = [ptr2],16 ;;
	mov ar.lc = temp1
	mov pr = temp2,-1 ;;

	ld8 temp1 = [ptr1],16 ;;
	mov ar.unat = temp1 ;;
	ld8.fill r7 = [ptr2],16
	ld8.fill r6 = [ptr1],16 ;;
	ld8.fill r5 = [ptr2],16
	ld8.fill r4 = [ptr1],56 ;;
	ld8 temp1 = [ptr2] ;;
	ld8 r18 = [ptr1] ;;
	mov b0 = r18 ;;

	mov ar.pfs = arg_newthread
	add sp = 60*8,sp
	mov ar.unat = temp1 ;;
	
	br.ret.sptk.many b0
.endp   switchStacks

.proc	startHelper
startHelper:
	.prologue
 	.save ar.pfs, loc0
	alloc loc0 = ar.pfs,0,9,8,0

	mov r2=sp
	add r3=8,sp ;;

	.body
	ld8 loc1 = [r2],16
	ld8 loc2 = [r3],16
	;;
	ld8 loc3 = [r2],16
	ld8 loc4 = [r3],16
	;;
	ld8 loc5 = [r2],16
	ld8 loc6 = [r3],16
	;;
	ld8 loc7 = [r2]
	ld8 loc8 = [r3]
	mov sp = r2		// adjust sp with the 16-byte scratch space
	;;
	ld8  r10 = [r7],8
	;;
	mov  b6 = r10
	ld8  gp = [r7]
	mov  out0 = loc1
	mov  out1 = loc2
	mov  out2 = loc3
	mov  out3 = loc4
	mov  out4 = loc5
	mov  out5 = loc6
	mov  out6 = loc7
	mov  out7 = loc8

	br.call.sptk.many b0=b6

	ld8  r10 = [r5],8
	;;
	mov  b6 = r10
	ld8  gp = [r5]
	mov  out0 = r6		// cleanup arg
	mov  out1 = r8		// return from user's

	br.call.sptk.many b0=b6
.endp	startHelper
