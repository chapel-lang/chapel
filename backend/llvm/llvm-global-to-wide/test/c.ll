; RUN: opt --load %bindir/lib/llvm-pgas${MOD_EXT} -global-to-wide -S < %s | FileCheck %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128-p100:128:64:64-p101:128:64:64"

; Test type promotion
%struct.c_localeid_t = type { i32, i32 }


declare i64* @.gf.addr.1(i64 addrspace(100)*) readnone
declare i32 @.gf.node.1(i64 addrspace(100)*) readnone
declare %struct.c_localeid_t @.gf.loc.1(i64 addrspace(100)*) readnone
declare i64 addrspace(100)* @.gf.make.1(%struct.c_localeid_t, i64*) readnone

%mystruct = type { i64 addrspace(100)*, i64 addrspace(100)*, i32 *}
; CHECK: %mystruct = type { { %struct.c_localeid_t, i64* }, { %struct.c_localeid_t, i64* }, i32* }

declare %mystruct* @.gf.addr.2(%mystruct addrspace(100)*) readnone
declare i32 @.gf.node.2(%mystruct addrspace(100)*) readnone
declare %struct.c_localeid_t @.gf.loc.2(%mystruct addrspace(100)*) readnone
declare %mystruct addrspace(100)* @.gf.make.2(%struct.c_localeid_t, %mystruct*) readnone


define i64 addrspace(100)* @get_one(%mystruct addrspace(100)* %s) {
; CHECK: { %struct.c_localeid_t, i64* } @get_one({ %struct.c_localeid_t, %mystruct* } %
; )
; CHECK-NOT: @.gf
; CHECK: ret { %struct.c_localeid_t, i64* }
entry:
  %gep = getelementptr inbounds %mystruct, %mystruct addrspace(100)* %s, i32 0, i32 0
  %ptr = load i64 addrspace(100)*, i64 addrspace(100)* addrspace(100)* %gep
  ret i64 addrspace(100)* %ptr
}
define i64 addrspace(100)* @get_two(%mystruct addrspace(100)* %s) {
; CHECK: { %struct.c_localeid_t, i64* } @get_two({ %struct.c_localeid_t, %mystruct* }
; )
; CHECK-NOT: @.gf
; CHECK: ret { %struct.c_localeid_t, i64* }
entry:
  %gep = getelementptr inbounds %mystruct, %mystruct addrspace(100)* %s, i32 0, i32 1
  %ptr = load i64 addrspace(100)*, i64 addrspace(100)* addrspace(100)* %gep
  ret i64 addrspace(100)* %ptr
}
define i32* @get_three(%mystruct addrspace(100)* %s) {
; CHECK: i32* @get_three({ %struct.c_localeid_t, %mystruct* } %
; )
; CHECK-NOT: @.gf
; CHECK: ret i32*
entry:
  %gep = getelementptr inbounds %mystruct, %mystruct addrspace(100)* %s, i32 0, i32 2
  %ptr = load i32*, i32* addrspace(100)* %gep
  ret i32* %ptr
}

define i64 @read_int(%mystruct addrspace(100)* %s) {
; CHECK: i64 @read_int({ %struct.c_localeid_t, %mystruct* } %
; )
; CHECK-NOT: @.gf
; CHECK: get
; CHECK-NOT: @.gf
; CHECK: ret i64
entry:
  %gep = getelementptr inbounds %mystruct, %mystruct addrspace(100)* %s, i32 0, i32 0
  %ptr = load i64 addrspace(100)*, i64 addrspace(100)* addrspace(100)* %gep
  %ret = load i64, i64 addrspace(100)* %ptr
  ret i64 %ret
}

define void @write_int(%mystruct addrspace(100)* %s, i64 %v) {
; CHECK: void @write_int({ %struct.c_localeid_t, %mystruct* } %
; )
; CHECK-NOT: @.gf
; CHECK: put
; CHECK-NOT: @.gf
; CHECK: ret void
entry:
  %gep = getelementptr inbounds %mystruct, %mystruct addrspace(100)* %s, i32 0, i32 0
  %ptr = load i64 addrspace(100)*, i64 addrspace(100)* addrspace(100)* %gep
  store i64 %v, i64 addrspace(100)* %ptr
  ret void
}

