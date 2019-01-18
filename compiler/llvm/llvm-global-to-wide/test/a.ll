; RUN: opt --load %bindir/lib/llvm-pgas${MOD_EXT} -global-to-wide -S < %s | FileCheck %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128-p100:128:64:64-p101:128:64:64"

; Note after LLVM 7 memcpy will no longer take an alignment argument
declare void @llvm.memcpy.p0i8.p100i8.i64(i8* nocapture, i8 addrspace(100)* nocapture, i64, i32, i1)
declare void @llvm.memcpy.p100i8.p0i8.i64(i8 addrspace(100)* nocapture, i8* nocapture, i64, i32, i1)
declare void @llvm.memcpy.p100i8.p100i8.i64(i8 addrspace(100)* nocapture, i8 addrspace(100)* nocapture, i64, i32, i1)

define void @teststore(i64 addrspace(100)* %storeme) {
; CHECK: @teststore({ %struct.c_localeid_t, i64* } %
; )
; CHECK-NOT: @memcpy
; CHECK: put
; CHECK-NOT: @memcpy
; CHECK: ret
entry:
  %a = alloca i64
  store i64 7, i64 * %a
  %storeme.cast = bitcast i64 addrspace(100)* %storeme to i8 addrspace(100)*
  %a.cast = bitcast i64 * %a to i8 *
  call void @llvm.memcpy.p100i8.p0i8.i64(i8 addrspace(100)* %storeme.cast, i8* %a.cast, i64 8, i32 1, i1 true)
  ret void
}


define i64 @testload(i64 addrspace(100)* %loadme) {
; CHECK: @testload({ %struct.c_localeid_t, i64* } %
; )
; CHECK-NOT: @memcpy
; CHECK: get
; CHECK-NOT: @memcpy
; CHECK: ret
entry:
  %a = alloca i64
  %loadme.cast = bitcast i64 addrspace(100)* %loadme to i8 addrspace(100)*
  %a.cast = bitcast i64 * %a to i8 *
  call void @llvm.memcpy.p0i8.p100i8.i64(i8* %a.cast, i8 addrspace(100)* %loadme.cast, i64 8, i32 1, i1 true)
  %ret = load i64, i64 * %a
  ret i64 %ret
}

define void @testcopy(i64 addrspace(100)* %dst, i64 addrspace(100)* %src) {
; CHECK: @testcopy({ %struct.c_localeid_t, i64* } %
; )
; CHECK-NOT: @memcpy
; CHECK: getput
; CHECK-NOT: @memcpy
; CHECK: ret
entry:
  %dst.cast = bitcast i64 addrspace(100)* %dst to i8 addrspace(100)*
  %src.cast = bitcast i64 addrspace(100)* %src to i8 addrspace(100)*
  call void @llvm.memcpy.p100i8.p100i8.i64(i8 addrspace(100)* %dst.cast, i8 addrspace(100)* %src.cast, i64 8, i32 1, i1 true)
  ret void
}

define i64 @read_int(i64 addrspace(100)* %src) {
; CHECK: i64 @read_int({ %struct.c_localeid_t, i64* } %
; )
; CHECK-NOT: @.gf
; CHECK: get
; CHECK-NOT: @.gf
; CHECK: ret i64
entry:
  %ret = load i64, i64 addrspace(100)* %src
  ret i64 %ret
}

define void @write_int(i64 addrspace(100)* %dst, i64 %v) {
; CHECK: void @write_int({ %struct.c_localeid_t, i64* } %
; )
; CHECK-NOT: @.gf
; CHECK: put
; CHECK-NOT: @.gf
; CHECK: ret void
entry:
  store i64 %v, i64 addrspace(100)* %dst
  ret void
}

