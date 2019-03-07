; RUN: opt --load %bindir/lib/llvm-pgas${MOD_EXT}  -global-to-wide -S < %s | FileCheck %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128-p100:128:64:64-p101:128:64:64"

; try a phi node
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

; A function to keep those types from dissapearing
define void @test(%mystruct %a) {
; CHECK: void @test(%mystruct %a)
entry:
  ret void
}

define void @D(i1 %a, i1 %b) {
entry:
  br i1 %a, label %D_end, label %D_rhs
D_rhs:
  br label %D_end
D_end:
  %tmp = phi i1 [ true, %entry ], [ %b, %D_rhs ]
  ret void
}


define void @E(i1 %a, i32 %n) {
entry:
  br label %E_loop
E_loop:
  %in = phi i32 [ %n, %entry ], [ %sum, %E_loop ]
  %sum = add i32 4, %in
  br i1 %a, label %E_loop, label %E_end
E_end:
  ret void
}



define void @G(i1 %a, %mystruct addrspace(100)* %s) {
entry:
  br label %G_loop
G_loop:
  %in = phi %mystruct addrspace(100)* [ %s, %entry ], [ %ptr, %G_loop ]
  %ptr = getelementptr inbounds %mystruct, %mystruct addrspace(100)* %in, i32 1
  br i1 %a, label %G_loop, label %G_end
G_end:
  ret void
}


