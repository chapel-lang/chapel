; RUN: opt --load %bindir/lib/llvm-pgas${MOD_EXT} -aggregate-global-ops -S < %s | FileCheck %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128-p100:128:64:64-p101:128:64:64"

; Note after LLVM 7 memcpy will no longer take an alignment argument
declare void @llvm.memcpy.p0i8.p100i8.i64(i8* nocapture, i8 addrspace(100)* nocapture, i64, i32, i1)
declare void @llvm.memcpy.p100i8.p0i8.i64(i8 addrspace(100)* nocapture, i8* nocapture, i64, i32, i1)
declare void @llvm.memcpy.p100i8.p100i8.i64(i8 addrspace(100)* nocapture, i8 addrspace(100)* nocapture, i64, i32, i1)


define void @teststore1(i64 addrspace(100)* %base) {
; CHECK: @teststore1(
; )
entry:
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 0
  %p1 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 1
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 2
; CHECK: store i64 1
; CHECK: store i64 2
; CHECK: store i64 3
; CHECK: memcpy
; CHECK: ret
  store i64 1, i64 addrspace(100)* %p0, align 8
  store i64 2, i64 addrspace(100)* %p1, align 8
  store i64 3, i64 addrspace(100)* %p2, align 8
  ret void
}

define void @teststore2(i64 addrspace(100)* %base) {
; CHECK: @teststore2(
; )
entry:
; CHECK: store i64 1
; CHECK: store i64 2
; CHECK: store i64 3
; CHECK: memcpy
; CHECK: ret
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 0
  store i64 1, i64 addrspace(100)* %p0, align 8
  %p1 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 1
  store i64 2, i64 addrspace(100)* %p1, align 8
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 2
  store i64 3, i64 addrspace(100)* %p2, align 8
  ret void
}

define void @teststore3(i64 addrspace(100)* %base) {
; CHECK: @teststore3(
; )
entry:
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 2
  %p1 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 1
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 0
; CHECK: store i64 3
; CHECK: store i64 2
; CHECK: store i64 1
; CHECK: memcpy
; CHECK: ret
  store i64 3, i64 addrspace(100)* %p2, align 8
  store i64 2, i64 addrspace(100)* %p1, align 8
  store i64 1, i64 addrspace(100)* %p0, align 8
  ret void
}

define void @teststore4(i64 addrspace(100)* %base) {
; CHECK: @teststore4(
; )
entry:
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 2
  %p01 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 0
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 0
; CHECK: store i64 3
; CHECK: store i64 2
; CHECK: store i64 1
; CHECK: memcpy
; CHECK: ret
  store i64 3, i64 addrspace(100)* %p2, align 8
  store i64 2, i64 addrspace(100)* %p01, align 8
  store i64 1, i64 addrspace(100)* %p0, align 8
  ret void
}


define i64 @testload1(i64 addrspace(100)* %base) {
; CHECK: @testload1(
; )
; CHECK: memcpy
; CHECK: load
; CHECK: load
; CHECK: load
; CHECK: ret
entry:
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 0
  %p1 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 1
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 2
  %v1 = load i64, i64 addrspace(100)* %p0, align 8
  %v2 = load i64, i64 addrspace(100)* %p1, align 8
  %v3 = load i64, i64 addrspace(100)* %p2, align 8
  %sum1 = add i64 %v1, %v2
  %sum2 = add i64 %sum1, %v3
  ret i64 %sum2
}

define i64 @testload2(i64 addrspace(100)* %base) {
; CHECK: @testload2(
; )
; CHECK: memcpy
; CHECK: load
; CHECK: load
; CHECK: load
; CHECK: ret
entry:
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 0
  %v1 = load i64, i64 addrspace(100)* %p0, align 8
  %p1 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 1
  %v2 = load i64, i64 addrspace(100)* %p1, align 8
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 2
  %v3 = load i64, i64 addrspace(100)* %p2, align 8
  %sum1 = add i64 %v1, %v2
  %sum2 = add i64 %sum1, %v3
  ret i64 %sum2
}

define i64 @testload3(i64 addrspace(100)* %base) {
; CHECK: @testload3(
; )
; CHECK: memcpy
; CHECK: load
; CHECK: load
; CHECK: load
; CHECK: ret
entry:
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 0
  %v1 = load i64, i64 addrspace(100)* %p0, align 8
  %p1 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 4
  %v2 = load i64, i64 addrspace(100)* %p1, align 8
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 8
  %v3 = load i64, i64 addrspace(100)* %p2, align 8
  %sum1 = add i64 %v1, %v2
  %sum2 = add i64 %sum1, %v3
  ret i64 %sum2
}

define i64 @testload4(i64 addrspace(100)* %base) {
; CHECK: @testload4(
; )
; CHECK: memcpy
; CHECK: load
; CHECK: load
; CHECK: load
; CHECK: ret
entry:
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 0
  %v1 = load i64, i64 addrspace(100)* %p0, align 8
  %p1 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 4
  %v2 = load i64, i64 addrspace(100)* %p1, align 8
  %sum1 = add i64 %v1, %v2
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 8
  %v3 = load i64, i64 addrspace(100)* %p2, align 8
  %sum2 = add i64 %sum1, %v3
  ret i64 %sum2
}

define i64 @testload5(i64 addrspace(100)* %base) {
; CHECK: @testload5(
; )
; CHECK: memcpy
; CHECK: load
; CHECK: load
; CHECK: load
; CHECK: ret
entry:
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 3
  %v1 = load i64, i64 addrspace(100)* %p0, align 8
  %p1 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 1
  %v2 = load i64, i64 addrspace(100)* %p1, align 8
  %sum1 = add i64 %v1, %v2
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 2
  %v3 = load i64, i64 addrspace(100)* %p2, align 8
  %sum2 = add i64 %sum1, %v3
  ret i64 %sum2
}

define i64 @testload6(i64 addrspace(100)* %base, i64 addrspace(100)* %other) {
; CHECK: @testload6(
; )
; CHECK: getelementptr
; CHECK: load
; CHECK: getelementptr
; CHECK: load
; CHECK: add
; CHECK: getelementptr
; CHECK: load
; CHECK: add
; CHECK: ret
entry:
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 3
  %v1 = load i64, i64 addrspace(100)* %p0, align 8
  %o1 = getelementptr inbounds i64, i64 addrspace(100)* %other, i32 1
  %vo = load i64, i64 addrspace(100)* %o1, align 8
  %sum1 = add i64 %v1, %vo
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 2
  %v3 = load i64, i64 addrspace(100)* %p2, align 8
  %sum2 = add i64 %sum1, %v3
  ret i64 %sum2
}

define i64 @testload7(i64 addrspace(100)* %base, i64 addrspace(100)* %other) {
; CHECK: @testload7(
; )
; CHECK: getelementptr
; CHECK: getelementptr
; CHECK: getelementptr
; CHECK: memcpy
; CHECK: load
; CHECK: load
; CHECK: add
; CHECK: load
; CHECK: add
; CHECK: ret
entry:
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 3
  %v1 = load i64, i64 addrspace(100)* %p0, align 8
  %sum0 = add i64 %v1, %v1
  %o1 = getelementptr inbounds i64, i64 addrspace(100)* %other, i32 1
  %p1 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 1
  %v2 = load i64, i64 addrspace(100)* %p1, align 8
  %vo = load i64, i64 addrspace(100)* %o1, align 8
  %sum1 = add i64 %sum0, %vo
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 2
  %v3 = load i64, i64 addrspace(100)* %p2, align 8
  %sum2 = add i64 %sum1, %v3
  ret i64 %sum2
}



define void @teststoreatomic(i64 addrspace(100)* %base) {
; CHECK: @teststoreatomic(
; )
entry:
  %p0 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 0
  %p1 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 1
  %p2 = getelementptr inbounds i64, i64 addrspace(100)* %base, i32 2
; CHECK: store atomic i64 1
; CHECK: store atomic i64 2
; CHECK: store atomic i64 3
; CHECK: ret
  store atomic i64 1, i64 addrspace(100)* %p0 unordered, align 8
  store atomic i64 2, i64 addrspace(100)* %p1 unordered, align 8
  store atomic i64 3, i64 addrspace(100)* %p2 unordered, align 8
  ret void
}


