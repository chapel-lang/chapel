; RUN: opt --load libglobal-to-wide.so -global-to-wide -S < %s | FileCheck %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128-p100:64:64:64"

; Check wide pointer manipulation functions.
%struct.c_localeid_t = type { i32, i32 }

declare i64* @.gf.addr.1(i64 addrspace(100)*) readnone
declare i32 @.gf.node.1(i64 addrspace(100)*) readnone
declare %struct.c_localeid_t @.gf.loc.1(i64 addrspace(100)*) readnone
declare i64 addrspace(100)* @.gf.make.1(%struct.c_localeid_t, i64*) readnone

define i64* @testaddr(i64 addrspace(100)* %w) {
; CHECK: @testaddr(i64* %
; )
; CHECK-NOT: @.gf
; CHECK: wide_ptr_get_address
; CHECK-NOT: @.gf
; CHECK: ret i64*
entry:
  %ret = call i64* @.gf.addr.1(i64 addrspace(100)* %w)
  ret i64* %ret
}

define i32 @testnode(i64 addrspace(100)* %w) {
; CHECK: @testnode(i64* %
; )
; CHECK-NOT: @.gf
; CHECK: wide_ptr_get_node
; CHECK-NOT: @.gf
; CHECK: ret i32
entry:
  %ret = call i32 @.gf.node.1(i64 addrspace(100)* %w)
  ret i32 %ret
}

define %struct.c_localeid_t @testloc(i64 addrspace(100)* %w) {
; CHECK: @testloc(i64* %
; )
; CHECK-NOT: @.gf
; CHECK: wide_ptr_read_localeID
; CHECK-NOT: @.gf
; CHECK: ret %struct.c_localeid_t
entry:
  %ret = call %struct.c_localeid_t @.gf.loc.1(i64 addrspace(100)* %w)
  ret %struct.c_localeid_t %ret
}

define i64 addrspace(100)* @testmake(%struct.c_localeid_t %loc, i64* %addr) {
; CHECK: i64* @testmake(
; )
; CHECK-NOT: @.gf
; CHECK: return_wide_ptr_loc
; CHECK-NOT: @.gf
; CHECK: ret i64*
entry:
  %ret = call i64 addrspace(100)* @.gf.make.1(%struct.c_localeid_t %loc, i64* %addr)
  ret i64 addrspace(100)* %ret
}

