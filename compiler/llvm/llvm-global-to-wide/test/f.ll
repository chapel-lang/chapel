; RUN: opt --load-pass-plugin=%bindir/llvm-pgas.%soext -S --passes=global-to-wide < %s | FileCheck %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128-p100:128:64:64-p101:128:64:64"

; try a phi node
%struct.c_localeid_t = type { i32, i32 }


declare ptr @.gf.addr.1(ptr addrspace(100)) readnone
declare i32 @.gf.node.1(ptr addrspace(100)) readnone
declare %struct.c_localeid_t @.gf.loc.1(ptr addrspace(100)) readnone
declare ptr addrspace(100) @.gf.make.1(%struct.c_localeid_t, ptr) readnone

; mystruct uses opaque pointers but with typed pointers it would be:
; %mystruct = type { i64 addrspace(100)*, i64 addrspace(100)*, i32 *}

%mystruct = type { ptr addrspace(100), ptr addrspace(100), ptr }
; CHECK: %mystruct = type { { %struct.c_localeid_t, ptr }, { %struct.c_localeid_t, ptr }, ptr }

declare ptr @.gf.addr.2(ptr addrspace(100)) readnone
declare i32 @.gf.node.2(ptr addrspace(100)) readnone
declare %struct.c_localeid_t @.gf.loc.2(ptr addrspace(100)) readnone
declare ptr addrspace(100) @.gf.make.2(%struct.c_localeid_t, ptr) readnone

; A function to keep those types from dissapearing
define void @test(%mystruct %a) {
; CHECK: void @test(%mystruct %a)
entry:
  ret void
}

define void @F() {
entry:
  br label %F_cond
F_cond:
  %tmp = phi ptr addrspace(100) [ undef, %entry ], [ undef, %F_body_five ]
  br i1 undef, label %F_body_three, label %F_end
F_body_three:
  br i1 undef, label %F_body_five, label %F_body_six
F_body_five:
  br label %F_cond
F_body_six:
  unreachable
F_end:
  unreachable
}
