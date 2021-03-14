; RUN: opt --load %bindir/lib/llvm-pgas${MOD_EXT} -global-to-wide -S < %s | FileCheck %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128-p100:128:64:64-p101:128:64:64"

; Note after LLVM 7 memcpy will no longer take an alignment argument
declare void @llvm.memcpy.p0i8.p100i8.i64(i8* nocapture, i8 addrspace(100)* nocapture, i64, i32, i1)
declare void @llvm.memcpy.p100i8.p0i8.i64(i8 addrspace(100)* nocapture, i8* nocapture, i64, i32, i1)
declare void @llvm.memcpy.p100i8.p100i8.i64(i8 addrspace(100)* nocapture, i8 addrspace(100)* nocapture, i64, i32, i1)

define i128 @testptrtoint(double addrspace(100)* %ptr) {
; CHECK: i128 @testptrtoint(
; CHECK: { %struct.c_localeid_t, double* } %
; )
; CHECK: alloca i128
; CHECK: bitcast i128* %
; CHECK: to { %struct.c_localeid_t, double* }*
; CHECK: store { %struct.c_localeid_t, double* } %ptr
; CHECK: load i128, i128* %
; CHECK: ret i128
entry:
  %ret = ptrtoint double addrspace(100)* %ptr to i128
  ret i128 %ret
}


define double addrspace(100)* @testinttoptr(i128 %i) {
; CHECK: { %struct.c_localeid_t, double* } @testinttoptr(i128 %
; )
; CHECK: alloca { %struct.c_localeid_t, double* }
; CHECK: bitcast { %struct.c_localeid_t, double* }* %
; CHECK: to i128*
; CHECK: store i128 %
; CHECK: load { %struct.c_localeid_t, double* }, { %struct.c_localeid_t, double* }*
; CHECK: ret { %struct.c_localeid_t, double* }
entry:
  %ret = inttoptr i128 %i to double addrspace(100)*
  ret double addrspace(100)* %ret
}
