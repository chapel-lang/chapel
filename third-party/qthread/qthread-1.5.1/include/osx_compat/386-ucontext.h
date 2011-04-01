#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#ifdef HAVE_STDARG_H
# include <stdarg.h>
#endif
#ifdef HAVE_SYS_UCONTEXT_H
# include <sys/ucontext.h>
#endif

#define setcontext(u) setmcontext(&(u)->uc_mcontext)
#define getcontext(u) getmcontext(&(u)->uc_mcontext)
typedef struct mcontext mcontext_t;
typedef struct ucontext ucontext_t;

typedef void (MakeContextCallback)(void);

int swapcontext(ucontext_t *, ucontext_t *);
void makecontext(ucontext_t *, MakeContextCallback *, int, ...);
int getmcontext(mcontext_t *);
void setmcontext(mcontext_t *);

/*-
 * Copyright (c) 1999 Marcel Moolenaar
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/sys/ucontext.h,v 1.4 1999/10/11 20:33:17 luoqi Exp $
 */

struct mcontext {
	unsigned long mc_edi;  /* 0: 1st arg (mandatory) */
	unsigned long mc_ebp;  /* 1: Stack frame pointer (esi) */
	unsigned long mc_ebx;  /* 2: PIC base register, also general-purp. reg */
#ifdef __x86_64__
	unsigned long mc_r12;  /* 3: extra callee-saved registers */
	unsigned long mc_r13;  /* 4: extra callee-saved registers */
	unsigned long mc_r14;  /* 5: extra callee-saved registers */
	unsigned long mc_r15;  /* 6: extra callee-saved registers */
	unsigned long mc_xcsr; /* 7: SSE2 control and status word */
#else
	unsigned long mc_esi;  /* 3: general-purpose register */
#endif
	unsigned long mc_cw;   /* 8/4: x87 control word */
	unsigned long mc_esp;  /* 9/5: machine state; stack pointer */
	unsigned long mc_eip;  /* 10/6: function pointer */
};

struct ucontext {
	mcontext_t	uc_mcontext;
	struct {
	    void * ss_sp;
	    size_t ss_size;
	    int ss_flags;
	} uc_stack;
};


