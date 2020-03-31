.. default-domain:: chpl

.. module:: C_FFTW
   :synopsis: Exposes the FFTW C API for advanced users.

C_FFTW
======
**Usage**

.. code-block:: chapel

   use FFTW.C_FFTW;

Exposes the FFTW C API for advanced users.

Please refer to the FFTW documentation for more details. 

.. function:: proc fftw_execute(p: fftw_plan): void

.. function:: proc fftw_plan_dft(rank: c_int, n: c_ptr(c_int), in_arg: c_ptr(fftw_complex), out_arg: c_ptr(fftw_complex), sign: c_int, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_dft_1d(n: c_int, in_arg: c_ptr(fftw_complex), out_arg: c_ptr(fftw_complex), sign: c_int, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_dft_2d(n0: c_int, n1: c_int, in_arg: c_ptr(fftw_complex), out_arg: c_ptr(fftw_complex), sign: c_int, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_dft_3d(n0: c_int, n1: c_int, n2: c_int, in_arg: c_ptr(fftw_complex), out_arg: c_ptr(fftw_complex), sign: c_int, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_many_dft(rank: c_int, n: c_ptr(c_int), howmany: c_int, in_arg: c_ptr(fftw_complex), inembed: c_ptr(c_int), istride: c_int, idist: c_int, out_arg: c_ptr(fftw_complex), onembed: c_ptr(c_int), ostride: c_int, odist: c_int, sign: c_int, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru_dft(rank: c_int, dims: c_ptr(fftw_iodim), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim), in_arg: c_ptr(fftw_complex), out_arg: c_ptr(fftw_complex), sign: c_int, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru_split_dft(rank: c_int, dims: c_ptr(fftw_iodim), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim), ri: c_ptr(c_double), ii: c_ptr(c_double), ro: c_ptr(c_double), io: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru64_dft(rank: c_int, dims: c_ptr(fftw_iodim64), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim64), in_arg: c_ptr(fftw_complex), out_arg: c_ptr(fftw_complex), sign: c_int, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru64_split_dft(rank: c_int, dims: c_ptr(fftw_iodim64), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim64), ri: c_ptr(c_double), ii: c_ptr(c_double), ro: c_ptr(c_double), io: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_execute_dft(p: fftw_plan, in_arg: c_ptr(fftw_complex), out_arg: c_ptr(fftw_complex)): void

.. function:: proc fftw_execute_split_dft(p: fftw_plan, ri: c_ptr(c_double), ii: c_ptr(c_double), ro: c_ptr(c_double), io: c_ptr(c_double)): void

.. function:: proc fftw_plan_many_dft_r2c(rank: c_int, n: c_ptr(c_int), howmany: c_int, in_arg: c_ptr(c_double), inembed: c_ptr(c_int), istride: c_int, idist: c_int, out_arg: c_ptr(fftw_complex), onembed: c_ptr(c_int), ostride: c_int, odist: c_int, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_dft_r2c(rank: c_int, n: c_ptr(c_int), in_arg: c_ptr(c_double), out_arg: c_ptr(fftw_complex), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_dft_r2c_1d(n: c_int, in_arg: c_ptr(c_double), out_arg: c_ptr(fftw_complex), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_dft_r2c_2d(n0: c_int, n1: c_int, in_arg: c_ptr(c_double), out_arg: c_ptr(fftw_complex), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_dft_r2c_3d(n0: c_int, n1: c_int, n2: c_int, in_arg: c_ptr(c_double), out_arg: c_ptr(fftw_complex), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_many_dft_c2r(rank: c_int, n: c_ptr(c_int), howmany: c_int, in_arg: c_ptr(fftw_complex), inembed: c_ptr(c_int), istride: c_int, idist: c_int, out_arg: c_ptr(c_double), onembed: c_ptr(c_int), ostride: c_int, odist: c_int, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_dft_c2r(rank: c_int, n: c_ptr(c_int), in_arg: c_ptr(fftw_complex), out_arg: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_dft_c2r_1d(n: c_int, in_arg: c_ptr(fftw_complex), out_arg: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_dft_c2r_2d(n0: c_int, n1: c_int, in_arg: c_ptr(fftw_complex), out_arg: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_dft_c2r_3d(n0: c_int, n1: c_int, n2: c_int, in_arg: c_ptr(fftw_complex), out_arg: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru_dft_r2c(rank: c_int, dims: c_ptr(fftw_iodim), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim), in_arg: c_ptr(c_double), out_arg: c_ptr(fftw_complex), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru_dft_c2r(rank: c_int, dims: c_ptr(fftw_iodim), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim), in_arg: c_ptr(fftw_complex), out_arg: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru_split_dft_r2c(rank: c_int, dims: c_ptr(fftw_iodim), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim), in_arg: c_ptr(c_double), ro: c_ptr(c_double), io: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru_split_dft_c2r(rank: c_int, dims: c_ptr(fftw_iodim), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim), ri: c_ptr(c_double), ii: c_ptr(c_double), out_arg: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru64_dft_r2c(rank: c_int, dims: c_ptr(fftw_iodim64), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim64), in_arg: c_ptr(c_double), out_arg: c_ptr(fftw_complex), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru64_dft_c2r(rank: c_int, dims: c_ptr(fftw_iodim64), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim64), in_arg: c_ptr(fftw_complex), out_arg: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru64_split_dft_r2c(rank: c_int, dims: c_ptr(fftw_iodim64), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim64), in_arg: c_ptr(c_double), ro: c_ptr(c_double), io: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru64_split_dft_c2r(rank: c_int, dims: c_ptr(fftw_iodim64), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim64), ri: c_ptr(c_double), ii: c_ptr(c_double), out_arg: c_ptr(c_double), flags: c_uint): fftw_plan

.. function:: proc fftw_execute_dft_r2c(p: fftw_plan, in_arg: c_ptr(c_double), out_arg: c_ptr(fftw_complex)): void

.. function:: proc fftw_execute_dft_c2r(p: fftw_plan, in_arg: c_ptr(fftw_complex), out_arg: c_ptr(c_double)): void

.. function:: proc fftw_execute_split_dft_r2c(p: fftw_plan, in_arg: c_ptr(c_double), ro: c_ptr(c_double), io: c_ptr(c_double)): void

.. function:: proc fftw_execute_split_dft_c2r(p: fftw_plan, ri: c_ptr(c_double), ii: c_ptr(c_double), out_arg: c_ptr(c_double)): void

.. function:: proc fftw_plan_many_r2r(rank: c_int, n: c_ptr(c_int), howmany: c_int, in_arg: c_ptr(c_double), inembed: c_ptr(c_int), istride: c_int, idist: c_int, out_arg: c_ptr(c_double), onembed: c_ptr(c_int), ostride: c_int, odist: c_int, kind: c_ptr(fftw_r2r_kind), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_r2r(rank: c_int, n: c_ptr(c_int), in_arg: c_ptr(c_double), out_arg: c_ptr(c_double), kind: c_ptr(fftw_r2r_kind), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_r2r_1d(n: c_int, in_arg: c_ptr(c_double), out_arg: c_ptr(c_double), kind: fftw_r2r_kind, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_r2r_2d(n0: c_int, n1: c_int, in_arg: c_ptr(c_double), out_arg: c_ptr(c_double), kind0: fftw_r2r_kind, kind1: fftw_r2r_kind, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_r2r_3d(n0: c_int, n1: c_int, n2: c_int, in_arg: c_ptr(c_double), out_arg: c_ptr(c_double), kind0: fftw_r2r_kind, kind1: fftw_r2r_kind, kind2: fftw_r2r_kind, flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru_r2r(rank: c_int, dims: c_ptr(fftw_iodim), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim), in_arg: c_ptr(c_double), out_arg: c_ptr(c_double), kind: c_ptr(fftw_r2r_kind), flags: c_uint): fftw_plan

.. function:: proc fftw_plan_guru64_r2r(rank: c_int, dims: c_ptr(fftw_iodim64), howmany_rank: c_int, howmany_dims: c_ptr(fftw_iodim64), in_arg: c_ptr(c_double), out_arg: c_ptr(c_double), kind: c_ptr(fftw_r2r_kind), flags: c_uint): fftw_plan

.. function:: proc fftw_execute_r2r(p: fftw_plan, in_arg: c_ptr(c_double), out_arg: c_ptr(c_double)): void

.. function:: proc fftw_destroy_plan(p: fftw_plan): void

.. function:: proc fftw_forget_wisdom(): void

.. function:: proc fftw_cleanup(): void

.. function:: proc fftw_set_timelimit(t: c_double): void

.. function:: proc fftw_plan_with_nthreads(nthreads: c_int): void

.. function:: proc fftw_init_threads(): c_int

.. function:: proc fftw_cleanup_threads(): void

.. function:: proc fftw_make_planner_thread_safe(): void

.. function:: proc fftw_export_wisdom_to_filename(filename: c_string): c_int

.. function:: proc fftw_export_wisdom_to_string(): c_string

.. function:: proc fftw_export_wisdom(write_char: fftw_write_char_func, data: c_void_ptr): void

.. function:: proc fftw_import_system_wisdom(): c_int

.. function:: proc fftw_import_wisdom_from_filename(filename: c_string): c_int

.. function:: proc fftw_import_wisdom_from_string(input_string: c_string): c_int

.. function:: proc fftw_import_wisdom(read_char: fftw_read_char_func, data: c_void_ptr): c_int

.. function:: proc fftw_fprint_plan(p: fftw_plan, ref output_file: _file): void

.. function:: proc fftw_fprint_plan(p: fftw_plan, output_file: c_ptr(_file)): void

.. function:: proc fftw_print_plan(p: fftw_plan): void

.. function:: proc fftw_sprint_plan(p: fftw_plan): c_string

.. function:: proc fftw_malloc(n: size_t): c_void_ptr

.. function:: proc fftw_alloc_real(n: size_t): c_ptr(c_double)

.. function:: proc fftw_alloc_complex(n: size_t): c_ptr(fftw_complex)

.. function:: proc fftw_free(p: c_void_ptr): void

.. function:: proc fftw_flops(p: fftw_plan, ref add: c_double, ref mul: c_double, ref fmas: c_double): void

.. function:: proc fftw_flops(p: fftw_plan, add: c_ptr(c_double), mul: c_ptr(c_double), fmas: c_ptr(c_double)): void

.. function:: proc fftw_estimate_cost(p: fftw_plan): c_double

.. function:: proc fftw_cost(p: fftw_plan): c_double

.. function:: proc fftw_alignment_of(ref p: c_double): c_int

.. function:: proc fftw_alignment_of(p: c_ptr(c_double)): c_int

.. data:: var fftw_version: c_ptr(c_char)

.. data:: var fftw_cc: c_ptr(c_char)

.. data:: var fftw_codelet_optim: c_ptr(c_char)

.. record:: fftw_iodim

   .. attribute:: var n: c_int

   .. attribute:: var is: c_int

   .. attribute:: var os: c_int

.. record:: fftw_iodim64

   .. attribute:: var n: c_ptrdiff

   .. attribute:: var is: c_ptrdiff

   .. attribute:: var os: c_ptrdiff

.. type:: type fftw_r2r_kind = c_int

.. type:: type fftw_read_char_func

.. type:: type fftw_write_char_func

.. type:: type fftw_complex = complex(128)

