
	.section	".text"
	.align 2
	.p2align 4,,15
	.globl switchStacks
	.type	switchStacks, @function
	
switchStacks:
	stwu %r1,-240(%r1)
	mfcr %r12
	mflr %r0
	stfd %f14,96(%r1)
	stfd %f15,104(%r1)
	mtctr %r6
	mr %r8,%r5
	stfd %f16,112(%r1)
	stfd %f17,120(%r1)
	stfd %f18,128(%r1)
	stfd %f19,136(%r1)
	stfd %f20,144(%r1)
	stfd %f21,152(%r1)
	stfd %f22,160(%r1)
	stfd %f23,168(%r1)
	stfd %f24,176(%r1)
	stfd %f25,184(%r1)
	stfd %f26,192(%r1)
	stfd %f27,200(%r1)
	stfd %f28,208(%r1)
	stfd %f29,216(%r1)
	stfd %f30,224(%r1)
	stfd %f31,232(%r1)
	stw %r14,24(%r1)
	stw %r15,28(%r1)
	stw %r16,32(%r1)
	stw %r17,36(%r1)
	stw %r18,40(%r1)
	stw %r19,44(%r1)
	mr %r5,%r1
	stw %r20,48(%r1)
	stw %r21,52(%r1)
	stw %r22,56(%r1)
	stw %r23,60(%r1)
	stw %r24,64(%r1)
	stw %r25,68(%r1)
	stw %r26,72(%r1)
	stw %r27,76(%r1)
	stw %r28,80(%r1)
	stw %r29,84(%r1)
	stw %r30,88(%r1)
	stw %r31,92(%r1)
	stw %r0,244(%r1)
	stw %r12,20(%r1)
	mr %r1,%r8
	bctrl
	lwz %r12,20(%r1)
	lwz %r0,244(%r1)
	lwz %r14,24(%r1)
	lwz %r15,28(%r1)
	lwz %r16,32(%r1)
	lwz %r17,36(%r1)
	mtlr %r0
	lwz %r18,40(%r1)
	lwz %r19,44(%r1)
	mtcrf 32,%r12
	lwz %r20,48(%r1)
	lwz %r21,52(%r1)
	mtcrf 16,%r12
	lwz %r22,56(%r1)
	lwz %r23,60(%r1)
	mtcrf 8,%r12
	lwz %r24,64(%r1)
	lwz %r25,68(%r1)
	lwz %r26,72(%r1)
	lwz %r27,76(%r1)
	lwz %r28,80(%r1)
	lwz %r29,84(%r1)
	lwz %r30,88(%r1)
	lwz %r31,92(%r1)
	lfd %f14,96(%r1)
	lfd %f15,104(%r1)
	lfd %f16,112(%r1)
	lfd %f17,120(%r1)
	lfd %f18,128(%r1)
	lfd %f19,136(%r1)
	lfd %f20,144(%r1)
	lfd %f21,152(%r1)
	lfd %f22,160(%r1)
	lfd %f23,168(%r1)
	lfd %f24,176(%r1)
	lfd %f25,184(%r1)
	lfd %f26,192(%r1)
	lfd %f27,200(%r1)
	lfd %f28,208(%r1)
	lfd %f29,216(%r1)
	lfd %f30,224(%r1)
	lfd %f31,232(%r1)
	addi %r1,%r1,240
	blr
	.size	switchStacks,.-switchStacks

# (r14) userf
# (r15) data
# (r16) cleanup
# (r17) cleanup arg
	.globl startHelper
	.type	startHelper, @function

startHelper:
	mtctr %r14
	mr %r3,%r15
	bctrl
	mtlr %r16
	mr %r3,%r17
	blr
       .size  startHelper,.-startHelper
