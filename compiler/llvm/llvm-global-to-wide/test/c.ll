; RUN: opt --load-pass-plugin=%bindir/llvm-pgas.%soext -S --passes=global-to-wide < %s | FileCheck %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128-p100:128:64:64-p101:128:64:64"

; Test type promotion
%struct.c_localeid_t = type { i32, i32 }


declare ptr @.gf.addr.1(ptr addrspace(100)) readnone
declare i32 @.gf.node.1(ptr addrspace(100)) readnone
declare %struct.c_localeid_t @.gf.loc.1(ptr addrspace(100)) readnone
declare ptr addrspace(100) @.gf.make.1(%struct.c_localeid_t, ptr) readnone

; mystruct has opaque pointers but with typed pointers it is
; %mystruct = type { i64 addrspace(100)*, i64 addrspace(100)*, i32 *}

%mystruct = type { ptr addrspace(100), ptr addrspace(100), ptr }
; CHECK: %mystruct = type { { %struct.c_localeid_t, ptr }, { %struct.c_localeid_t, ptr }, ptr }

declare ptr @.gf.addr.2(ptr addrspace(100)) readnone
declare i32 @.gf.node.2(ptr addrspace(100)) readnone
declare %struct.c_localeid_t @.gf.loc.2(ptr addrspace(100)) readnone
declare ptr addrspace(100) @.gf.make.2(%struct.c_localeid_t, ptr) readnone


define ptr addrspace(100) @get_one(ptr addrspace(100) %s) {
; CHECK: { %struct.c_localeid_t, ptr } @get_one({ %struct.c_localeid_t, ptr } %
; )
; CHECK-NOT: @.gf
; CHECK: ret { %struct.c_localeid_t, ptr }
entry:
  %gep = getelementptr inbounds %mystruct, ptr addrspace(100) %s, i32 0, i32 0
  %ptr = load ptr addrspace(100), ptr addrspace(100) %gep
  ret ptr addrspace(100) %ptr
}
define ptr addrspace(100) @get_two(ptr addrspace(100) %s) {
; CHECK: { %struct.c_localeid_t, ptr } @get_two({ %struct.c_localeid_t, ptr }
; )
; CHECK-NOT: @.gf
; CHECK: ret { %struct.c_localeid_t, ptr }
entry:
  %gep = getelementptr inbounds %mystruct, ptr addrspace(100) %s, i32 0, i32 1
  %ptr = load ptr addrspace(100), ptr addrspace(100) %gep
  ret ptr addrspace(100) %ptr
}
define ptr @get_three(ptr addrspace(100) %s) {
; CHECK: ptr @get_three({ %struct.c_localeid_t, ptr } %
; )
; CHECK-NOT: @.gf
; CHECK: ret ptr
entry:
  %gep = getelementptr inbounds %mystruct, ptr addrspace(100) %s, i32 0, i32 2
  %ptr = load i32*, ptr addrspace(100) %gep
  ret ptr %ptr
}

define i64 @read_int(ptr addrspace(100) %s) {
; CHECK: i64 @read_int({ %struct.c_localeid_t, ptr } %
; )
; CHECK-NOT: @.gf
; CHECK: get
; CHECK-NOT: @.gf
; CHECK: ret i64
entry:
  %gep = getelementptr inbounds %mystruct, ptr addrspace(100) %s, i32 0, i32 0
  %ptr = load ptr addrspace(100), ptr addrspace(100) %gep
  %ret = load i64, ptr addrspace(100) %ptr
  ret i64 %ret
}

define void @write_int(ptr addrspace(100) %s, i64 %v) {
; CHECK: void @write_int({ %struct.c_localeid_t, ptr } %
; )
; CHECK-NOT: @.gf
; CHECK: put
; CHECK-NOT: @.gf
; CHECK: ret void
entry:
  %gep = getelementptr inbounds %mystruct, ptr addrspace(100) %s, i32 0, i32 0
  %ptr = load ptr addrspace(100), ptr addrspace(100) %gep
  store i64 %v, ptr addrspace(100) %ptr
  ret void
}
