; RUN: llc < %s -mtriple=x86_64-apple-darwin -mcpu=knl --show-mc-encoding | FileCheck %s --check-prefix AVX512
; RUN: llc < %s -mtriple=x86_64-apple-darwin -mcpu=corei7-avx --show-mc-encoding | FileCheck %s --check-prefix AVX

; AVX512-LABEL: @test_fdiv
; AVX512: vdivss %xmm{{.*}} ## encoding: [0x62
; AVX-LABEL: @test_fdiv
; AVX:    vdivss %xmm{{.*}} ## encoding: [0xc5

define float @test_fdiv(float %a, float %b) {
  %c = fdiv float %a, %b
  ret float %c
}

; AVX512-LABEL: @test_fsub
; AVX512: vsubss %xmm{{.*}} ## encoding: [0x62
; AVX-LABEL: @test_fsub
; AVX:    vsubss %xmm{{.*}} ## encoding: [0xc5

define float @test_fsub(float %a, float %b) {
  %c = fsub float %a, %b
  ret float %c
}

; AVX512-LABEL: @test_fadd
; AVX512: vaddsd %xmm{{.*}} ## encoding: [0x62
; AVX-LABEL: @test_fadd
; AVX:    vaddsd %xmm{{.*}} ## encoding: [0xc5

define double @test_fadd(double %a, double %b) {
  %c = fadd double %a, %b
  ret double %c
}

declare float     @llvm.trunc.f32(float  %Val)
declare double    @llvm.trunc.f64(double %Val)
declare float     @llvm.rint.f32(float  %Val)
declare double    @llvm.rint.f64(double %Val)
declare double    @llvm.sqrt.f64(double %Val)
declare float     @llvm.sqrt.f32(float  %Val)

; AVX512-LABEL: @test_trunc
; AVX512: vrndscaless
; AVX-LABEL: @test_trunc
; AVX:    vroundss

define float @test_trunc(float %a) {
  %c = call float @llvm.trunc.f32(float %a)
  ret float %c
}

; AVX512-LABEL: @test_sqrt
; AVX512: vsqrtsd %xmm{{.*}} ## encoding: [0x62
; AVX-LABEL: @test_sqrt
; AVX:    vsqrtsd %xmm{{.*}} ## encoding: [0xc5

define double @test_sqrt(double %a) {
  %c = call double @llvm.sqrt.f64(double %a)
  ret double %c
}

; AVX512-LABEL: @test_rint
; AVX512: vrndscaless
; AVX-LABEL: @test_rint
; AVX:    vroundss

define float @test_rint(float %a) {
  %c = call float @llvm.rint.f32(float %a)
  ret float %c
}

; AVX512-LABEL: @test_vmax
; AVX512: vmaxss %xmm{{.*}} ## encoding: [0x62
; AVX-LABEL: @test_vmax
; AVX:    vmaxss %xmm{{.*}} ## encoding: [0xc5

define float @test_vmax(float %i, float %j) {
  %cmp_res = fcmp ogt float %i, %j
  %max = select i1 %cmp_res, float %i, float %j
  ret float %max
}

; AVX512-LABEL: @test_mov
; AVX512: vcmpltss %xmm{{.*}} ## encoding: [0x62
; AVX-LABEL: @test_mov
; AVX:    vcmpltss %xmm{{.*}} ## encoding: [0xc5

define float @test_mov(float %a, float %b, float %i, float %j) {
  %cmp_res = fcmp ogt float %i, %j
  %max = select i1 %cmp_res, float %b, float %a
  ret float %max
}

