; $Id: amem.s,v 1.1 1996/09/23 23:43:10 pvmsrc Exp $
; $CHeader: amem.s 1.3 1995/08/30 08:47:06 $
; Copyright 1992 Convex Computer Corp.
;	int __ldcws  (void *addr)
;	int __read32 (void *addr)
;	int __synch  ()
;
;	Note: addr must be on a 16 byte boundary.

        .space  $TEXT$,sort=8
        .subspa $CODE$,quad=0,align=8,access=44,code_only,sort=24
        .export __ldcws32,entry,priv_lev=3,argw0=gr,rtnval=gr
__ldcws32
        .proc
        .callinfo caller,frame=0
        .entry
	sync
	ldcws	0(0,%arg0),%ret0
	nop
	bv	%r0(%rp)
	.exit
	nop
        .procend

        .space  $TEXT$
        .subspa $CODE$
        .export __read32,entry,priv_lev=3,argw0=gr,rtnval=gr
__read32
        .proc
        .callinfo caller,frame=0
        .entry
	bv	%r0(%rp)
	.exit
	ldws	0(0,%arg0),%ret0
        .procend

        .space  $TEXT$
        .subspa $CODE$
        .export __synch,entry,priv_lev=3,argw0=gr,rtnval=gr
__synch
        .proc
        .callinfo caller,frame=0
        .entry
        bv      %r0(%rp)
        .exit
        sync
        .procend
        .end
