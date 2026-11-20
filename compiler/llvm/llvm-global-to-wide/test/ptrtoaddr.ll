; REQUIRES: llvm-22-or-newer
; RUN: opt --load-pass-plugin=%bindir/llvm-pgas.%soext -S --passes=global-to-wide < %s | FileCheck %s --check-prefixes=FULL,COMMON
; RUN: sed 's/p100:128:64:64-p101/p100:128:64:64:64-p101/' %s | opt --load-pass-plugin=%bindir/llvm-pgas.%soext -S --passes=global-to-wide | FileCheck %s --check-prefixes=NARROW,COMMON
; RUN: opt --load-pass-plugin=%bindir/llvm-pgas.%soext -S --passes='global-to-wide,function(sroa,instcombine,simplifycfg)' < %s | FileCheck %s --check-prefixes=SEMANTIC,FULL-SEMANTIC
; RUN: sed 's/p100:128:64:64-p101/p100:128:64:64:64-p101/' %s | opt --load-pass-plugin=%bindir/llvm-pgas.%soext -S --passes='global-to-wide,function(sroa,instcombine,simplifycfg)' | FileCheck %s --check-prefixes=SEMANTIC,NARROW-SEMANTIC

target datalayout = "e-p:64:64:64-i64:64-p100:128:64:64-p101:128:64:64"

%struct.c_localeid_t = type { i32 }

declare ptr addrspace(100) @.gf.make.1(%struct.c_localeid_t, ptr) readnone
declare ptr @.gf.addr.1(ptr addrspace(100)) readnone
declare %struct.c_localeid_t @.gf.loc.1(ptr addrspace(100)) readnone

; FULL-LABEL: define i128 @wide_addr({ %struct.c_localeid_t, ptr } %p)
; FULL: [[FULL_ADDR:%.*]] = extractvalue { %struct.c_localeid_t, ptr } %p, 1
; FULL: [[FULL_ADDR_BITS:%.*]] = ptrtoint ptr [[FULL_ADDR]] to i128
; FULL: [[FULL_RAW:%.*]] = or i128 {{%.*}}, [[FULL_ADDR_BITS]]
; FULL: ret i128 [[FULL_RAW]]
; NARROW-LABEL: define i128 @wide_addr({ %struct.c_localeid_t, ptr } %p)
; NARROW: [[NARROW_RAW:%.*]] = or i128
; NARROW: [[NARROW_ADDR:%.*]] = trunc i128 [[NARROW_RAW]] to i64
; NARROW: [[NARROW_EXT:%.*]] = zext i64 [[NARROW_ADDR]] to i128
; NARROW: ret i128 [[NARROW_EXT]]
define i128 @wide_addr(ptr addrspace(100) %p) {
  %addr = ptrtoaddr ptr addrspace(100) %p to i128
  ret i128 %addr
}

; COMMON-LABEL: define i64 @wide_addr_trunc({ %struct.c_localeid_t, ptr } %p)
; COMMON: [[TRUNC_RAW:%.*]] = or i128
; COMMON: [[TRUNC_ADDR:%.*]] = trunc i128 [[TRUNC_RAW]] to i64
; COMMON: ret i64 [[TRUNC_ADDR]]
define i64 @wide_addr_trunc(ptr addrspace(100) %p) {
  %addr = ptrtoaddr ptr addrspace(100) %p to i64
  ret i64 %addr
}

; COMMON-LABEL: define i128 @wide_int({ %struct.c_localeid_t, ptr } %p)
; COMMON: [[INT_RAW:%.*]] = or i128
; COMMON: ret i128 [[INT_RAW]]
define i128 @wide_int(ptr addrspace(100) %p) {
  %value = ptrtoint ptr addrspace(100) %p to i128
  ret i128 %value
}

; COMMON-LABEL: define i64 @local_addr(ptr %p)
; COMMON: [[LOCAL:%.*]] = ptrtoaddr ptr %p to i64
; COMMON: ret i64 [[LOCAL]]
define i64 @local_addr(ptr %p) {
  %addr = ptrtoaddr ptr %p to i64
  ret i64 %addr
}

; SEMANTIC-LABEL: define i1 @overlap_guard()
; SEMANTIC: ret i1 true
define i1 @overlap_guard() {
  %dst = call ptr addrspace(100) @.gf.make.1(%struct.c_localeid_t { i32 7 }, ptr inttoptr (i64 4096 to ptr))
  %src = call ptr addrspace(100) @.gf.make.1(%struct.c_localeid_t { i32 7 }, ptr inttoptr (i64 4104 to ptr))
  %dstaddr = ptrtoaddr ptr addrspace(100) %dst to i128
  %srcaddr = ptrtoaddr ptr addrspace(100) %src to i128
  %difference = sub i128 %dstaddr, %srcaddr
  %overlap = icmp ugt i128 %difference, -32
  ret i1 %overlap
}

; FULL-SEMANTIC-LABEL: define i1 @distinct_locales()
; FULL-SEMANTIC: ret i1 true
; NARROW-SEMANTIC-LABEL: define i1 @distinct_locales()
; NARROW-SEMANTIC: ret i1 false
define i1 @distinct_locales() {
  %a = call ptr addrspace(100) @.gf.make.1(%struct.c_localeid_t { i32 7 }, ptr inttoptr (i64 4096 to ptr))
  %b = call ptr addrspace(100) @.gf.make.1(%struct.c_localeid_t { i32 8 }, ptr inttoptr (i64 4096 to ptr))
  %aaddr = ptrtoaddr ptr addrspace(100) %a to i128
  %baddr = ptrtoaddr ptr addrspace(100) %b to i128
  %distinct = icmp ne i128 %aaddr, %baddr
  ret i1 %distinct
}

; SEMANTIC-LABEL: define i1 @integer_roundtrip()
; SEMANTIC: ret i1 true
define i1 @integer_roundtrip() {
  %original = call ptr addrspace(100) @.gf.make.1(%struct.c_localeid_t { i32 7 }, ptr inttoptr (i64 4096 to ptr))
  %bits = ptrtoint ptr addrspace(100) %original to i128
  %converted = inttoptr i128 %bits to ptr addrspace(100)
  %locale = call %struct.c_localeid_t @.gf.loc.1(ptr addrspace(100) %converted)
  %node = extractvalue %struct.c_localeid_t %locale, 0
  %addr = call ptr @.gf.addr.1(ptr addrspace(100) %converted)
  %addrbits = ptrtoint ptr %addr to i64
  %node_ok = icmp eq i32 %node, 7
  %addr_ok = icmp eq i64 %addrbits, 4096
  %ok = and i1 %node_ok, %addr_ok
  ret i1 %ok
}
