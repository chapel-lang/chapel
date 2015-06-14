; RUN: opt -objc-arc-contract -S < %s | FileCheck %s

target datalayout = "e-p:64:64:64"

declare i8* @objc_retain(i8*)
declare void @objc_release(i8*)
declare void @use_pointer(i8*)

@x = external global i8*

; CHECK-LABEL: define void @test0(
; CHECK: entry:
; CHECK-NEXT: tail call void @objc_storeStrong(i8** @x, i8* %p) [[NUW:#[0-9]+]]
; CHECK-NEXT: ret void
; CHECK-NEXT: }
define void @test0(i8* %p) {
entry:
  %0 = tail call i8* @objc_retain(i8* %p) nounwind
  %tmp = load i8*, i8** @x, align 8
  store i8* %0, i8** @x, align 8
  tail call void @objc_release(i8* %tmp) nounwind
  ret void
}

; Don't do this if the load is volatile.

; CHECK-LABEL: define void @test1(i8* %p) {
; CHECK-NEXT: entry:
; CHECK-NEXT:   %0 = tail call i8* @objc_retain(i8* %p) [[NUW]]
; CHECK-NEXT:   %tmp = load volatile i8*, i8** @x, align 8
; CHECK-NEXT:   store i8* %0, i8** @x, align 8
; CHECK-NEXT:   tail call void @objc_release(i8* %tmp) [[NUW]]
; CHECK-NEXT:   ret void
; CHECK-NEXT: }
define void @test1(i8* %p) {
entry:
  %0 = tail call i8* @objc_retain(i8* %p) nounwind
  %tmp = load volatile i8*, i8** @x, align 8
  store i8* %0, i8** @x, align 8
  tail call void @objc_release(i8* %tmp) nounwind
  ret void
}

; Don't do this if the store is volatile.

; CHECK-LABEL: define void @test2(i8* %p) {
; CHECK-NEXT: entry:
; CHECK-NEXT:   %0 = tail call i8* @objc_retain(i8* %p) [[NUW]]
; CHECK-NEXT:   %tmp = load i8*, i8** @x, align 8
; CHECK-NEXT:   store volatile i8* %0, i8** @x, align 8
; CHECK-NEXT:   tail call void @objc_release(i8* %tmp) [[NUW]]
; CHECK-NEXT:   ret void
; CHECK-NEXT: }
define void @test2(i8* %p) {
entry:
  %0 = tail call i8* @objc_retain(i8* %p) nounwind
  %tmp = load i8*, i8** @x, align 8
  store volatile i8* %0, i8** @x, align 8
  tail call void @objc_release(i8* %tmp) nounwind
  ret void
}

; Don't do this if there's a use of the old pointer value between the store
; and the release.

; CHECK-LABEL: define void @test3(i8* %newValue) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    %x0 = tail call i8* @objc_retain(i8* %newValue) [[NUW]]
; CHECK-NEXT:    %x1 = load i8*, i8** @x, align 8
; CHECK-NEXT:    store i8* %x0, i8** @x, align 8
; CHECK-NEXT:    tail call void @use_pointer(i8* %x1), !clang.arc.no_objc_arc_exceptions !0
; CHECK-NEXT:    tail call void @objc_release(i8* %x1) [[NUW]], !clang.imprecise_release !0
; CHECK-NEXT:    ret void
; CHECK-NEXT:  }
define void @test3(i8* %newValue) {
entry:
  %x0 = tail call i8* @objc_retain(i8* %newValue) nounwind
  %x1 = load i8*, i8** @x, align 8
  store i8* %newValue, i8** @x, align 8
  tail call void @use_pointer(i8* %x1), !clang.arc.no_objc_arc_exceptions !0
  tail call void @objc_release(i8* %x1) nounwind, !clang.imprecise_release !0
  ret void
}

; Like test3, but with an icmp use instead of a call, for good measure.

; CHECK-LABEL:  define i1 @test4(i8* %newValue, i8* %foo) {
; CHECK-NEXT:   entry:
; CHECK-NEXT:     %x0 = tail call i8* @objc_retain(i8* %newValue) [[NUW]]
; CHECK-NEXT:     %x1 = load i8*, i8** @x, align 8
; CHECK-NEXT:     store i8* %x0, i8** @x, align 8
; CHECK-NEXT:     %t = icmp eq i8* %x1, %foo
; CHECK-NEXT:     tail call void @objc_release(i8* %x1) [[NUW]], !clang.imprecise_release !0
; CHECK-NEXT:     ret i1 %t
; CHECK-NEXT:   }
define i1 @test4(i8* %newValue, i8* %foo) {
entry:
  %x0 = tail call i8* @objc_retain(i8* %newValue) nounwind
  %x1 = load i8*, i8** @x, align 8
  store i8* %newValue, i8** @x, align 8
  %t = icmp eq i8* %x1, %foo
  tail call void @objc_release(i8* %x1) nounwind, !clang.imprecise_release !0
  ret i1 %t
}

; Do form an objc_storeStrong here, because the use is before the store.

; CHECK-LABEL: define i1 @test5(i8* %newValue, i8* %foo) {
; CHECK: %t = icmp eq i8* %x1, %foo
; CHECK: tail call void @objc_storeStrong(i8** @x, i8* %newValue) [[NUW]]
; CHECK: }
define i1 @test5(i8* %newValue, i8* %foo) {
entry:
  %x0 = tail call i8* @objc_retain(i8* %newValue) nounwind
  %x1 = load i8*, i8** @x, align 8
  %t = icmp eq i8* %x1, %foo
  store i8* %newValue, i8** @x, align 8
  tail call void @objc_release(i8* %x1) nounwind, !clang.imprecise_release !0
  ret i1 %t
}

; Like test5, but the release is before the store.

; CHECK-LABEL: define i1 @test6(i8* %newValue, i8* %foo) {
; CHECK: %t = icmp eq i8* %x1, %foo
; CHECK: tail call void @objc_storeStrong(i8** @x, i8* %newValue) [[NUW]]
; CHECK: }
define i1 @test6(i8* %newValue, i8* %foo) {
entry:
  %x0 = tail call i8* @objc_retain(i8* %newValue) nounwind
  %x1 = load i8*, i8** @x, align 8
  tail call void @objc_release(i8* %x1) nounwind, !clang.imprecise_release !0
  %t = icmp eq i8* %x1, %foo
  store i8* %newValue, i8** @x, align 8
  ret i1 %t
}

; Like test0, but there's no store, so don't form an objc_storeStrong.

; CHECK-LABEL: define void @test7(
; CHECK-NEXT: entry:
; CHECK-NEXT:   %0 = tail call i8* @objc_retain(i8* %p) [[NUW]]
; CHECK-NEXT:   %tmp = load i8*, i8** @x, align 8
; CHECK-NEXT:   tail call void @objc_release(i8* %tmp) [[NUW]]
; CHECK-NEXT:   ret void
; CHECK-NEXT: }
define void @test7(i8* %p) {
entry:
  %0 = tail call i8* @objc_retain(i8* %p) nounwind
  %tmp = load i8*, i8** @x, align 8
  tail call void @objc_release(i8* %tmp) nounwind
  ret void
}

; Like test0, but there's no retain, so don't form an objc_storeStrong.

; CHECK-LABEL: define void @test8(
; CHECK-NEXT: entry:
; CHECK-NEXT:   %tmp = load i8*, i8** @x, align 8
; CHECK-NEXT:   store i8* %p, i8** @x, align 8
; CHECK-NEXT:   tail call void @objc_release(i8* %tmp) [[NUW]]
; CHECK-NEXT:   ret void
; CHECK-NEXT: }
define void @test8(i8* %p) {
entry:
  %tmp = load i8*, i8** @x, align 8
  store i8* %p, i8** @x, align 8
  tail call void @objc_release(i8* %tmp) nounwind
  ret void
}

; Make sure that we properly handle release that *may* release our new
; value in between the retain and the store. We need to be sure that
; this we can safely move the retain to the store. This specific test
; makes sure that we properly handled a release of an unrelated
; pointer.
;
; CHECK-LABEL: define i1 @test9(i8* %newValue, i8* %foo, i8* %unrelated_ptr) {
; CHECK-NOT: objc_storeStrong
define i1 @test9(i8* %newValue, i8* %foo, i8* %unrelated_ptr) {
entry:
  %x0 = tail call i8* @objc_retain(i8* %newValue) nounwind
  tail call void @objc_release(i8* %unrelated_ptr) nounwind, !clang.imprecise_release !0
  %x1 = load i8*, i8** @x, align 8
  tail call void @objc_release(i8* %x1) nounwind, !clang.imprecise_release !0
  %t = icmp eq i8* %x1, %foo
  store i8* %newValue, i8** @x, align 8
  ret i1 %t  
}

; Make sure that we don't perform the optimization when we just have a call.
;
; CHECK-LABEL: define i1 @test10(i8* %newValue, i8* %foo, i8* %unrelated_ptr) {
; CHECK-NOT: objc_storeStrong
define i1 @test10(i8* %newValue, i8* %foo, i8* %unrelated_ptr) {
entry:
  %x0 = tail call i8* @objc_retain(i8* %newValue) nounwind
  call void @use_pointer(i8* %unrelated_ptr)
  %x1 = load i8*, i8** @x, align 8
  tail call void @objc_release(i8* %x1) nounwind, !clang.imprecise_release !0
  %t = icmp eq i8* %x1, %foo
  store i8* %newValue, i8** @x, align 8
  ret i1 %t
}

; Make sure we form the store strong if the use in between the retain
; and the store does not touch reference counts.
; CHECK-LABEL: define i1 @test11(i8* %newValue, i8* %foo, i8* %unrelated_ptr) {
; CHECK: objc_storeStrong
define i1 @test11(i8* %newValue, i8* %foo, i8* %unrelated_ptr) {
entry:
  %x0 = tail call i8* @objc_retain(i8* %newValue) nounwind
  %t = icmp eq i8* %newValue, %foo
  %x1 = load i8*, i8** @x, align 8
  tail call void @objc_release(i8* %x1) nounwind, !clang.imprecise_release !0
  store i8* %newValue, i8** @x, align 8
  ret i1 %t
}

!0 = !{}

; CHECK: attributes [[NUW]] = { nounwind }
