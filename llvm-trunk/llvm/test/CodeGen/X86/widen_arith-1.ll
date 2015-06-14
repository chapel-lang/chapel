; RUN: llc < %s -mcpu=generic -march=x86 -mattr=+sse4.2 |  FileCheck %s

define void @update(<3 x i8>* %dst, <3 x i8>* %src, i32 %n) nounwind {
entry:
; CHECK-NOT: pextrw
; CHECK: add

	%dst.addr = alloca <3 x i8>*		; <<3 x i8>**> [#uses=2]
	%src.addr = alloca <3 x i8>*		; <<3 x i8>**> [#uses=2]
	%n.addr = alloca i32		; <i32*> [#uses=2]
	%i = alloca i32, align 4		; <i32*> [#uses=6]
	store <3 x i8>* %dst, <3 x i8>** %dst.addr
	store <3 x i8>* %src, <3 x i8>** %src.addr
	store i32 %n, i32* %n.addr
	store i32 0, i32* %i
	br label %forcond

forcond:		; preds = %forinc, %entry
	%tmp = load i32, i32* %i		; <i32> [#uses=1]
	%tmp1 = load i32, i32* %n.addr		; <i32> [#uses=1]
	%cmp = icmp slt i32 %tmp, %tmp1		; <i1> [#uses=1]
	br i1 %cmp, label %forbody, label %afterfor

forbody:		; preds = %forcond
	%tmp2 = load i32, i32* %i		; <i32> [#uses=1]
	%tmp3 = load <3 x i8>*, <3 x i8>** %dst.addr		; <<3 x i8>*> [#uses=1]
	%arrayidx = getelementptr <3 x i8>, <3 x i8>* %tmp3, i32 %tmp2		; <<3 x i8>*> [#uses=1]
	%tmp4 = load i32, i32* %i		; <i32> [#uses=1]
	%tmp5 = load <3 x i8>*, <3 x i8>** %src.addr		; <<3 x i8>*> [#uses=1]
	%arrayidx6 = getelementptr <3 x i8>, <3 x i8>* %tmp5, i32 %tmp4		; <<3 x i8>*> [#uses=1]
	%tmp7 = load <3 x i8>, <3 x i8>* %arrayidx6		; <<3 x i8>> [#uses=1]
	%add = add <3 x i8> %tmp7, < i8 1, i8 1, i8 1 >		; <<3 x i8>> [#uses=1]
	store <3 x i8> %add, <3 x i8>* %arrayidx
	br label %forinc

forinc:		; preds = %forbody
	%tmp8 = load i32, i32* %i		; <i32> [#uses=1]
	%inc = add i32 %tmp8, 1		; <i32> [#uses=1]
	store i32 %inc, i32* %i
	br label %forcond

afterfor:		; preds = %forcond
	ret void
}
