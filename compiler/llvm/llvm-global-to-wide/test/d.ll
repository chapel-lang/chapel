; RUN: opt --load-pass-plugin=%bindir/llvm-pgas.%soext -S --passes=global-to-wide < %s | FileCheck %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128-p100:128:64:64-p101:128:64:64"

; Test recursive type promotion
%struct.c_localeid_t = type { i32, i32 }

;%recurs = type opaque
;%type_one = type opaque
;%type_two = type opaque

; these types are defined with opaque pointers but with typed ptrs they are:
; %recurs = type { %recurs addrspace(100)* }
; %type_one = type { %type_two addrspace(100)* }
; %type_two = type { %type_one addrspace(100)* }

%recurs = type { ptr addrspace(100) }
; CHECK: %recurs = type { { %struct.c_localeid_t, ptr } }
%type_one = type { ptr addrspace(100) }
; CHECK: %type_one = type { { %struct.c_localeid_t, ptr } }
%type_two = type { ptr addrspace(100) }
; CHECK: %type_two = type { { %struct.c_localeid_t, ptr } }

; A function to keep those types from dissapearing
define void @test(%recurs %a, %type_one %b, %type_two %c) {
; CHECK: void @test(%recurs %a, %type_one %b, %type_two %c)
entry:
  ret void
}

