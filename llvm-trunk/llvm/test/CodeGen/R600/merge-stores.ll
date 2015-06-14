; RUN: llc -march=amdgcn -verify-machineinstrs < %s | FileCheck -check-prefix=SI -check-prefix=GCN %s
; RUN: llc -march=amdgcn -mcpu=bonaire -verify-machineinstrs < %s | FileCheck -check-prefix=SI -check-prefix=GCN %s

; Run with devices with different unaligned load restrictions.

; TODO: Vector element tests
; TODO: Non-zero base offset for load and store combinations
; TODO: Same base addrspacecasted


; GCN-LABEL: {{^}}merge_global_store_2_constants_i8:
; GCN: buffer_store_byte
; GCN: buffer_store_byte
; GCN: s_endpgm
define void @merge_global_store_2_constants_i8(i8 addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr i8, i8 addrspace(1)* %out, i32 1

  store i8 123, i8 addrspace(1)* %out.gep.1
  store i8 456, i8 addrspace(1)* %out, align 2
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_2_constants_i8_natural_align:
; GCN: buffer_store_byte
; GCN: buffer_store_byte
; GCN: s_endpgm
define void @merge_global_store_2_constants_i8_natural_align(i8 addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr i8, i8 addrspace(1)* %out, i32 1

  store i8 123, i8 addrspace(1)* %out.gep.1
  store i8 456, i8 addrspace(1)* %out
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_2_constants_i16:
; GCN: buffer_store_dword v
define void @merge_global_store_2_constants_i16(i16 addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr i16, i16 addrspace(1)* %out, i32 1

  store i16 123, i16 addrspace(1)* %out.gep.1
  store i16 456, i16 addrspace(1)* %out, align 4
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_2_constants_0_i16:
; GCN: buffer_store_dword v
define void @merge_global_store_2_constants_0_i16(i16 addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr i16, i16 addrspace(1)* %out, i32 1

  store i16 0, i16 addrspace(1)* %out.gep.1
  store i16 0, i16 addrspace(1)* %out, align 4
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_2_constants_i16_natural_align:
; GCN: buffer_store_short
; GCN: buffer_store_short
; GCN: s_endpgm
define void @merge_global_store_2_constants_i16_natural_align(i16 addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr i16, i16 addrspace(1)* %out, i32 1

  store i16 123, i16 addrspace(1)* %out.gep.1
  store i16 456, i16 addrspace(1)* %out
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_2_constants_i32:
; SI-DAG: s_movk_i32 [[SLO:s[0-9]+]], 0x1c8
; SI-DAG: s_movk_i32 [[SHI:s[0-9]+]], 0x7b
; SI-DAG: v_mov_b32_e32 v[[LO:[0-9]+]], [[SLO]]
; SI-DAG: v_mov_b32_e32 v[[HI:[0-9]+]], [[SHI]]
; GCN: buffer_store_dwordx2 v{{\[}}[[LO]]:[[HI]]{{\]}}
define void @merge_global_store_2_constants_i32(i32 addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 1

  store i32 123, i32 addrspace(1)* %out.gep.1
  store i32 456, i32 addrspace(1)* %out
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_2_constants_i32_f32:
; GCN: buffer_store_dwordx2
define void @merge_global_store_2_constants_i32_f32(i32 addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 1
  %out.gep.1.bc = bitcast i32 addrspace(1)* %out.gep.1 to float addrspace(1)*
  store float 1.0, float addrspace(1)* %out.gep.1.bc
  store i32 456, i32 addrspace(1)* %out
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_2_constants_f32_i32:
; GCN: buffer_store_dwordx2
define void @merge_global_store_2_constants_f32_i32(float addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr float, float addrspace(1)* %out, i32 1
  %out.gep.1.bc = bitcast float addrspace(1)* %out.gep.1 to i32 addrspace(1)*
  store i32 123, i32 addrspace(1)* %out.gep.1.bc
  store float 4.0, float addrspace(1)* %out
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_4_constants_i32:
; GCN: buffer_store_dwordx4
define void @merge_global_store_4_constants_i32(i32 addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 1
  %out.gep.2 = getelementptr i32, i32 addrspace(1)* %out, i32 2
  %out.gep.3 = getelementptr i32, i32 addrspace(1)* %out, i32 3

  store i32 123, i32 addrspace(1)* %out.gep.1
  store i32 456, i32 addrspace(1)* %out.gep.2
  store i32 333, i32 addrspace(1)* %out.gep.3
  store i32 1234, i32 addrspace(1)* %out
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_4_constants_f32_order:
; XGCN: buffer_store_dwordx4
; GCN: buffer_store_dword v
; GCN: buffer_store_dword v
; GCN: buffer_store_dwordx2 v
define void @merge_global_store_4_constants_f32_order(float addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr float, float addrspace(1)* %out, i32 1
  %out.gep.2 = getelementptr float, float addrspace(1)* %out, i32 2
  %out.gep.3 = getelementptr float, float addrspace(1)* %out, i32 3

  store float 8.0, float addrspace(1)* %out
  store float 1.0, float addrspace(1)* %out.gep.1
  store float 2.0, float addrspace(1)* %out.gep.2
  store float 4.0, float addrspace(1)* %out.gep.3
  ret void
}

; First store is out of order. Because of order of combines, the
; consecutive store fails because only some of the stores have been
; replaced with integer constant stores, and then won't merge because
; the types are different.

; GCN-LABEL: {{^}}merge_global_store_4_constants_f32:
; XGCN: buffer_store_dwordx4
; GCN: buffer_store_dword v
; GCN: buffer_store_dword v
; GCN: buffer_store_dword v
; GCN: buffer_store_dword v
define void @merge_global_store_4_constants_f32(float addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr float, float addrspace(1)* %out, i32 1
  %out.gep.2 = getelementptr float, float addrspace(1)* %out, i32 2
  %out.gep.3 = getelementptr float, float addrspace(1)* %out, i32 3

  store float 1.0, float addrspace(1)* %out.gep.1
  store float 2.0, float addrspace(1)* %out.gep.2
  store float 4.0, float addrspace(1)* %out.gep.3
  store float 8.0, float addrspace(1)* %out
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_3_constants_i32:
; SI-DAG: buffer_store_dwordx2
; SI-DAG: buffer_store_dword
; SI-NOT: buffer_store_dword
; GCN: s_endpgm
define void @merge_global_store_3_constants_i32(i32 addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 1
  %out.gep.2 = getelementptr i32, i32 addrspace(1)* %out, i32 2

  store i32 123, i32 addrspace(1)* %out.gep.1
  store i32 456, i32 addrspace(1)* %out.gep.2
  store i32 1234, i32 addrspace(1)* %out
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_2_constants_i64:
; XGCN: buffer_store_dwordx4
; GCN: buffer_store_dwordx2
; GCN: buffer_store_dwordx2
define void @merge_global_store_2_constants_i64(i64 addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr i64, i64 addrspace(1)* %out, i64 1

  store i64 123, i64 addrspace(1)* %out.gep.1
  store i64 456, i64 addrspace(1)* %out
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_4_constants_i64:
; XGCN: buffer_store_dwordx4
; XGCN: buffer_store_dwordx4

; GCN: buffer_store_dwordx2
; GCN: buffer_store_dwordx2
; GCN: buffer_store_dwordx2
; GCN: buffer_store_dwordx2
define void @merge_global_store_4_constants_i64(i64 addrspace(1)* %out) #0 {
  %out.gep.1 = getelementptr i64, i64 addrspace(1)* %out, i64 1
  %out.gep.2 = getelementptr i64, i64 addrspace(1)* %out, i64 2
  %out.gep.3 = getelementptr i64, i64 addrspace(1)* %out, i64 3

  store i64 123, i64 addrspace(1)* %out.gep.1
  store i64 456, i64 addrspace(1)* %out.gep.2
  store i64 333, i64 addrspace(1)* %out.gep.3
  store i64 1234, i64 addrspace(1)* %out
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_2_adjacent_loads_i32:
; GCN: buffer_load_dwordx2 [[LOAD:v\[[0-9]+:[0-9]+\]]]
; GCN: buffer_store_dwordx2 [[LOAD]]
define void @merge_global_store_2_adjacent_loads_i32(i32 addrspace(1)* %out, i32 addrspace(1)* %in) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 1
  %in.gep.1 = getelementptr i32, i32 addrspace(1)* %in, i32 1

  %lo = load i32, i32 addrspace(1)* %in
  %hi = load i32, i32 addrspace(1)* %in.gep.1

  store i32 %lo, i32 addrspace(1)* %out
  store i32 %hi, i32 addrspace(1)* %out.gep.1
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_2_adjacent_loads_i32_nonzero_base:
; GCN: buffer_load_dwordx2 [[LOAD:v\[[0-9]+:[0-9]+\]]], s{{\[[0-9]+:[0-9]+\]}}, 0 offset:8
; GCN: buffer_store_dwordx2 [[LOAD]], s{{\[[0-9]+:[0-9]+\]}}, 0 offset:8
define void @merge_global_store_2_adjacent_loads_i32_nonzero_base(i32 addrspace(1)* %out, i32 addrspace(1)* %in) #0 {
  %in.gep.0 = getelementptr i32, i32 addrspace(1)* %in, i32 2
  %in.gep.1 = getelementptr i32, i32 addrspace(1)* %in, i32 3

  %out.gep.0 = getelementptr i32, i32 addrspace(1)* %out, i32 2
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 3
  %lo = load i32, i32 addrspace(1)* %in.gep.0
  %hi = load i32, i32 addrspace(1)* %in.gep.1

  store i32 %lo, i32 addrspace(1)* %out.gep.0
  store i32 %hi, i32 addrspace(1)* %out.gep.1
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_2_adjacent_loads_shuffle_i32:
; GCN: buffer_load_dword v
; GCN: buffer_load_dword v
; GCN: buffer_store_dword v
; GCN: buffer_store_dword v
define void @merge_global_store_2_adjacent_loads_shuffle_i32(i32 addrspace(1)* %out, i32 addrspace(1)* %in) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 1
  %in.gep.1 = getelementptr i32, i32 addrspace(1)* %in, i32 1

  %lo = load i32, i32 addrspace(1)* %in
  %hi = load i32, i32 addrspace(1)* %in.gep.1

  store i32 %hi, i32 addrspace(1)* %out
  store i32 %lo, i32 addrspace(1)* %out.gep.1
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_4_adjacent_loads_i32:
; GCN: buffer_load_dwordx4 [[LOAD:v\[[0-9]+:[0-9]+\]]]
; GCN: buffer_store_dwordx4 [[LOAD]]
define void @merge_global_store_4_adjacent_loads_i32(i32 addrspace(1)* %out, i32 addrspace(1)* %in) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 1
  %out.gep.2 = getelementptr i32, i32 addrspace(1)* %out, i32 2
  %out.gep.3 = getelementptr i32, i32 addrspace(1)* %out, i32 3
  %in.gep.1 = getelementptr i32, i32 addrspace(1)* %in, i32 1
  %in.gep.2 = getelementptr i32, i32 addrspace(1)* %in, i32 2
  %in.gep.3 = getelementptr i32, i32 addrspace(1)* %in, i32 3

  %x = load i32, i32 addrspace(1)* %in
  %y = load i32, i32 addrspace(1)* %in.gep.1
  %z = load i32, i32 addrspace(1)* %in.gep.2
  %w = load i32, i32 addrspace(1)* %in.gep.3

  store i32 %x, i32 addrspace(1)* %out
  store i32 %y, i32 addrspace(1)* %out.gep.1
  store i32 %z, i32 addrspace(1)* %out.gep.2
  store i32 %w, i32 addrspace(1)* %out.gep.3
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_3_adjacent_loads_i32:
; SI-DAG: buffer_load_dwordx2
; SI-DAG: buffer_load_dword v
; GCN: s_waitcnt
; SI-DAG: buffer_store_dword v
; SI-DAG: buffer_store_dwordx2 v
; GCN: s_endpgm
define void @merge_global_store_3_adjacent_loads_i32(i32 addrspace(1)* %out, i32 addrspace(1)* %in) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 1
  %out.gep.2 = getelementptr i32, i32 addrspace(1)* %out, i32 2
  %in.gep.1 = getelementptr i32, i32 addrspace(1)* %in, i32 1
  %in.gep.2 = getelementptr i32, i32 addrspace(1)* %in, i32 2

  %x = load i32, i32 addrspace(1)* %in
  %y = load i32, i32 addrspace(1)* %in.gep.1
  %z = load i32, i32 addrspace(1)* %in.gep.2

  store i32 %x, i32 addrspace(1)* %out
  store i32 %y, i32 addrspace(1)* %out.gep.1
  store i32 %z, i32 addrspace(1)* %out.gep.2
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_4_adjacent_loads_f32:
; GCN: buffer_load_dwordx4 [[LOAD:v\[[0-9]+:[0-9]+\]]]
; GCN: buffer_store_dwordx4 [[LOAD]]
define void @merge_global_store_4_adjacent_loads_f32(float addrspace(1)* %out, float addrspace(1)* %in) #0 {
  %out.gep.1 = getelementptr float, float addrspace(1)* %out, i32 1
  %out.gep.2 = getelementptr float, float addrspace(1)* %out, i32 2
  %out.gep.3 = getelementptr float, float addrspace(1)* %out, i32 3
  %in.gep.1 = getelementptr float, float addrspace(1)* %in, i32 1
  %in.gep.2 = getelementptr float, float addrspace(1)* %in, i32 2
  %in.gep.3 = getelementptr float, float addrspace(1)* %in, i32 3

  %x = load float, float addrspace(1)* %in
  %y = load float, float addrspace(1)* %in.gep.1
  %z = load float, float addrspace(1)* %in.gep.2
  %w = load float, float addrspace(1)* %in.gep.3

  store float %x, float addrspace(1)* %out
  store float %y, float addrspace(1)* %out.gep.1
  store float %z, float addrspace(1)* %out.gep.2
  store float %w, float addrspace(1)* %out.gep.3
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_4_adjacent_loads_i32_nonzero_base:
; GCN: buffer_load_dwordx4 [[LOAD:v\[[0-9]+:[0-9]+\]]], s{{\[[0-9]+:[0-9]+\]}}, 0 offset:44
; GCN: buffer_store_dwordx4 [[LOAD]], s{{\[[0-9]+:[0-9]+\]}}, 0 offset:28
define void @merge_global_store_4_adjacent_loads_i32_nonzero_base(i32 addrspace(1)* %out, i32 addrspace(1)* %in) #0 {
  %in.gep.0 = getelementptr i32, i32 addrspace(1)* %in, i32 11
  %in.gep.1 = getelementptr i32, i32 addrspace(1)* %in, i32 12
  %in.gep.2 = getelementptr i32, i32 addrspace(1)* %in, i32 13
  %in.gep.3 = getelementptr i32, i32 addrspace(1)* %in, i32 14
  %out.gep.0 = getelementptr i32, i32 addrspace(1)* %out, i32 7
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 8
  %out.gep.2 = getelementptr i32, i32 addrspace(1)* %out, i32 9
  %out.gep.3 = getelementptr i32, i32 addrspace(1)* %out, i32 10

  %x = load i32, i32 addrspace(1)* %in.gep.0
  %y = load i32, i32 addrspace(1)* %in.gep.1
  %z = load i32, i32 addrspace(1)* %in.gep.2
  %w = load i32, i32 addrspace(1)* %in.gep.3

  store i32 %x, i32 addrspace(1)* %out.gep.0
  store i32 %y, i32 addrspace(1)* %out.gep.1
  store i32 %z, i32 addrspace(1)* %out.gep.2
  store i32 %w, i32 addrspace(1)* %out.gep.3
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_4_adjacent_loads_inverse_i32:
; GCN: buffer_load_dwordx4 [[LOAD:v\[[0-9]+:[0-9]+\]]]
; GCN: s_barrier
; GCN: buffer_store_dwordx4 [[LOAD]]
define void @merge_global_store_4_adjacent_loads_inverse_i32(i32 addrspace(1)* %out, i32 addrspace(1)* %in) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 1
  %out.gep.2 = getelementptr i32, i32 addrspace(1)* %out, i32 2
  %out.gep.3 = getelementptr i32, i32 addrspace(1)* %out, i32 3
  %in.gep.1 = getelementptr i32, i32 addrspace(1)* %in, i32 1
  %in.gep.2 = getelementptr i32, i32 addrspace(1)* %in, i32 2
  %in.gep.3 = getelementptr i32, i32 addrspace(1)* %in, i32 3

  %x = load i32, i32 addrspace(1)* %in
  %y = load i32, i32 addrspace(1)* %in.gep.1
  %z = load i32, i32 addrspace(1)* %in.gep.2
  %w = load i32, i32 addrspace(1)* %in.gep.3

  ; Make sure the barrier doesn't stop this
  tail call void @llvm.AMDGPU.barrier.local() #1

  store i32 %w, i32 addrspace(1)* %out.gep.3
  store i32 %z, i32 addrspace(1)* %out.gep.2
  store i32 %y, i32 addrspace(1)* %out.gep.1
  store i32 %x, i32 addrspace(1)* %out

  ret void
}

; TODO: Re-packing of loaded register required. Maybe an IR pass
; should catch this?

; GCN-LABEL: {{^}}merge_global_store_4_adjacent_loads_shuffle_i32:
; GCN: buffer_load_dword v
; GCN: buffer_load_dword v
; GCN: buffer_load_dword v
; GCN: buffer_load_dword v
; GCN: s_barrier
; GCN: buffer_store_dword v
; GCN: buffer_store_dword v
; GCN: buffer_store_dword v
; GCN: buffer_store_dword v
define void @merge_global_store_4_adjacent_loads_shuffle_i32(i32 addrspace(1)* %out, i32 addrspace(1)* %in) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 1
  %out.gep.2 = getelementptr i32, i32 addrspace(1)* %out, i32 2
  %out.gep.3 = getelementptr i32, i32 addrspace(1)* %out, i32 3
  %in.gep.1 = getelementptr i32, i32 addrspace(1)* %in, i32 1
  %in.gep.2 = getelementptr i32, i32 addrspace(1)* %in, i32 2
  %in.gep.3 = getelementptr i32, i32 addrspace(1)* %in, i32 3

  %x = load i32, i32 addrspace(1)* %in
  %y = load i32, i32 addrspace(1)* %in.gep.1
  %z = load i32, i32 addrspace(1)* %in.gep.2
  %w = load i32, i32 addrspace(1)* %in.gep.3

  ; Make sure the barrier doesn't stop this
  tail call void @llvm.AMDGPU.barrier.local() #1

  store i32 %w, i32 addrspace(1)* %out
  store i32 %z, i32 addrspace(1)* %out.gep.1
  store i32 %y, i32 addrspace(1)* %out.gep.2
  store i32 %x, i32 addrspace(1)* %out.gep.3

  ret void
}

; GCN-LABEL: {{^}}merge_global_store_4_adjacent_loads_i8:
; GCN: buffer_load_dword [[LOAD:v[0-9]+]]
; GCN: buffer_store_dword [[LOAD]]
; GCN: s_endpgm
define void @merge_global_store_4_adjacent_loads_i8(i8 addrspace(1)* %out, i8 addrspace(1)* %in) #0 {
  %out.gep.1 = getelementptr i8, i8 addrspace(1)* %out, i8 1
  %out.gep.2 = getelementptr i8, i8 addrspace(1)* %out, i8 2
  %out.gep.3 = getelementptr i8, i8 addrspace(1)* %out, i8 3
  %in.gep.1 = getelementptr i8, i8 addrspace(1)* %in, i8 1
  %in.gep.2 = getelementptr i8, i8 addrspace(1)* %in, i8 2
  %in.gep.3 = getelementptr i8, i8 addrspace(1)* %in, i8 3

  %x = load i8, i8 addrspace(1)* %in, align 4
  %y = load i8, i8 addrspace(1)* %in.gep.1
  %z = load i8, i8 addrspace(1)* %in.gep.2
  %w = load i8, i8 addrspace(1)* %in.gep.3

  store i8 %x, i8 addrspace(1)* %out, align 4
  store i8 %y, i8 addrspace(1)* %out.gep.1
  store i8 %z, i8 addrspace(1)* %out.gep.2
  store i8 %w, i8 addrspace(1)* %out.gep.3
  ret void
}

; GCN-LABEL: {{^}}merge_global_store_4_adjacent_loads_i8_natural_align:
; GCN: buffer_load_ubyte
; GCN: buffer_load_ubyte
; GCN: buffer_load_ubyte
; GCN: buffer_load_ubyte
; GCN: buffer_store_byte
; GCN: buffer_store_byte
; GCN: buffer_store_byte
; GCN: buffer_store_byte
; GCN: s_endpgm
define void @merge_global_store_4_adjacent_loads_i8_natural_align(i8 addrspace(1)* %out, i8 addrspace(1)* %in) #0 {
  %out.gep.1 = getelementptr i8, i8 addrspace(1)* %out, i8 1
  %out.gep.2 = getelementptr i8, i8 addrspace(1)* %out, i8 2
  %out.gep.3 = getelementptr i8, i8 addrspace(1)* %out, i8 3
  %in.gep.1 = getelementptr i8, i8 addrspace(1)* %in, i8 1
  %in.gep.2 = getelementptr i8, i8 addrspace(1)* %in, i8 2
  %in.gep.3 = getelementptr i8, i8 addrspace(1)* %in, i8 3

  %x = load i8, i8 addrspace(1)* %in
  %y = load i8, i8 addrspace(1)* %in.gep.1
  %z = load i8, i8 addrspace(1)* %in.gep.2
  %w = load i8, i8 addrspace(1)* %in.gep.3

  store i8 %x, i8 addrspace(1)* %out
  store i8 %y, i8 addrspace(1)* %out.gep.1
  store i8 %z, i8 addrspace(1)* %out.gep.2
  store i8 %w, i8 addrspace(1)* %out.gep.3
  ret void
}

; This works once AA is enabled on the subtarget
; GCN-LABEL: {{^}}merge_global_store_4_vector_elts_loads_v4i32:
; GCN: buffer_load_dwordx4 [[LOAD:v\[[0-9]+:[0-9]+\]]]
; XGCN: buffer_store_dwordx4 [[LOAD]]
; GCN: buffer_store_dword v
; GCN: buffer_store_dword v
; GCN: buffer_store_dword v
; GCN: buffer_store_dword v
define void @merge_global_store_4_vector_elts_loads_v4i32(i32 addrspace(1)* %out, <4 x i32> addrspace(1)* %in) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(1)* %out, i32 1
  %out.gep.2 = getelementptr i32, i32 addrspace(1)* %out, i32 2
  %out.gep.3 = getelementptr i32, i32 addrspace(1)* %out, i32 3
  %vec = load <4 x i32>, <4 x i32> addrspace(1)* %in

  %x = extractelement <4 x i32> %vec, i32 0
  %y = extractelement <4 x i32> %vec, i32 1
  %z = extractelement <4 x i32> %vec, i32 2
  %w = extractelement <4 x i32> %vec, i32 3

  store i32 %x, i32 addrspace(1)* %out
  store i32 %y, i32 addrspace(1)* %out.gep.1
  store i32 %z, i32 addrspace(1)* %out.gep.2
  store i32 %w, i32 addrspace(1)* %out.gep.3
  ret void
}

; GCN-LABEL: {{^}}merge_local_store_2_constants_i8:
; GCN: ds_write_b8
; GCN: ds_write_b8
; GCN: s_endpgm
define void @merge_local_store_2_constants_i8(i8 addrspace(3)* %out) #0 {
  %out.gep.1 = getelementptr i8, i8 addrspace(3)* %out, i32 1

  store i8 123, i8 addrspace(3)* %out.gep.1
  store i8 456, i8 addrspace(3)* %out, align 2
  ret void
}

; GCN-LABEL: {{^}}merge_local_store_2_constants_i32:
; GCN-DAG: s_movk_i32 [[SLO:s[0-9]+]], 0x1c8
; GCN-DAG: s_movk_i32 [[SHI:s[0-9]+]], 0x7b
; GCN-DAG: v_mov_b32_e32 v[[LO:[0-9]+]], [[SLO]]
; GCN-DAG: v_mov_b32_e32 v[[HI:[0-9]+]], [[SHI]]
; GCN: ds_write2_b32 v{{[0-9]+}}, v[[LO]], v[[HI]] offset1:1{{$}}
define void @merge_local_store_2_constants_i32(i32 addrspace(3)* %out) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(3)* %out, i32 1

  store i32 123, i32 addrspace(3)* %out.gep.1
  store i32 456, i32 addrspace(3)* %out
  ret void
}

; GCN-LABEL: {{^}}merge_local_store_4_constants_i32:
; GCN: ds_write_b32
; GCN: ds_write_b32
; GCN: ds_write_b32
; GCN: ds_write_b32
define void @merge_local_store_4_constants_i32(i32 addrspace(3)* %out) #0 {
  %out.gep.1 = getelementptr i32, i32 addrspace(3)* %out, i32 1
  %out.gep.2 = getelementptr i32, i32 addrspace(3)* %out, i32 2
  %out.gep.3 = getelementptr i32, i32 addrspace(3)* %out, i32 3

  store i32 123, i32 addrspace(3)* %out.gep.1
  store i32 456, i32 addrspace(3)* %out.gep.2
  store i32 333, i32 addrspace(3)* %out.gep.3
  store i32 1234, i32 addrspace(3)* %out
  ret void
}

declare void @llvm.AMDGPU.barrier.local() #1

attributes #0 = { nounwind }
attributes #1 = { noduplicate nounwind }
