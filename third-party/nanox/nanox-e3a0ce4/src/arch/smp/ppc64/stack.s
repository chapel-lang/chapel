	.section	".toc","aw"
	.section	".text"

	.align 2
	.p2align 4,,15
	.globl switchStacks
	.section	".opd","aw"
	.align 3

switchStacks:
	.quad	.L.switchStacks,.TOC.@tocbase,0
	.previous
	.type	switchStacks, @function
	
.L.switchStacks:
	mflr %r0
	std %r0,16(%r1)
	std %r31,-8(%r1)
	stdu %r1,-368(%r1)
	mr %r31,%r1

   # save registers
   stfd %f14,216(%r1)
   stfd %f15,224(%r1)
   stfd %f16,232(%r1)
   stfd %f17,240(%r1)
   stfd %f18,248(%r1)
   stfd %f19,256(%r1)
   stfd %f20,264(%r1)
   stfd %f21,272(%r1)
   stfd %f22,280(%r1)
   stfd %f23,288(%r1)
   stfd %f24,296(%r1)
   stfd %f25,304(%r1)
   stfd %f26,312(%r1)
   stfd %f27,320(%r1)
   stfd %f28,328(%r1)
   stfd %f29,336(%r1)
   stfd %f30,344(%r1)
   stfd %f31,352(%r1)
   std %r14,72(%r1)
   std %r15,80(%r1)
   std %r16,88(%r1)
   std %r17,96(%r1)
   std %r18,104(%r1)
   std %r19,112(%r1)
   std %r20,120(%r1)
   std %r21,128(%r1)
   std %r22,136(%r1)
   std %r23,144(%r1)
   std %r24,152(%r1)
   std %r25,160(%r1)
   std %r26,168(%r1)
   std %r27,176(%r1)
   std %r28,184(%r1)
   std %r29,192(%r1)
   std %r30,200(%r1)
   std %r31,208(%r1)

   # switch stacks
   mr %r8,%r5
   mr %r5,%r1
   mr %r1,%r8

   # execute helper
	ld %r9,0(%r6)
	std %r2,40(%r1)
	ld %r11,16(%r6)
	mtctr %r9
	ld %r2,8(%r6)
	bctrl
	ld %r2,40(%r1)

   # restore registers
   ld %r14,72(%r1)
   ld %r15,80(%r1)
   ld %r16,88(%r1)
   ld %r17,96(%r1)
   ld %r18,104(%r1)
   ld %r19,112(%r1)
   ld %r20,120(%r1)
   ld %r21,128(%r1)
   ld %r22,136(%r1)
   ld %r23,144(%r1)
   ld %r24,152(%r1)
   ld %r25,160(%r1)
   ld %r26,168(%r1)
   ld %r27,176(%r1)
   ld %r28,184(%r1)
   ld %r29,192(%r1)
   ld %r30,200(%r1)
   ld %r31,208(%r1)
   lfd %f14,216(%r1)
   lfd %f15,224(%r1)
   lfd %f16,232(%r1)
   lfd %f17,240(%r1)
   lfd %f18,248(%r1)
   lfd %f19,256(%r1)
   lfd %f20,264(%r1)
   lfd %f21,272(%r1)
   lfd %f22,280(%r1)
   lfd %f23,288(%r1)
   lfd %f24,296(%r1)
   lfd %f25,304(%r1)
   lfd %f26,312(%r1)
   lfd %f27,320(%r1)
   lfd %f28,328(%r1)
   lfd %f29,336(%r1)
   lfd %f30,344(%r1)
   lfd %f31,352(%r1)

	addi %r1,%r1,368
	ld %r0,16(%r1)
	mtlr %r0
	ld %r31,-8(%r1)
	blr
	.long 0
	.byte 0,9,0,1,128,1,0,1
	.size	switchStacks,.-.L.switchStacks

# (r14) userf
# (r15) data
# (r16) cleanup
# (r17) cleanup arg
	.section	".toc","aw"
	.section	".text"
	.align 2
	.p2align 4,,15
	.globl startHelper
	.section	".opd","aw"
	.align 3

startHelper:
	.quad	.L.startHelper,.TOC.@tocbase,0
	.previous
	.type	startHelper, @function
.L.startHelper:
   # execute userFunction
	std %r2,40(%r1)
   ld %r0,0(%r14)
	mtctr %r0
	ld %r11,16(%r14)
   ld %r2,8(%r14)
	mr %r3,%r15
	bctrl

   # execute cleanup
   ld %r0,0(%r16)
	mtctr %r0
	ld %r11,16(%r16)
   ld %r2,8(%r16)
	mr %r3,%r17
	bctrl

   # this is probably not executed ever
	ld %r2,40(%r1)
	mr %r3,%r17
	blr
  .size  startHelper,.-.L.startHelper
