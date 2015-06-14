; RUN: llc < %s | FileCheck %s

; Test case based on this source:
; int puts(const char*);
; __declspec(noinline) void crash() {
;   *(volatile int*)0 = 42;
; }
; int filt();
; void use_both() {
;   __try {
;     __try {
;       crash();
;     } __finally {
;       puts("__finally");
;     }
;   } __except (filt()) {
;     puts("__except");
;   }
; }

target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc"

$"\01??_C@_09KJEHOMHG@__finally?$AA@" = comdat any

$"\01??_C@_08MLCMLGHM@__except?$AA@" = comdat any

@"\01??_C@_09KJEHOMHG@__finally?$AA@" = linkonce_odr unnamed_addr constant [10 x i8] c"__finally\00", comdat, align 1
@"\01??_C@_08MLCMLGHM@__except?$AA@" = linkonce_odr unnamed_addr constant [9 x i8] c"__except\00", comdat, align 1

declare void @crash()

declare i32 @filt()

; Function Attrs: nounwind uwtable
define void @use_both() #1 {
entry:
  %exn.slot = alloca i8*
  %ehselector.slot = alloca i32
  invoke void @crash() #5
          to label %invoke.cont unwind label %lpad

invoke.cont:                                      ; preds = %entry
  %0 = call i8* @llvm.frameaddress(i32 0)
  invoke void @"\01?fin$0@0@use_both@@"(i1 zeroext false, i8* %0) #5
          to label %invoke.cont2 unwind label %lpad1

invoke.cont2:                                     ; preds = %invoke.cont
  br label %__try.cont

lpad:                                             ; preds = %entry
  %1 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__C_specific_handler to i8*)
          cleanup
          catch i8* bitcast (i32 (i8*, i8*)* @"\01?filt$0@0@use_both@@" to i8*)
  %2 = extractvalue { i8*, i32 } %1, 0
  store i8* %2, i8** %exn.slot
  %3 = extractvalue { i8*, i32 } %1, 1
  store i32 %3, i32* %ehselector.slot
  %4 = call i8* @llvm.frameaddress(i32 0)
  invoke void @"\01?fin$0@0@use_both@@"(i1 zeroext true, i8* %4) #5
          to label %invoke.cont3 unwind label %lpad1

lpad1:                                            ; preds = %lpad, %invoke.cont
  %5 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__C_specific_handler to i8*)
          catch i8* bitcast (i32 (i8*, i8*)* @"\01?filt$0@0@use_both@@" to i8*)
  %6 = extractvalue { i8*, i32 } %5, 0
  store i8* %6, i8** %exn.slot
  %7 = extractvalue { i8*, i32 } %5, 1
  store i32 %7, i32* %ehselector.slot
  br label %catch.dispatch

invoke.cont3:                                     ; preds = %lpad
  br label %catch.dispatch

catch.dispatch:                                   ; preds = %invoke.cont3, %lpad1
  %sel = load i32, i32* %ehselector.slot
  %8 = call i32 @llvm.eh.typeid.for(i8* bitcast (i32 (i8*, i8*)* @"\01?filt$0@0@use_both@@" to i8*)) #6
  %matches = icmp eq i32 %sel, %8
  br i1 %matches, label %__except, label %eh.resume

__except:                                         ; preds = %catch.dispatch
  %call = call i32 @puts(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"\01??_C@_08MLCMLGHM@__except?$AA@", i32 0, i32 0))
  br label %__try.cont

__try.cont:                                       ; preds = %__except, %invoke.cont2
  ret void

eh.resume:                                        ; preds = %catch.dispatch
  %exn = load i8*, i8** %exn.slot
  %sel4 = load i32, i32* %ehselector.slot
  %lpad.val = insertvalue { i8*, i32 } undef, i8* %exn, 0
  %lpad.val5 = insertvalue { i8*, i32 } %lpad.val, i32 %sel4, 1
  resume { i8*, i32 } %lpad.val5
}

; CHECK-LABEL: use_both:
; CHECK: .Ltmp0
; CHECK: callq crash
; CHECK: .Ltmp1
; CHECK: .Ltmp3
; CHECK: callq "?fin$0@0@use_both@@"
; CHECK: .Ltmp4
; CHECK: retq
;
; CHECK: .seh_handlerdata
; CHECK-NEXT: .long 3
; CHECK-NEXT: .long .Ltmp0@IMGREL
; CHECK-NEXT: .long .Ltmp1@IMGREL+1
; CHECK-NEXT: .long "?fin$0@0@use_both@@"@IMGREL
; CHECK-NEXT: .long 0
; CHECK-NEXT: .long .Ltmp0@IMGREL
; CHECK-NEXT: .long .Ltmp1@IMGREL+1
; CHECK-NEXT: .long "?filt$0@0@use_both@@"@IMGREL
; CHECK-NEXT: .long .Ltmp{{[0-9]+}}@IMGREL
; CHECK-NEXT: .long .Ltmp3@IMGREL
; CHECK-NEXT: .long .Ltmp4@IMGREL+1
; CHECK-NEXT: .long "?filt$0@0@use_both@@"@IMGREL
; CHECK-NEXT: .long .Ltmp{{[0-9]+}}@IMGREL

; Function Attrs: noinline nounwind
define internal i32 @"\01?filt$0@0@use_both@@"(i8* %exception_pointers, i8* %frame_pointer) #2 {
entry:
  %frame_pointer.addr = alloca i8*, align 8
  %exception_pointers.addr = alloca i8*, align 8
  %exn.slot = alloca i8*
  store i8* %frame_pointer, i8** %frame_pointer.addr, align 8
  store i8* %exception_pointers, i8** %exception_pointers.addr, align 8
  %0 = load i8*, i8** %exception_pointers.addr
  %1 = bitcast i8* %0 to { i32*, i8* }*
  %2 = getelementptr inbounds { i32*, i8* }, { i32*, i8* }* %1, i32 0, i32 0
  %3 = load i32*, i32** %2
  %4 = load i32, i32* %3
  %5 = zext i32 %4 to i64
  %6 = inttoptr i64 %5 to i8*
  store i8* %6, i8** %exn.slot
  %call = call i32 @filt()
  ret i32 %call
}

define internal void @"\01?fin$0@0@use_both@@"(i1 zeroext %abnormal_termination, i8* %frame_pointer) #3 {
entry:
  %frame_pointer.addr = alloca i8*, align 8
  %abnormal_termination.addr = alloca i8, align 1
  store i8* %frame_pointer, i8** %frame_pointer.addr, align 8
  %frombool = zext i1 %abnormal_termination to i8
  store i8 %frombool, i8* %abnormal_termination.addr, align 1
  %call = call i32 @puts(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09KJEHOMHG@__finally?$AA@", i32 0, i32 0))
  ret void
}

declare i32 @puts(i8*) #3

declare i32 @__C_specific_handler(...)

; Function Attrs: nounwind readnone
declare i8* @llvm.frameaddress(i32) #4

; Function Attrs: nounwind readnone
declare i32 @llvm.eh.typeid.for(i8*) #4

attributes #0 = { noinline nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noinline nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind readnone }
attributes #5 = { noinline }
attributes #6 = { nounwind }
