; RUN: opt --load-pass-plugin=%bindir/llvm-pgas.%soext -S --passes=global-to-wide < %s | FileCheck %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128-p100:128:64:64-p101:128:64:64"

define <2 x ptr addrspace(100)> @insert(<2 x ptr addrspace(100)> %vec, ptr addrspace(100) %x) {
; CHECK: <2 x i128> @insert(<2 x i128> %vec, { %struct.c_localeid_t, ptr } %x)
; CHECK: insertelement <2 x i128>
; CHECK-SAME: , i128
; CHECK-SAME: , i64 0
entry:
  %ret = insertelement <2 x ptr addrspace(100)> %vec, ptr addrspace(100) %x, i64 0
  ret <2 x ptr addrspace(100)> %ret
}

define ptr addrspace(100) @extract(<2 x ptr addrspace(100)> %vec) {
; CHECK: { %struct.c_localeid_t, ptr } @extract(<2 x i128> %vec)
; CHECK: extractelement <2 x i128>
; CHECK-SAME: , i64 1
; CHECK: ret { %struct.c_localeid_t, ptr }
entry:
  %ret = extractelement <2 x ptr addrspace(100)> %vec, i64 1
  ret ptr addrspace(100) %ret
}

define <2 x ptr addrspace(100)> @shuffle(<2 x ptr addrspace(100)> %vec1, <2 x ptr addrspace(100)> %vec2) {
; CHECK: <2 x i128> @shuffle(<2 x i128> %vec1, <2 x i128> %vec2)
; CHECK: %ret = shufflevector <2 x i128> %vec1, <2 x i128> %vec2, <2 x i32> zeroinitializer
; CHECK: ret <2 x i128> %ret
entry:
  %ret = shufflevector <2 x ptr addrspace(100)> %vec1, <2 x ptr addrspace(100)> %vec2, <2 x i32> zeroinitializer
  ret <2 x ptr addrspace(100)> %ret
}
