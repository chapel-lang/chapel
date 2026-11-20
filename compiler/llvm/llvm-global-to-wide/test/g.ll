; RUN: opt --load-pass-plugin=%bindir/llvm-pgas.%soext -S --passes=global-to-wide < %s | FileCheck %s
; RUN: opt --load-pass-plugin=%bindir/llvm-pgas.%soext -S --passes='global-to-wide,function(sroa,instcombine,simplifycfg)' < %s | FileCheck %s --check-prefix=SEMANTIC

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128-p100:128:64:64-p101:128:64:64"

%struct.c_localeid_t = type { i32, i32 }

; Note after LLVM 7 memcpy will no longer take an alignment argument
declare void @llvm.memcpy.p0i8.p100i8.i64(ptr nocapture, ptr addrspace(100) nocapture, i64, i32, i1)
declare void @llvm.memcpy.p100i8.p0i8.i64(ptr addrspace(100) nocapture, ptr nocapture, i64, i32, i1)
declare void @llvm.memcpy.p100i8.p100i8.i64(ptr addrspace(100) nocapture, ptr addrspace(100) nocapture, i64, i32, i1)
declare ptr addrspace(100) @.gf.make.1(%struct.c_localeid_t, ptr) readnone
declare ptr @.gf.addr.1(ptr addrspace(100)) readnone
declare %struct.c_localeid_t @.gf.loc.1(ptr addrspace(100)) readnone

define i128 @testptrtoint(ptr addrspace(100) %ptr) {
; CHECK: i128 @testptrtoint(
; CHECK: { %struct.c_localeid_t, ptr } %
; )
; CHECK: [[ADDR:%.*]] = extractvalue { %struct.c_localeid_t, ptr } %ptr, 1
; CHECK: [[ADDR_BITS:%.*]] = ptrtoint ptr [[ADDR]] to i128
; CHECK: [[VALUE:%.*]] = or i128 {{%.*}}, [[ADDR_BITS]]
; CHECK: ret i128 [[VALUE]]
entry:
  %ret = ptrtoint ptr addrspace(100) %ptr to i128
  ret i128 %ret
}

define i64 @testptrtoint_trunc(ptr addrspace(100) %ptr) {
; CHECK: i64 @testptrtoint_trunc(
; CHECK: [[RAW:%.*]] = or i128
; CHECK: [[TRUNC:%.*]] = trunc i128 [[RAW]] to i64
; CHECK: ret i64 [[TRUNC]]
entry:
  %ret = ptrtoint ptr addrspace(100) %ptr to i64
  ret i64 %ret
}

define i256 @testptrtoint_extend(ptr addrspace(100) %ptr) {
; CHECK: i256 @testptrtoint_extend(
; CHECK: [[RAW_EXT:%.*]] = or i128
; CHECK: [[EXT:%.*]] = zext i128 [[RAW_EXT]] to i256
; CHECK: ret i256 [[EXT]]
entry:
  %ret = ptrtoint ptr addrspace(100) %ptr to i256
  ret i256 %ret
}


define ptr addrspace(100) @testinttoptr(i128 %i) {
; CHECK: { %struct.c_localeid_t, ptr } @testinttoptr(i128 %
; )
; CHECK: [[ADDR_PTR:%.*]] = inttoptr i128 %i to ptr
; CHECK: [[LOCALE_HIGH:%.*]] = lshr i128 %i, 64
; CHECK: [[LOCALE_BITS:%.*]] = trunc i128 [[LOCALE_HIGH]] to i64
; CHECK: store i64 [[LOCALE_BITS]], ptr %
; CHECK: [[LOCALE:%.*]] = load %struct.c_localeid_t, ptr %
; CHECK: [[WITH_LOCALE:%.*]] = insertvalue { %struct.c_localeid_t, ptr } undef, %struct.c_localeid_t [[LOCALE]], 0
; CHECK: [[WIDE:%.*]] = insertvalue { %struct.c_localeid_t, ptr } [[WITH_LOCALE]], ptr [[ADDR_PTR]], 1
; CHECK: ret { %struct.c_localeid_t, ptr } [[WIDE]]
entry:
  %ret = inttoptr i128 %i to ptr addrspace(100)
  ret ptr addrspace(100) %ret
}

define ptr addrspace(100) @testinttoptr_extend(i64 %i) {
; CHECK: [[WIDE_INT:%.*]] = zext i64 %i to i128
; CHECK: inttoptr i128 [[WIDE_INT]] to ptr
entry:
  %ret = inttoptr i64 %i to ptr addrspace(100)
  ret ptr addrspace(100) %ret
}

; SEMANTIC-LABEL: define i1 @roundtrip_locale_and_address()
; SEMANTIC: ret i1 true
define i1 @roundtrip_locale_and_address() {
  %original = call ptr addrspace(100) @.gf.make.1(%struct.c_localeid_t { i32 7, i32 42 }, ptr inttoptr (i64 4096 to ptr))
  %bits = ptrtoint ptr addrspace(100) %original to i128
  %converted = inttoptr i128 %bits to ptr addrspace(100)
  %locale = call %struct.c_localeid_t @.gf.loc.1(ptr addrspace(100) %converted)
  %node = extractvalue %struct.c_localeid_t %locale, 0
  %subloc = extractvalue %struct.c_localeid_t %locale, 1
  %addr = call ptr @.gf.addr.1(ptr addrspace(100) %converted)
  %addrbits = ptrtoint ptr %addr to i64
  %node_ok = icmp eq i32 %node, 7
  %subloc_ok = icmp eq i32 %subloc, 42
  %addr_ok = icmp eq i64 %addrbits, 4096
  %locale_ok = and i1 %node_ok, %subloc_ok
  %ok = and i1 %locale_ok, %addr_ok
  ret i1 %ok
}
