	.text
	.amdgcn_target "amdgcn-amd-amdhsa--gfx700"
	.globl	add_nums                        ; -- Begin function add_nums
	.p2align	8
	.type	add_nums,@function
add_nums:                               ; @add_nums
add_nums$local:
; %bb.0:
	s_mov_b32 flat_scratch_lo, s7
	s_add_i32 s6, s6, s9
	s_lshr_b32 flat_scratch_hi, s6, 8
	s_add_u32 s0, s0, s9
	s_addc_u32 s1, s1, 0
	s_load_dwordx2 s[4:5], s[4:5], 0x0
	s_waitcnt lgkmcnt(0)
	v_writelane_b32 v8, s4, 0
	v_writelane_b32 v8, s5, 1
	s_mov_b64 s[6:7], 0
	s_mov_b32 s5, s7
	v_mov_b32_e32 v0, s5
	buffer_store_dword v0, off, s[0:3], 0 offset:12
	s_mov_b32 s4, s6
	v_mov_b32_e32 v0, s4
	buffer_store_dword v0, off, s[0:3], 0 offset:8
	v_mov_b32_e32 v0, s5
	buffer_store_dword v0, off, s[0:3], 0 offset:20
	v_mov_b32_e32 v0, s4
	buffer_store_dword v0, off, s[0:3], 0 offset:16
; %bb.1:
	v_readlane_b32 s6, v8, 0
	v_readlane_b32 s7, v8, 1
	s_mov_b32 s5, s7
	v_mov_b32_e32 v0, s5
	buffer_store_dword v0, off, s[0:3], 0 offset:12
	s_mov_b32 s4, s6
	v_mov_b32_e32 v0, s4
	buffer_store_dword v0, off, s[0:3], 0 offset:8
	v_mov_b32_e32 v0, s5
	buffer_store_dword v0, off, s[0:3], 0 offset:20
	v_mov_b32_e32 v0, s4
	buffer_store_dword v0, off, s[0:3], 0 offset:16
	buffer_load_dword v0, off, s[0:3], 0 offset:8
	buffer_load_dword v2, off, s[0:3], 0 offset:12
                                        ; kill: def $vgpr0 killed $vgpr0 def $vgpr0_vgpr1 killed $exec
	s_waitcnt vmcnt(0)
	v_mov_b32_e32 v1, v2
	buffer_load_dword v2, off, s[0:3], 0 offset:16
	buffer_load_dword v4, off, s[0:3], 0 offset:20
                                        ; kill: def $vgpr2 killed $vgpr2 def $vgpr2_vgpr3 killed $exec
	s_waitcnt vmcnt(0)
	v_mov_b32_e32 v3, v4
	s_mov_b64 s[6:7], 4
	v_mov_b32_e32 v4, v2
	s_mov_b32 s4, s6
	v_mov_b32_e32 v5, v3
	s_mov_b32 s8, s7
	v_add_i32_e64 v4, s[4:5], v4, s4
	v_mov_b32_e32 v6, s8
	v_addc_u32_e64 v6, s[4:5], v5, v6, s[4:5]
                                        ; kill: def $vgpr4 killed $vgpr4 def $vgpr4_vgpr5 killed $exec
	v_mov_b32_e32 v5, v6
	flat_load_dword v4, v[4:5]
	flat_load_dword v2, v[2:3]
                                        ; kill: def $vgpr2 killed $vgpr2 def $vgpr2_vgpr3 killed $exec
	s_waitcnt vmcnt(0) lgkmcnt(0)
	v_mov_b32_e32 v3, v4
	s_mov_b32 s8, 0x40240000
	s_mov_b32 s4, 0
                                        ; kill: def $sgpr4 killed $sgpr4 def $sgpr4_sgpr5
	s_mov_b32 s5, s8
	v_add_f64 v[2:3], v[2:3], s[4:5]
	v_mov_b32_e32 v6, v3
	v_mov_b32_e32 v4, v0
	s_mov_b32 s4, s6
	v_mov_b32_e32 v5, v1
	s_mov_b32 s6, s7
	v_add_i32_e64 v4, s[4:5], v4, s4
	v_mov_b32_e32 v7, s6
	v_addc_u32_e64 v7, s[4:5], v5, v7, s[4:5]
                                        ; kill: def $vgpr4 killed $vgpr4 def $vgpr4_vgpr5 killed $exec
	v_mov_b32_e32 v5, v7
	flat_store_dword v[4:5], v6
                                        ; kill: def $vgpr2 killed $vgpr2 killed $vgpr2_vgpr3 killed $exec
	flat_store_dword v[0:1], v2
	s_endpgm
	.section	.rodata,#alloc
	.p2align	6
	.amdhsa_kernel add_nums
		.amdhsa_group_segment_fixed_size 0
		.amdhsa_private_segment_fixed_size 24
		.amdhsa_kernarg_size 8
		.amdhsa_user_sgpr_count 8
		.amdhsa_user_sgpr_private_segment_buffer 1
		.amdhsa_user_sgpr_dispatch_ptr 0
		.amdhsa_user_sgpr_queue_ptr 0
		.amdhsa_user_sgpr_kernarg_segment_ptr 1
		.amdhsa_user_sgpr_dispatch_id 0
		.amdhsa_user_sgpr_flat_scratch_init 1
		.amdhsa_user_sgpr_private_segment_size 0
		.amdhsa_system_sgpr_private_segment_wavefront_offset 1
		.amdhsa_system_sgpr_workgroup_id_x 1
		.amdhsa_system_sgpr_workgroup_id_y 0
		.amdhsa_system_sgpr_workgroup_id_z 0
		.amdhsa_system_sgpr_workgroup_info 0
		.amdhsa_system_vgpr_workitem_id 0
		.amdhsa_next_free_vgpr 9
		.amdhsa_next_free_sgpr 10
		.amdhsa_reserve_vcc 0
		.amdhsa_float_round_mode_32 0
		.amdhsa_float_round_mode_16_64 0
		.amdhsa_float_denorm_mode_32 3
		.amdhsa_float_denorm_mode_16_64 3
		.amdhsa_dx10_clamp 1
		.amdhsa_ieee_mode 1
		.amdhsa_exception_fp_ieee_invalid_op 0
		.amdhsa_exception_fp_denorm_src 0
		.amdhsa_exception_fp_ieee_div_zero 0
		.amdhsa_exception_fp_ieee_overflow 0
		.amdhsa_exception_fp_ieee_underflow 0
		.amdhsa_exception_fp_ieee_inexact 0
		.amdhsa_exception_int_div_zero 0
	.end_amdhsa_kernel
	.text
.Lfunc_end0:
	.size	add_nums, .Lfunc_end0-add_nums
                                        ; -- End function
	.section	.AMDGPU.csdata
; Kernel info:
; codeLenInByte = 384
; NumSgprs: 14
; NumVgprs: 9
; ScratchSize: 24
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 0 bytes/workgroup (compile time only)
; SGPRBlocks: 1
; VGPRBlocks: 2
; NumSGPRsForWavesPerEU: 14
; NumVGPRsForWavesPerEU: 9
; Occupancy: 10
; WaveLimiterHint : 0
; COMPUTE_PGM_RSRC2:SCRATCH_EN: 1
; COMPUTE_PGM_RSRC2:USER_SGPR: 8
; COMPUTE_PGM_RSRC2:TRAP_HANDLER: 0
; COMPUTE_PGM_RSRC2:TGID_X_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Y_EN: 0
; COMPUTE_PGM_RSRC2:TGID_Z_EN: 0
; COMPUTE_PGM_RSRC2:TIDIG_COMP_CNT: 0
	.type	chpl__cid_object_chpl,@object   ; @chpl__cid_object_chpl
	.section	.rodata,#alloc
	.p2align	2
chpl__cid_object_chpl:
	.long	1                               ; 0x1
	.size	chpl__cid_object_chpl, 4

	.type	chpl__cid_chpl___EndCountBase,@object ; @chpl__cid_chpl___EndCountBase
	.p2align	2
chpl__cid_chpl___EndCountBase:
	.long	2                               ; 0x2
	.size	chpl__cid_chpl___EndCountBase, 4

	.type	chpl__cid_chpl___EndCount_AtomicT_int64_t_int64_t,@object ; @chpl__cid_chpl___EndCount_AtomicT_int64_t_int64_t
	.p2align	2
chpl__cid_chpl___EndCount_AtomicT_int64_t_int64_t:
	.long	3                               ; 0x3
	.size	chpl__cid_chpl___EndCount_AtomicT_int64_t_int64_t, 4

	.type	chpl__cid_chpl_ModuleDeinit,@object ; @chpl__cid_chpl_ModuleDeinit
	.p2align	2
chpl__cid_chpl_ModuleDeinit:
	.long	4                               ; 0x4
	.size	chpl__cid_chpl_ModuleDeinit, 4

	.type	chpl__cid_BaseDist_chpl,@object ; @chpl__cid_BaseDist_chpl
	.p2align	2
chpl__cid_BaseDist_chpl:
	.long	5                               ; 0x5
	.size	chpl__cid_BaseDist_chpl, 4

	.type	chpl__cid_BaseDom_chpl,@object  ; @chpl__cid_BaseDom_chpl
	.p2align	2
chpl__cid_BaseDom_chpl:
	.long	7                               ; 0x7
	.size	chpl__cid_BaseDom_chpl, 4

	.type	chpl__cid_BaseRectangularDom_1_int64_t_F_chpl,@object ; @chpl__cid_BaseRectangularDom_1_int64_t_F_chpl
	.p2align	2
chpl__cid_BaseRectangularDom_1_int64_t_F_chpl:
	.long	8                               ; 0x8
	.size	chpl__cid_BaseRectangularDom_1_int64_t_F_chpl, 4

	.type	chpl__cid_BaseArr_chpl,@object  ; @chpl__cid_BaseArr_chpl
	.p2align	2
chpl__cid_BaseArr_chpl:
	.long	10                              ; 0xa
	.size	chpl__cid_BaseArr_chpl, 4

	.type	chpl__cid_BaseArrOverRectangularDom_1_int64_t_F_chpl,@object ; @chpl__cid_BaseArrOverRectangularDom_1_int64_t_F_chpl
	.p2align	2
chpl__cid_BaseArrOverRectangularDom_1_int64_t_F_chpl:
	.long	11                              ; 0xb
	.size	chpl__cid_BaseArrOverRectangularDom_1_int64_t_F_chpl, 4

	.type	chpl__cid_BaseRectangularArr_1_int64_t_F_AbstractLocaleModel_chpl,@object ; @chpl__cid_BaseRectangularArr_1_int64_t_F_AbstractLocaleModel_chpl
	.p2align	2
chpl__cid_BaseRectangularArr_1_int64_t_F_AbstractLocaleModel_chpl:
	.long	12                              ; 0xc
	.size	chpl__cid_BaseRectangularArr_1_int64_t_F_AbstractLocaleModel_chpl, 4

	.type	chpl__cid_BaseRectangularArr_1_int64_t_F_int64_t_chpl,@object ; @chpl__cid_BaseRectangularArr_1_int64_t_F_int64_t_chpl
	.p2align	2
chpl__cid_BaseRectangularArr_1_int64_t_F_int64_t_chpl:
	.long	14                              ; 0xe
	.size	chpl__cid_BaseRectangularArr_1_int64_t_F_int64_t_chpl, 4

	.type	chpl__cid_BaseRectangularArr_1_int64_t_F_locale_chpl,@object ; @chpl__cid_BaseRectangularArr_1_int64_t_F_locale_chpl
	.p2align	2
chpl__cid_BaseRectangularArr_1_int64_t_F_locale_chpl:
	.long	16                              ; 0x10
	.size	chpl__cid_BaseRectangularArr_1_int64_t_F_locale_chpl, 4

	.type	chpl__cid_BaseRectangularArr_1_int64_t_F_uint64_t_chpl,@object ; @chpl__cid_BaseRectangularArr_1_int64_t_F_uint64_t_chpl
	.p2align	2
chpl__cid_BaseRectangularArr_1_int64_t_F_uint64_t_chpl:
	.long	18                              ; 0x12
	.size	chpl__cid_BaseRectangularArr_1_int64_t_F_uint64_t_chpl, 4

	.type	chpl__cid_BaseLocale_chpl,@object ; @chpl__cid_BaseLocale_chpl
	.p2align	2
chpl__cid_BaseLocale_chpl:
	.long	20                              ; 0x14
	.size	chpl__cid_BaseLocale_chpl, 4

	.type	chpl__cid_DummyLocale_chpl,@object ; @chpl__cid_DummyLocale_chpl
	.p2align	2
chpl__cid_DummyLocale_chpl:
	.long	21                              ; 0x15
	.size	chpl__cid_DummyLocale_chpl, 4

	.type	chpl__cid_AbstractLocaleModel_chpl,@object ; @chpl__cid_AbstractLocaleModel_chpl
	.p2align	2
chpl__cid_AbstractLocaleModel_chpl:
	.long	22                              ; 0x16
	.size	chpl__cid_AbstractLocaleModel_chpl, 4

	.type	chpl__cid_AbstractRootLocale_chpl,@object ; @chpl__cid_AbstractRootLocale_chpl
	.p2align	2
chpl__cid_AbstractRootLocale_chpl:
	.long	25                              ; 0x19
	.size	chpl__cid_AbstractRootLocale_chpl, 4

	.type	chpl__cid__ic_chpl_direct_counted_range_iter_chpl,@object ; @chpl__cid__ic_chpl_direct_counted_range_iter_chpl
	.p2align	2
chpl__cid__ic_chpl_direct_counted_range_iter_chpl:
	.long	27                              ; 0x1b
	.size	chpl__cid__ic_chpl_direct_counted_range_iter_chpl, 4

	.type	chpl__cid__ic_chpl_direct_counted_range_iter_helper_chpl,@object ; @chpl__cid__ic_chpl_direct_counted_range_iter_helper_chpl
	.p2align	2
chpl__cid__ic_chpl_direct_counted_range_iter_helper_chpl:
	.long	28                              ; 0x1c
	.size	chpl__cid__ic_chpl_direct_counted_range_iter_helper_chpl, 4

	.type	chpl__cid__ic_chpl_direct_param_stride_range_iter_chpl,@object ; @chpl__cid__ic_chpl_direct_param_stride_range_iter_chpl
	.p2align	2
chpl__cid__ic_chpl_direct_param_stride_range_iter_chpl:
	.long	29                              ; 0x1d
	.size	chpl__cid__ic_chpl_direct_param_stride_range_iter_chpl, 4

	.type	chpl__cid__ic_these_range_int64_t_bounded_F_chpl,@object ; @chpl__cid__ic_these_range_int64_t_bounded_F_chpl
	.p2align	2
chpl__cid__ic_these_range_int64_t_bounded_F_chpl:
	.long	30                              ; 0x1e
	.size	chpl__cid__ic_these_range_int64_t_bounded_F_chpl, 4

	.type	chpl__cid_DefaultDist_chpl,@object ; @chpl__cid_DefaultDist_chpl
	.p2align	2
chpl__cid_DefaultDist_chpl:
	.long	6                               ; 0x6
	.size	chpl__cid_DefaultDist_chpl, 4

	.type	chpl__cid_DefaultRectangularDom_1_int64_t_F_chpl,@object ; @chpl__cid_DefaultRectangularDom_1_int64_t_F_chpl
	.p2align	2
chpl__cid_DefaultRectangularDom_1_int64_t_F_chpl:
	.long	9                               ; 0x9
	.size	chpl__cid_DefaultRectangularDom_1_int64_t_F_chpl, 4

	.type	chpl__cid_DefaultRectangularArr_1_int64_t_F_AbstractLocaleModel_int64_t_chpl,@object ; @chpl__cid_DefaultRectangularArr_1_int64_t_F_AbstractLocaleModel_int64_t_chpl
	.p2align	2
chpl__cid_DefaultRectangularArr_1_int64_t_F_AbstractLocaleModel_int64_t_chpl:
	.long	13                              ; 0xd
	.size	chpl__cid_DefaultRectangularArr_1_int64_t_F_AbstractLocaleModel_int64_t_chpl, 4

	.type	chpl__cid_DefaultRectangularArr_1_int64_t_F_int64_t_int64_t_chpl,@object ; @chpl__cid_DefaultRectangularArr_1_int64_t_F_int64_t_int64_t_chpl
	.p2align	2
chpl__cid_DefaultRectangularArr_1_int64_t_F_int64_t_int64_t_chpl:
	.long	15                              ; 0xf
	.size	chpl__cid_DefaultRectangularArr_1_int64_t_F_int64_t_int64_t_chpl, 4

	.type	chpl__cid_DefaultRectangularArr_1_int64_t_F_locale_int64_t_chpl,@object ; @chpl__cid_DefaultRectangularArr_1_int64_t_F_locale_int64_t_chpl
	.p2align	2
chpl__cid_DefaultRectangularArr_1_int64_t_F_locale_int64_t_chpl:
	.long	17                              ; 0x11
	.size	chpl__cid_DefaultRectangularArr_1_int64_t_F_locale_int64_t_chpl, 4

	.type	chpl__cid_DefaultRectangularArr_1_int64_t_F_uint64_t_int64_t_chpl,@object ; @chpl__cid_DefaultRectangularArr_1_int64_t_F_uint64_t_int64_t_chpl
	.p2align	2
chpl__cid_DefaultRectangularArr_1_int64_t_F_uint64_t_int64_t_chpl:
	.long	19                              ; 0x13
	.size	chpl__cid_DefaultRectangularArr_1_int64_t_F_uint64_t_int64_t_chpl, 4

	.type	chpl__cid_GPULocale_chpl,@object ; @chpl__cid_GPULocale_chpl
	.p2align	2
chpl__cid_GPULocale_chpl:
	.long	23                              ; 0x17
	.size	chpl__cid_GPULocale_chpl, 4

	.type	chpl__cid_LocaleModel_0_chpl,@object ; @chpl__cid_LocaleModel_0_chpl
	.p2align	2
chpl__cid_LocaleModel_0_chpl:
	.long	24                              ; 0x18
	.size	chpl__cid_LocaleModel_0_chpl, 4

	.type	chpl__cid_RootLocale_chpl,@object ; @chpl__cid_RootLocale_chpl
	.p2align	2
chpl__cid_RootLocale_chpl:
	.long	26                              ; 0x1a
	.size	chpl__cid_RootLocale_chpl, 4

	.type	chpl__cid__ic_chpl_bytes__ref_string_chpl,@object ; @chpl__cid__ic_chpl_bytes__ref_string_chpl
	.p2align	2
chpl__cid__ic_chpl_bytes__ref_string_chpl:
	.long	31                              ; 0x1f
	.size	chpl__cid__ic_chpl_bytes__ref_string_chpl, 4

	.type	chpl__cid_Error_chpl,@object    ; @chpl__cid_Error_chpl
	.p2align	2
chpl__cid_Error_chpl:
	.long	32                              ; 0x20
	.size	chpl__cid_Error_chpl, 4

	.type	chpl__cid_NilThrownError_chpl,@object ; @chpl__cid_NilThrownError_chpl
	.p2align	2
chpl__cid_NilThrownError_chpl:
	.long	33                              ; 0x21
	.size	chpl__cid_NilThrownError_chpl, 4

	.type	chpl__cid_DecodeError_chpl,@object ; @chpl__cid_DecodeError_chpl
	.p2align	2
chpl__cid_DecodeError_chpl:
	.long	34                              ; 0x22
	.size	chpl__cid_DecodeError_chpl, 4

	.type	chpl__cid_IllegalArgumentError_chpl,@object ; @chpl__cid_IllegalArgumentError_chpl
	.p2align	2
chpl__cid_IllegalArgumentError_chpl:
	.long	35                              ; 0x23
	.size	chpl__cid_IllegalArgumentError_chpl, 4

	.type	chpl__cid_CodepointSplittingError_chpl,@object ; @chpl__cid_CodepointSplittingError_chpl
	.p2align	2
chpl__cid_CodepointSplittingError_chpl:
	.long	36                              ; 0x24
	.size	chpl__cid_CodepointSplittingError_chpl, 4

	.type	chpl__cid_TaskErrors_chpl,@object ; @chpl__cid_TaskErrors_chpl
	.p2align	2
chpl__cid_TaskErrors_chpl:
	.long	37                              ; 0x25
	.size	chpl__cid_TaskErrors_chpl, 4

	.type	chpl__cid_QioPluginFile_chpl,@object ; @chpl__cid_QioPluginFile_chpl
	.p2align	2
chpl__cid_QioPluginFile_chpl:
	.long	59                              ; 0x3b
	.size	chpl__cid_QioPluginFile_chpl, 4

	.type	chpl__cid_QioPluginChannel_chpl,@object ; @chpl__cid_QioPluginChannel_chpl
	.p2align	2
chpl__cid_QioPluginChannel_chpl:
	.long	60                              ; 0x3c
	.size	chpl__cid_QioPluginChannel_chpl, 4

	.type	chpl__cid_SystemError_chpl,@object ; @chpl__cid_SystemError_chpl
	.p2align	2
chpl__cid_SystemError_chpl:
	.long	38                              ; 0x26
	.size	chpl__cid_SystemError_chpl, 4

	.type	chpl__cid_BlockingIOError_chpl,@object ; @chpl__cid_BlockingIOError_chpl
	.p2align	2
chpl__cid_BlockingIOError_chpl:
	.long	39                              ; 0x27
	.size	chpl__cid_BlockingIOError_chpl, 4

	.type	chpl__cid_ChildProcessError_chpl,@object ; @chpl__cid_ChildProcessError_chpl
	.p2align	2
chpl__cid_ChildProcessError_chpl:
	.long	40                              ; 0x28
	.size	chpl__cid_ChildProcessError_chpl, 4

	.type	chpl__cid_ConnectionError_chpl,@object ; @chpl__cid_ConnectionError_chpl
	.p2align	2
chpl__cid_ConnectionError_chpl:
	.long	41                              ; 0x29
	.size	chpl__cid_ConnectionError_chpl, 4

	.type	chpl__cid_BrokenPipeError_chpl,@object ; @chpl__cid_BrokenPipeError_chpl
	.p2align	2
chpl__cid_BrokenPipeError_chpl:
	.long	42                              ; 0x2a
	.size	chpl__cid_BrokenPipeError_chpl, 4

	.type	chpl__cid_ConnectionAbortedError_chpl,@object ; @chpl__cid_ConnectionAbortedError_chpl
	.p2align	2
chpl__cid_ConnectionAbortedError_chpl:
	.long	43                              ; 0x2b
	.size	chpl__cid_ConnectionAbortedError_chpl, 4

	.type	chpl__cid_ConnectionRefusedError_chpl,@object ; @chpl__cid_ConnectionRefusedError_chpl
	.p2align	2
chpl__cid_ConnectionRefusedError_chpl:
	.long	44                              ; 0x2c
	.size	chpl__cid_ConnectionRefusedError_chpl, 4

	.type	chpl__cid_ConnectionResetError_chpl,@object ; @chpl__cid_ConnectionResetError_chpl
	.p2align	2
chpl__cid_ConnectionResetError_chpl:
	.long	45                              ; 0x2d
	.size	chpl__cid_ConnectionResetError_chpl, 4

	.type	chpl__cid_FileExistsError_chpl,@object ; @chpl__cid_FileExistsError_chpl
	.p2align	2
chpl__cid_FileExistsError_chpl:
	.long	46                              ; 0x2e
	.size	chpl__cid_FileExistsError_chpl, 4

	.type	chpl__cid_FileNotFoundError_chpl,@object ; @chpl__cid_FileNotFoundError_chpl
	.p2align	2
chpl__cid_FileNotFoundError_chpl:
	.long	47                              ; 0x2f
	.size	chpl__cid_FileNotFoundError_chpl, 4

	.type	chpl__cid_InterruptedError_chpl,@object ; @chpl__cid_InterruptedError_chpl
	.p2align	2
chpl__cid_InterruptedError_chpl:
	.long	48                              ; 0x30
	.size	chpl__cid_InterruptedError_chpl, 4

	.type	chpl__cid_IsADirectoryError_chpl,@object ; @chpl__cid_IsADirectoryError_chpl
	.p2align	2
chpl__cid_IsADirectoryError_chpl:
	.long	49                              ; 0x31
	.size	chpl__cid_IsADirectoryError_chpl, 4

	.type	chpl__cid_NotADirectoryError_chpl,@object ; @chpl__cid_NotADirectoryError_chpl
	.p2align	2
chpl__cid_NotADirectoryError_chpl:
	.long	50                              ; 0x32
	.size	chpl__cid_NotADirectoryError_chpl, 4

	.type	chpl__cid_PermissionError_chpl,@object ; @chpl__cid_PermissionError_chpl
	.p2align	2
chpl__cid_PermissionError_chpl:
	.long	51                              ; 0x33
	.size	chpl__cid_PermissionError_chpl, 4

	.type	chpl__cid_ProcessLookupError_chpl,@object ; @chpl__cid_ProcessLookupError_chpl
	.p2align	2
chpl__cid_ProcessLookupError_chpl:
	.long	52                              ; 0x34
	.size	chpl__cid_ProcessLookupError_chpl, 4

	.type	chpl__cid_TimeoutError_chpl,@object ; @chpl__cid_TimeoutError_chpl
	.p2align	2
chpl__cid_TimeoutError_chpl:
	.long	53                              ; 0x35
	.size	chpl__cid_TimeoutError_chpl, 4

	.type	chpl__cid_IOError_chpl,@object  ; @chpl__cid_IOError_chpl
	.p2align	2
chpl__cid_IOError_chpl:
	.long	54                              ; 0x36
	.size	chpl__cid_IOError_chpl, 4

	.type	chpl__cid_EOFError_chpl,@object ; @chpl__cid_EOFError_chpl
	.p2align	2
chpl__cid_EOFError_chpl:
	.long	55                              ; 0x37
	.size	chpl__cid_EOFError_chpl, 4

	.type	chpl__cid_UnexpectedEOFError_chpl,@object ; @chpl__cid_UnexpectedEOFError_chpl
	.p2align	2
chpl__cid_UnexpectedEOFError_chpl:
	.long	56                              ; 0x38
	.size	chpl__cid_UnexpectedEOFError_chpl, 4

	.type	chpl__cid_BadFormatError_chpl,@object ; @chpl__cid_BadFormatError_chpl
	.p2align	2
chpl__cid_BadFormatError_chpl:
	.long	57                              ; 0x39
	.size	chpl__cid_BadFormatError_chpl, 4

	.type	chpl__cid_BadRegexError_chpl,@object ; @chpl__cid_BadRegexError_chpl
	.p2align	2
chpl__cid_BadRegexError_chpl:
	.long	58                              ; 0x3a
	.size	chpl__cid_BadRegexError_chpl, 4

	.type	chpl_numGlobalsOnHeap,@object   ; @chpl_numGlobalsOnHeap
	.p2align	2
chpl_numGlobalsOnHeap:
	.long	0                               ; 0x0
	.size	chpl_numGlobalsOnHeap, 4

	.type	chpl_globals_registry,@object   ; @chpl_globals_registry
	.local	chpl_globals_registry
	.comm	chpl_globals_registry,8,3
	.type	__unnamed_1,@object             ; @0
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_1:
	.asciz	"DecodeError"
	.size	__unnamed_1, 12

	.type	_cstr_literal_4266,@object      ; @_cstr_literal_4266
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4266:
	.quad	__unnamed_1
	.size	_cstr_literal_4266, 8

	.type	__unnamed_2,@object             ; @1
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_2:
	.asciz	"NilThrownError"
	.size	__unnamed_2, 15

	.type	_cstr_literal_4267,@object      ; @_cstr_literal_4267
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4267:
	.quad	__unnamed_2
	.size	_cstr_literal_4267, 8

	.type	__unnamed_3,@object             ; @2
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_3:
	.asciz	"_ic_chpl_direct_param_stride_range_iter"
	.size	__unnamed_3, 40

	.type	_cstr_literal_4268,@object      ; @_cstr_literal_4268
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4268:
	.quad	__unnamed_3
	.size	_cstr_literal_4268, 8

	.type	__unnamed_4,@object             ; @3
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_4:
	.asciz	"_ic_chpl_direct_counted_range_iter_helper"
	.size	__unnamed_4, 42

	.type	_cstr_literal_4269,@object      ; @_cstr_literal_4269
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4269:
	.quad	__unnamed_4
	.size	_cstr_literal_4269, 8

	.type	__unnamed_5,@object             ; @4
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_5:
	.asciz	"_ic_chpl_direct_counted_range_iter"
	.size	__unnamed_5, 35

	.type	_cstr_literal_4270,@object      ; @_cstr_literal_4270
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4270:
	.quad	__unnamed_5
	.size	_cstr_literal_4270, 8

	.type	__unnamed_6,@object             ; @5
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_6:
	.asciz	"_EndCount(AtomicT(int(64)),int(64))"
	.size	__unnamed_6, 36

	.type	_cstr_literal_4271,@object      ; @_cstr_literal_4271
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4271:
	.quad	__unnamed_6
	.size	_cstr_literal_4271, 8

	.type	__unnamed_7,@object             ; @6
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_7:
	.asciz	"DummyLocale"
	.size	__unnamed_7, 12

	.type	_cstr_literal_4272,@object      ; @_cstr_literal_4272
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4272:
	.quad	__unnamed_7
	.size	_cstr_literal_4272, 8

	.type	__unnamed_8,@object             ; @7
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_8:
	.asciz	"BlockingIOError"
	.size	__unnamed_8, 16

	.type	_cstr_literal_4273,@object      ; @_cstr_literal_4273
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4273:
	.quad	__unnamed_8
	.size	_cstr_literal_4273, 8

	.type	__unnamed_9,@object             ; @8
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_9:
	.asciz	"ChildProcessError"
	.size	__unnamed_9, 18

	.type	_cstr_literal_4274,@object      ; @_cstr_literal_4274
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4274:
	.quad	__unnamed_9
	.size	_cstr_literal_4274, 8

	.type	__unnamed_10,@object            ; @9
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_10:
	.asciz	"BrokenPipeError"
	.size	__unnamed_10, 16

	.type	_cstr_literal_4275,@object      ; @_cstr_literal_4275
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4275:
	.quad	__unnamed_10
	.size	_cstr_literal_4275, 8

	.type	__unnamed_11,@object            ; @10
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_11:
	.asciz	"ConnectionAbortedError"
	.size	__unnamed_11, 23

	.type	_cstr_literal_4276,@object      ; @_cstr_literal_4276
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4276:
	.quad	__unnamed_11
	.size	_cstr_literal_4276, 8

	.type	__unnamed_12,@object            ; @11
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_12:
	.asciz	"ConnectionRefusedError"
	.size	__unnamed_12, 23

	.type	_cstr_literal_4277,@object      ; @_cstr_literal_4277
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4277:
	.quad	__unnamed_12
	.size	_cstr_literal_4277, 8

	.type	__unnamed_13,@object            ; @12
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_13:
	.asciz	"ConnectionResetError"
	.size	__unnamed_13, 21

	.type	_cstr_literal_4278,@object      ; @_cstr_literal_4278
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4278:
	.quad	__unnamed_13
	.size	_cstr_literal_4278, 8

	.type	__unnamed_14,@object            ; @13
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_14:
	.asciz	"FileExistsError"
	.size	__unnamed_14, 16

	.type	_cstr_literal_4279,@object      ; @_cstr_literal_4279
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4279:
	.quad	__unnamed_14
	.size	_cstr_literal_4279, 8

	.type	__unnamed_15,@object            ; @14
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_15:
	.asciz	"FileNotFoundError"
	.size	__unnamed_15, 18

	.type	_cstr_literal_4280,@object      ; @_cstr_literal_4280
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4280:
	.quad	__unnamed_15
	.size	_cstr_literal_4280, 8

	.type	__unnamed_16,@object            ; @15
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_16:
	.asciz	"InterruptedError"
	.size	__unnamed_16, 17

	.type	_cstr_literal_4281,@object      ; @_cstr_literal_4281
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4281:
	.quad	__unnamed_16
	.size	_cstr_literal_4281, 8

	.type	__unnamed_17,@object            ; @16
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_17:
	.asciz	"IsADirectoryError"
	.size	__unnamed_17, 18

	.type	_cstr_literal_4282,@object      ; @_cstr_literal_4282
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4282:
	.quad	__unnamed_17
	.size	_cstr_literal_4282, 8

	.type	__unnamed_18,@object            ; @17
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_18:
	.asciz	"NotADirectoryError"
	.size	__unnamed_18, 19

	.type	_cstr_literal_4283,@object      ; @_cstr_literal_4283
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4283:
	.quad	__unnamed_18
	.size	_cstr_literal_4283, 8

	.type	__unnamed_19,@object            ; @18
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_19:
	.asciz	"PermissionError"
	.size	__unnamed_19, 16

	.type	_cstr_literal_4284,@object      ; @_cstr_literal_4284
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4284:
	.quad	__unnamed_19
	.size	_cstr_literal_4284, 8

	.type	__unnamed_20,@object            ; @19
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_20:
	.asciz	"ProcessLookupError"
	.size	__unnamed_20, 19

	.type	_cstr_literal_4285,@object      ; @_cstr_literal_4285
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4285:
	.quad	__unnamed_20
	.size	_cstr_literal_4285, 8

	.type	__unnamed_21,@object            ; @20
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_21:
	.asciz	"TimeoutError"
	.size	__unnamed_21, 13

	.type	_cstr_literal_4286,@object      ; @_cstr_literal_4286
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4286:
	.quad	__unnamed_21
	.size	_cstr_literal_4286, 8

	.type	__unnamed_22,@object            ; @21
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_22:
	.asciz	"EOFError"
	.size	__unnamed_22, 9

	.type	_cstr_literal_4287,@object      ; @_cstr_literal_4287
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4287:
	.quad	__unnamed_22
	.size	_cstr_literal_4287, 8

	.type	__unnamed_23,@object            ; @22
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_23:
	.asciz	"UnexpectedEOFError"
	.size	__unnamed_23, 19

	.type	_cstr_literal_4288,@object      ; @_cstr_literal_4288
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4288:
	.quad	__unnamed_23
	.size	_cstr_literal_4288, 8

	.type	__unnamed_24,@object            ; @23
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_24:
	.asciz	"BadFormatError"
	.size	__unnamed_24, 15

	.type	_cstr_literal_4289,@object      ; @_cstr_literal_4289
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4289:
	.quad	__unnamed_24
	.size	_cstr_literal_4289, 8

	.type	__unnamed_25,@object            ; @24
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_25:
	.asciz	"IOError"
	.size	__unnamed_25, 8

	.type	_cstr_literal_4290,@object      ; @_cstr_literal_4290
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4290:
	.quad	__unnamed_25
	.size	_cstr_literal_4290, 8

	.type	__unnamed_26,@object            ; @25
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_26:
	.asciz	"SystemError"
	.size	__unnamed_26, 12

	.type	_cstr_literal_4291,@object      ; @_cstr_literal_4291
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4291:
	.quad	__unnamed_26
	.size	_cstr_literal_4291, 8

	.type	__unnamed_27,@object            ; @26
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_27:
	.asciz	"_ic_these_range_int64_t_bounded_F"
	.size	__unnamed_27, 34

	.type	_cstr_literal_4292,@object      ; @_cstr_literal_4292
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4292:
	.quad	__unnamed_27
	.size	_cstr_literal_4292, 8

	.type	__unnamed_28,@object            ; @27
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_28:
	.asciz	"IllegalArgumentError"
	.size	__unnamed_28, 21

	.type	_cstr_literal_4293,@object      ; @_cstr_literal_4293
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4293:
	.quad	__unnamed_28
	.size	_cstr_literal_4293, 8

	.type	__unnamed_29,@object            ; @28
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_29:
	.asciz	"_ic_chpl_direct_range_iter"
	.size	__unnamed_29, 27

	.type	_cstr_literal_4294,@object      ; @_cstr_literal_4294
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4294:
	.quad	__unnamed_29
	.size	_cstr_literal_4294, 8

	.type	__unnamed_30,@object            ; @29
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_30:
	.asciz	"_ic_chpl_bytes__ref_string"
	.size	__unnamed_30, 27

	.type	_cstr_literal_4295,@object      ; @_cstr_literal_4295
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4295:
	.quad	__unnamed_30
	.size	_cstr_literal_4295, 8

	.type	__unnamed_31,@object            ; @30
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_31:
	.asciz	"_ic__indexLen__ref_string"
	.size	__unnamed_31, 26

	.type	_cstr_literal_4296,@object      ; @_cstr_literal_4296
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4296:
	.quad	__unnamed_31
	.size	_cstr_literal_4296, 8

	.type	__unnamed_32,@object            ; @31
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_32:
	.asciz	"_ic__cpIndexLen__ref_string"
	.size	__unnamed_32, 28

	.type	_cstr_literal_4297,@object      ; @_cstr_literal_4297
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4297:
	.quad	__unnamed_32
	.size	_cstr_literal_4297, 8

	.type	__unnamed_33,@object            ; @32
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_33:
	.asciz	"_ic_codepoints__ref_string"
	.size	__unnamed_33, 27

	.type	_cstr_literal_4298,@object      ; @_cstr_literal_4298
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4298:
	.quad	__unnamed_33
	.size	_cstr_literal_4298, 8

	.type	__unnamed_34,@object            ; @33
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_34:
	.asciz	"CodepointSplittingError"
	.size	__unnamed_34, 24

	.type	_cstr_literal_4299,@object      ; @_cstr_literal_4299
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4299:
	.quad	__unnamed_34
	.size	_cstr_literal_4299, 8

	.type	__unnamed_35,@object            ; @34
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_35:
	.asciz	"DefaultDist"
	.size	__unnamed_35, 12

	.type	_cstr_literal_4300,@object      ; @_cstr_literal_4300
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4300:
	.quad	__unnamed_35
	.size	_cstr_literal_4300, 8

	.type	__unnamed_36,@object            ; @35
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_36:
	.asciz	"DefaultRectangularDom(1,int(64),false)"
	.size	__unnamed_36, 39

	.type	_cstr_literal_4301,@object      ; @_cstr_literal_4301
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4301:
	.quad	__unnamed_36
	.size	_cstr_literal_4301, 8

	.type	__unnamed_37,@object            ; @36
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_37:
	.asciz	"DefaultRectangularArr(1,int(64),false,locale,int(64))"
	.size	__unnamed_37, 54

	.type	_cstr_literal_4302,@object      ; @_cstr_literal_4302
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4302:
	.quad	__unnamed_37
	.size	_cstr_literal_4302, 8

	.type	__unnamed_38,@object            ; @37
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_38:
	.asciz	"BaseArrOverRectangularDom(1,int(64),false)"
	.size	__unnamed_38, 43

	.type	_cstr_literal_4303,@object      ; @_cstr_literal_4303
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4303:
	.quad	__unnamed_38
	.size	_cstr_literal_4303, 8

	.type	__unnamed_39,@object            ; @38
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_39:
	.asciz	"_ic__arrs_BaseDom"
	.size	__unnamed_39, 18

	.type	_cstr_literal_4304,@object      ; @_cstr_literal_4304
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4304:
	.quad	__unnamed_39
	.size	_cstr_literal_4304, 8

	.type	__unnamed_40,@object            ; @39
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_40:
	.asciz	"_ic_these_TaskErrors"
	.size	__unnamed_40, 21

	.type	_cstr_literal_4305,@object      ; @_cstr_literal_4305
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4305:
	.quad	__unnamed_40
	.size	_cstr_literal_4305, 8

	.type	__unnamed_41,@object            ; @40
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_41:
	.asciz	"TaskErrors"
	.size	__unnamed_41, 11

	.type	_cstr_literal_4306,@object      ; @_cstr_literal_4306
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4306:
	.quad	__unnamed_41
	.size	_cstr_literal_4306, 8

	.type	__unnamed_42,@object            ; @41
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_42:
	.asciz	"_ic_chpl_direct_pos_stride_range_iter"
	.size	__unnamed_42, 38

	.type	_cstr_literal_4307,@object      ; @_cstr_literal_4307
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4307:
	.quad	__unnamed_42
	.size	_cstr_literal_4307, 8

	.type	__unnamed_43,@object            ; @42
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_43:
	.asciz	"_ic_chpl__serialViewIter"
	.size	__unnamed_43, 25

	.type	_cstr_literal_4308,@object      ; @_cstr_literal_4308
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4308:
	.quad	__unnamed_43
	.size	_cstr_literal_4308, 8

	.type	__unnamed_44,@object            ; @43
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_44:
	.asciz	"_ic_these_DefaultRectangularArr_1_int64_t_F_locale_int64_t"
	.size	__unnamed_44, 59

	.type	_cstr_literal_4309,@object      ; @_cstr_literal_4309
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4309:
	.quad	__unnamed_44
	.size	_cstr_literal_4309, 8

	.type	__unnamed_45,@object            ; @44
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_45:
	.asciz	"_ic_these__ref__array_DefaultRectangularArr_1_int64_t_F_locale_int64_t"
	.size	__unnamed_45, 71

	.type	_cstr_literal_4310,@object      ; @_cstr_literal_4310
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4310:
	.quad	__unnamed_45
	.size	_cstr_literal_4310, 8

	.type	__unnamed_46,@object            ; @45
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_46:
	.asciz	"RootLocale"
	.size	__unnamed_46, 11

	.type	_cstr_literal_4311,@object      ; @_cstr_literal_4311
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4311:
	.quad	__unnamed_46
	.size	_cstr_literal_4311, 8

	.type	__unnamed_47,@object            ; @46
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_47:
	.asciz	"_ic_these_DefaultRectangularDom_1_int64_t_F"
	.size	__unnamed_47, 44

	.type	_cstr_literal_4312,@object      ; @_cstr_literal_4312
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4312:
	.quad	__unnamed_47
	.size	_cstr_literal_4312, 8

	.type	__unnamed_48,@object            ; @47
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_48:
	.asciz	"_ic_these__ref__domain_DefaultRectangularDom_1_int64_t_F"
	.size	__unnamed_48, 57

	.type	_cstr_literal_4313,@object      ; @_cstr_literal_4313
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4313:
	.quad	__unnamed_48
	.size	_cstr_literal_4313, 8

	.type	__unnamed_49,@object            ; @48
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_49:
	.asciz	"_ic_chpl_initOnLocales_AbstractRootLocale"
	.size	__unnamed_49, 42

	.type	_cstr_literal_4314,@object      ; @_cstr_literal_4314
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4314:
	.quad	__unnamed_49
	.size	_cstr_literal_4314, 8

	.type	__unnamed_50,@object            ; @49
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_50:
	.asciz	"DefaultRectangularArr(1,int(64),false,unmanaged AbstractLocaleModel,int(64))"
	.size	__unnamed_50, 77

	.type	_cstr_literal_4315,@object      ; @_cstr_literal_4315
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4315:
	.quad	__unnamed_50
	.size	_cstr_literal_4315, 8

	.type	__unnamed_51,@object            ; @50
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_51:
	.asciz	"GPULocale"
	.size	__unnamed_51, 10

	.type	_cstr_literal_4316,@object      ; @_cstr_literal_4316
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4316:
	.quad	__unnamed_51
	.size	_cstr_literal_4316, 8

	.type	__unnamed_52,@object            ; @51
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_52:
	.asciz	"LocaleModel(0)"
	.size	__unnamed_52, 15

	.type	_cstr_literal_4317,@object      ; @_cstr_literal_4317
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4317:
	.quad	__unnamed_52
	.size	_cstr_literal_4317, 8

	.type	__unnamed_53,@object            ; @52
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_53:
	.asciz	"chpl_ModuleDeinit"
	.size	__unnamed_53, 18

	.type	_cstr_literal_4318,@object      ; @_cstr_literal_4318
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4318:
	.quad	__unnamed_53
	.size	_cstr_literal_4318, 8

	.type	__unnamed_54,@object            ; @53
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_54:
	.asciz	"DefaultRectangularArr(1,int(64),false,int(64),int(64))"
	.size	__unnamed_54, 55

	.type	_cstr_literal_4319,@object      ; @_cstr_literal_4319
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4319:
	.quad	__unnamed_54
	.size	_cstr_literal_4319, 8

	.type	__unnamed_55,@object            ; @54
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_55:
	.asciz	"DefaultRectangularArr(1,int(64),false,uint(64),int(64))"
	.size	__unnamed_55, 56

	.type	_cstr_literal_4320,@object      ; @_cstr_literal_4320
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4320:
	.quad	__unnamed_55
	.size	_cstr_literal_4320, 8

	.type	__unnamed_56,@object            ; @55
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_56:
	.asciz	"_ic_these_DefaultRectangularArr_1_int64_t_F_uint64_t_int64_t"
	.size	__unnamed_56, 61

	.type	_cstr_literal_4321,@object      ; @_cstr_literal_4321
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4321:
	.quad	__unnamed_56
	.size	_cstr_literal_4321, 8

	.type	__unnamed_57,@object            ; @56
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_57:
	.asciz	"_ic_these__ref__array_DefaultRectangularArr_1_int64_t_F_uint64_t_int64_t"
	.size	__unnamed_57, 73

	.type	_cstr_literal_4322,@object      ; @_cstr_literal_4322
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4322:
	.quad	__unnamed_57
	.size	_cstr_literal_4322, 8

	.type	__unnamed_58,@object            ; @57
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_58:
	.asciz	"_ic_these_help_DefaultRectangularDom_1_int64_t_F"
	.size	__unnamed_58, 49

	.type	_cstr_literal_4323,@object      ; @_cstr_literal_4323
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4323:
	.quad	__unnamed_58
	.size	_cstr_literal_4323, 8

	.type	__unnamed_59,@object            ; @58
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_59:
	.asciz	"_ic_these_DefaultRectangularArr_1_int64_t_F_AbstractLocaleModel_int64_t"
	.size	__unnamed_59, 72

	.type	_cstr_literal_4324,@object      ; @_cstr_literal_4324
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4324:
	.quad	__unnamed_59
	.size	_cstr_literal_4324, 8

	.type	__unnamed_60,@object            ; @59
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_60:
	.asciz	"_ic_these__ref__array_DefaultRectangularArr_1_int64_t_F_AbstractLocaleModel_int64_t"
	.size	__unnamed_60, 84

	.type	_cstr_literal_4325,@object      ; @_cstr_literal_4325
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4325:
	.quad	__unnamed_60
	.size	_cstr_literal_4325, 8

	.type	__unnamed_61,@object            ; @60
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_61:
	.asciz	"_ic_these_DefaultRectangularArr_1_int64_t_F_int64_t_int64_t"
	.size	__unnamed_61, 60

	.type	_cstr_literal_4326,@object      ; @_cstr_literal_4326
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4326:
	.quad	__unnamed_61
	.size	_cstr_literal_4326, 8

	.type	__unnamed_62,@object            ; @61
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_62:
	.asciz	"_ic_these__ref__array_DefaultRectangularArr_1_int64_t_F_int64_t_int64_t"
	.size	__unnamed_62, 72

	.type	_cstr_literal_4327,@object      ; @_cstr_literal_4327
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4327:
	.quad	__unnamed_62
	.size	_cstr_literal_4327, 8

	.type	chpl_mem_descs,@object          ; @chpl_mem_descs
	.p2align	4
chpl_mem_descs:
	.quad	__unnamed_1
	.quad	__unnamed_2
	.quad	__unnamed_3
	.quad	__unnamed_4
	.quad	__unnamed_5
	.quad	__unnamed_6
	.quad	__unnamed_7
	.quad	__unnamed_8
	.quad	__unnamed_9
	.quad	__unnamed_10
	.quad	__unnamed_11
	.quad	__unnamed_12
	.quad	__unnamed_13
	.quad	__unnamed_14
	.quad	__unnamed_15
	.quad	__unnamed_16
	.quad	__unnamed_17
	.quad	__unnamed_18
	.quad	__unnamed_19
	.quad	__unnamed_20
	.quad	__unnamed_21
	.quad	__unnamed_22
	.quad	__unnamed_23
	.quad	__unnamed_24
	.quad	__unnamed_25
	.quad	__unnamed_26
	.quad	__unnamed_27
	.quad	__unnamed_28
	.quad	__unnamed_3
	.quad	__unnamed_29
	.quad	__unnamed_30
	.quad	__unnamed_4
	.quad	__unnamed_5
	.quad	__unnamed_31
	.quad	__unnamed_32
	.quad	__unnamed_33
	.quad	__unnamed_34
	.quad	__unnamed_35
	.quad	__unnamed_36
	.quad	__unnamed_37
	.quad	__unnamed_38
	.quad	__unnamed_39
	.quad	__unnamed_40
	.quad	__unnamed_41
	.quad	__unnamed_27
	.quad	__unnamed_42
	.quad	__unnamed_43
	.quad	__unnamed_44
	.quad	__unnamed_45
	.quad	__unnamed_46
	.quad	__unnamed_47
	.quad	__unnamed_48
	.quad	__unnamed_49
	.quad	__unnamed_49
	.quad	__unnamed_50
	.quad	__unnamed_51
	.quad	__unnamed_52
	.quad	__unnamed_53
	.quad	__unnamed_54
	.quad	__unnamed_3
	.quad	__unnamed_29
	.quad	__unnamed_55
	.quad	__unnamed_43
	.quad	__unnamed_56
	.quad	__unnamed_57
	.quad	__unnamed_47
	.quad	__unnamed_44
	.quad	__unnamed_45
	.quad	__unnamed_47
	.quad	__unnamed_44
	.quad	__unnamed_45
	.quad	__unnamed_58
	.quad	__unnamed_47
	.quad	__unnamed_44
	.quad	__unnamed_45
	.quad	__unnamed_59
	.quad	__unnamed_60
	.quad	__unnamed_43
	.quad	__unnamed_59
	.quad	__unnamed_60
	.quad	__unnamed_59
	.quad	__unnamed_60
	.quad	__unnamed_61
	.quad	__unnamed_62
	.quad	__unnamed_43
	.quad	__unnamed_61
	.quad	__unnamed_62
	.quad	__unnamed_61
	.quad	__unnamed_62
	.quad	__unnamed_56
	.quad	__unnamed_57
	.quad	__unnamed_56
	.quad	__unnamed_57
	.quad	__unnamed_48
	.size	chpl_mem_descs, 752

	.type	chpl_mem_numDescs,@object       ; @chpl_mem_numDescs
	.section	.rodata,#alloc
	.p2align	2
chpl_mem_numDescs:
	.long	94                              ; 0x5e
	.size	chpl_mem_numDescs, 4

	.type	chpl_subclass_max_id,@object    ; @chpl_subclass_max_id
	.p2align	4
chpl_subclass_max_id:
	.long	0                               ; 0x0
	.long	60                              ; 0x3c
	.long	3                               ; 0x3
	.long	3                               ; 0x3
	.long	4                               ; 0x4
	.long	6                               ; 0x6
	.long	6                               ; 0x6
	.long	9                               ; 0x9
	.long	9                               ; 0x9
	.long	9                               ; 0x9
	.long	19                              ; 0x13
	.long	19                              ; 0x13
	.long	13                              ; 0xd
	.long	13                              ; 0xd
	.long	15                              ; 0xf
	.long	15                              ; 0xf
	.long	17                              ; 0x11
	.long	17                              ; 0x11
	.long	19                              ; 0x13
	.long	19                              ; 0x13
	.long	26                              ; 0x1a
	.long	21                              ; 0x15
	.long	24                              ; 0x18
	.long	23                              ; 0x17
	.long	24                              ; 0x18
	.long	26                              ; 0x1a
	.long	26                              ; 0x1a
	.long	27                              ; 0x1b
	.long	28                              ; 0x1c
	.long	29                              ; 0x1d
	.long	30                              ; 0x1e
	.long	31                              ; 0x1f
	.long	58                              ; 0x3a
	.long	33                              ; 0x21
	.long	34                              ; 0x22
	.long	35                              ; 0x23
	.long	36                              ; 0x24
	.long	37                              ; 0x25
	.long	57                              ; 0x39
	.long	39                              ; 0x27
	.long	40                              ; 0x28
	.long	45                              ; 0x2d
	.long	42                              ; 0x2a
	.long	43                              ; 0x2b
	.long	44                              ; 0x2c
	.long	45                              ; 0x2d
	.long	46                              ; 0x2e
	.long	47                              ; 0x2f
	.long	48                              ; 0x30
	.long	49                              ; 0x31
	.long	50                              ; 0x32
	.long	51                              ; 0x33
	.long	52                              ; 0x34
	.long	53                              ; 0x35
	.long	57                              ; 0x39
	.long	55                              ; 0x37
	.long	56                              ; 0x38
	.long	57                              ; 0x39
	.long	58                              ; 0x3a
	.long	59                              ; 0x3b
	.long	60                              ; 0x3c
	.size	chpl_subclass_max_id, 244

	.type	__unnamed_63,@object            ; @62
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_63:
	.zero	1
	.size	__unnamed_63, 1

	.type	_cstr_literal_3594,@object      ; @_cstr_literal_3594
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_3594:
	.quad	__unnamed_63
	.size	_cstr_literal_3594, 8

	.type	__unnamed_64,@object            ; @63
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_64:
	.asciz	"object"
	.size	__unnamed_64, 7

	.type	_cstr_literal_4328,@object      ; @_cstr_literal_4328
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4328:
	.quad	__unnamed_64
	.size	_cstr_literal_4328, 8

	.type	__unnamed_65,@object            ; @64
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_65:
	.asciz	"_EndCountBase"
	.size	__unnamed_65, 14

	.type	_cstr_literal_4329,@object      ; @_cstr_literal_4329
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4329:
	.quad	__unnamed_65
	.size	_cstr_literal_4329, 8

	.type	__unnamed_66,@object            ; @65
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_66:
	.asciz	"BaseDist"
	.size	__unnamed_66, 9

	.type	_cstr_literal_4330,@object      ; @_cstr_literal_4330
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4330:
	.quad	__unnamed_66
	.size	_cstr_literal_4330, 8

	.type	__unnamed_67,@object            ; @66
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_67:
	.asciz	"BaseDom"
	.size	__unnamed_67, 8

	.type	_cstr_literal_4331,@object      ; @_cstr_literal_4331
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4331:
	.quad	__unnamed_67
	.size	_cstr_literal_4331, 8

	.type	__unnamed_68,@object            ; @67
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_68:
	.asciz	"BaseRectangularDom(1,int(64),false)"
	.size	__unnamed_68, 36

	.type	_cstr_literal_4332,@object      ; @_cstr_literal_4332
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4332:
	.quad	__unnamed_68
	.size	_cstr_literal_4332, 8

	.type	__unnamed_69,@object            ; @68
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_69:
	.asciz	"BaseArr"
	.size	__unnamed_69, 8

	.type	_cstr_literal_4333,@object      ; @_cstr_literal_4333
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4333:
	.quad	__unnamed_69
	.size	_cstr_literal_4333, 8

	.type	__unnamed_70,@object            ; @69
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_70:
	.asciz	"BaseRectangularArr(1,int(64),false,unmanaged AbstractLocaleModel)"
	.size	__unnamed_70, 66

	.type	_cstr_literal_4334,@object      ; @_cstr_literal_4334
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4334:
	.quad	__unnamed_70
	.size	_cstr_literal_4334, 8

	.type	__unnamed_71,@object            ; @70
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_71:
	.asciz	"BaseRectangularArr(1,int(64),false,int(64))"
	.size	__unnamed_71, 44

	.type	_cstr_literal_4335,@object      ; @_cstr_literal_4335
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4335:
	.quad	__unnamed_71
	.size	_cstr_literal_4335, 8

	.type	__unnamed_72,@object            ; @71
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_72:
	.asciz	"BaseRectangularArr(1,int(64),false,locale)"
	.size	__unnamed_72, 43

	.type	_cstr_literal_4336,@object      ; @_cstr_literal_4336
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4336:
	.quad	__unnamed_72
	.size	_cstr_literal_4336, 8

	.type	__unnamed_73,@object            ; @72
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_73:
	.asciz	"BaseRectangularArr(1,int(64),false,uint(64))"
	.size	__unnamed_73, 45

	.type	_cstr_literal_4337,@object      ; @_cstr_literal_4337
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4337:
	.quad	__unnamed_73
	.size	_cstr_literal_4337, 8

	.type	__unnamed_74,@object            ; @73
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_74:
	.asciz	"BaseLocale"
	.size	__unnamed_74, 11

	.type	_cstr_literal_4338,@object      ; @_cstr_literal_4338
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4338:
	.quad	__unnamed_74
	.size	_cstr_literal_4338, 8

	.type	__unnamed_75,@object            ; @74
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_75:
	.asciz	"AbstractLocaleModel"
	.size	__unnamed_75, 20

	.type	_cstr_literal_4339,@object      ; @_cstr_literal_4339
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4339:
	.quad	__unnamed_75
	.size	_cstr_literal_4339, 8

	.type	__unnamed_76,@object            ; @75
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_76:
	.asciz	"AbstractRootLocale"
	.size	__unnamed_76, 19

	.type	_cstr_literal_4340,@object      ; @_cstr_literal_4340
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4340:
	.quad	__unnamed_76
	.size	_cstr_literal_4340, 8

	.type	__unnamed_77,@object            ; @76
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_77:
	.asciz	"Error"
	.size	__unnamed_77, 6

	.type	_cstr_literal_4341,@object      ; @_cstr_literal_4341
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4341:
	.quad	__unnamed_77
	.size	_cstr_literal_4341, 8

	.type	__unnamed_78,@object            ; @77
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_78:
	.asciz	"ConnectionError"
	.size	__unnamed_78, 16

	.type	_cstr_literal_4342,@object      ; @_cstr_literal_4342
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4342:
	.quad	__unnamed_78
	.size	_cstr_literal_4342, 8

	.type	__unnamed_79,@object            ; @78
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_79:
	.asciz	"BadRegexError"
	.size	__unnamed_79, 14

	.type	_cstr_literal_4343,@object      ; @_cstr_literal_4343
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4343:
	.quad	__unnamed_79
	.size	_cstr_literal_4343, 8

	.type	__unnamed_80,@object            ; @79
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_80:
	.asciz	"QioPluginFile"
	.size	__unnamed_80, 14

	.type	_cstr_literal_4344,@object      ; @_cstr_literal_4344
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4344:
	.quad	__unnamed_80
	.size	_cstr_literal_4344, 8

	.type	__unnamed_81,@object            ; @80
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_81:
	.asciz	"QioPluginChannel"
	.size	__unnamed_81, 17

	.type	_cstr_literal_4345,@object      ; @_cstr_literal_4345
	.section	.data.rel.ro,#alloc,#write
	.p2align	3
_cstr_literal_4345:
	.quad	__unnamed_81
	.size	_cstr_literal_4345, 8

	.type	chpl_classNames,@object         ; @chpl_classNames
	.p2align	4
chpl_classNames:
	.quad	__unnamed_63
	.quad	__unnamed_64
	.quad	__unnamed_65
	.quad	__unnamed_6
	.quad	__unnamed_53
	.quad	__unnamed_66
	.quad	__unnamed_35
	.quad	__unnamed_67
	.quad	__unnamed_68
	.quad	__unnamed_36
	.quad	__unnamed_69
	.quad	__unnamed_38
	.quad	__unnamed_70
	.quad	__unnamed_50
	.quad	__unnamed_71
	.quad	__unnamed_54
	.quad	__unnamed_72
	.quad	__unnamed_37
	.quad	__unnamed_73
	.quad	__unnamed_55
	.quad	__unnamed_74
	.quad	__unnamed_7
	.quad	__unnamed_75
	.quad	__unnamed_51
	.quad	__unnamed_52
	.quad	__unnamed_76
	.quad	__unnamed_46
	.quad	__unnamed_5
	.quad	__unnamed_4
	.quad	__unnamed_3
	.quad	__unnamed_27
	.quad	__unnamed_30
	.quad	__unnamed_77
	.quad	__unnamed_2
	.quad	__unnamed_1
	.quad	__unnamed_28
	.quad	__unnamed_34
	.quad	__unnamed_41
	.quad	__unnamed_26
	.quad	__unnamed_8
	.quad	__unnamed_9
	.quad	__unnamed_78
	.quad	__unnamed_10
	.quad	__unnamed_11
	.quad	__unnamed_12
	.quad	__unnamed_13
	.quad	__unnamed_14
	.quad	__unnamed_15
	.quad	__unnamed_16
	.quad	__unnamed_17
	.quad	__unnamed_18
	.quad	__unnamed_19
	.quad	__unnamed_20
	.quad	__unnamed_21
	.quad	__unnamed_25
	.quad	__unnamed_22
	.quad	__unnamed_23
	.quad	__unnamed_24
	.quad	__unnamed_79
	.quad	__unnamed_80
	.quad	__unnamed_81
	.size	chpl_classNames, 488

	.type	chpl_finfo,@object              ; @chpl_finfo
	.section	.rodata,#alloc
	.p2align	3
chpl_finfo:
	.zero	16
	.size	chpl_finfo, 16

	.type	chpl_vmtable,@object            ; @chpl_vmtable
	.p2align	3
chpl_vmtable:
	.zero	8
	.size	chpl_vmtable, 8

	.type	chpl_global_serialize_table,@object ; @chpl_global_serialize_table
	.local	chpl_global_serialize_table
	.comm	chpl_global_serialize_table,1,3
	.ident	"Chapel version 1.28.0 pre-release (584dd9c313)"
	.ident	"clang version 3.8.0 (tags/RELEASE_380/final)"
	.section	".note.GNU-stack"
	.addrsig
	.amdgpu_metadata
---
amdhsa.kernels:
  - .args:
      - .address_space:  generic
        .offset:         0
        .size:           8
        .value_kind:     global_buffer
    .group_segment_fixed_size: 0
    .kernarg_segment_align: 8
    .kernarg_segment_size: 8
    .max_flat_workgroup_size: 1024
    .name:           add_nums
    .private_segment_fixed_size: 24
    .sgpr_count:     14
    .sgpr_spill_count: 2
    .symbol:         add_nums.kd
    .vgpr_count:     9
    .vgpr_spill_count: 0
    .wavefront_size: 64
amdhsa.target:   amdgcn-amd-amdhsa--gfx700
amdhsa.version:
  - 1
  - 1
...

	.end_amdgpu_metadata
