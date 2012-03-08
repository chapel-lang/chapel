/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#define DIV_2D_N 8
#define DIV_2D_M 8

#define I_SHIFT (I_BITS/2)
#define J_BITS I_SHIFT

#define STEP_TAG_2D(plan, step, i, j) _STEP_TAG(plan, step, ((starpu_tag_t) i << I_SHIFT) | (starpu_tag_t) j)

#ifdef __STARPU_USE_CUDA
/* Twist the full vector into a n2,m2 chunk */
static void
STARPUFFT(twist1_2d_kernel_gpu)(void *descr[], void *_args)
{
	struct STARPUFFT(args) *args = _args;
	STARPUFFT(plan) plan = args->plan;
	int i = args->i;
	int j = args->j;
	int n1 = plan->n1[0];
	int n2 = plan->n2[0];
	int m1 = plan->n1[1];
	int m2 = plan->n2[1];

	_cufftComplex * restrict in = (_cufftComplex *)STARPU_VECTOR_GET_PTR(descr[0]);
	_cufftComplex * restrict twisted1 = (_cufftComplex *)STARPU_VECTOR_GET_PTR(descr[1]);

	STARPUFFT(cuda_twist1_2d_host)(in, twisted1, i, j, n1, n2, m1, m2);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}

/* fft1:
 *
 * Perform one fft of size n2,m2 */
static void
STARPUFFT(fft1_2d_plan_gpu)(void *args)
{
	STARPUFFT(plan) plan = args;
	int n2 = plan->n2[0];
	int m2 = plan->n2[1];
	int workerid = starpu_worker_get_id();
	cufftResult cures;

	cures = cufftPlan2d(&plan->plans[workerid].plan1_cuda, n2, m2, _CUFFT_C2C);
	STARPU_ASSERT(cures == CUFFT_SUCCESS);
	cufftSetStream(plan->plans[workerid].plan1_cuda, starpu_cuda_get_local_stream());
	STARPU_ASSERT(cures == CUFFT_SUCCESS);
}

static void
STARPUFFT(fft1_2d_kernel_gpu)(void *descr[], void *_args)
{
	struct STARPUFFT(args) *args = _args;
	STARPUFFT(plan) plan = args->plan;
	int i = args->i;
	int j = args->j;
	int n2 = plan->n2[0];
	int m2 = plan->n2[1];
	cufftResult cures;

	_cufftComplex * restrict in = (_cufftComplex *)STARPU_VECTOR_GET_PTR(descr[0]);
	_cufftComplex * restrict out = (_cufftComplex *)STARPU_VECTOR_GET_PTR(descr[1]);
	const _cufftComplex * restrict roots0 = (_cufftComplex *)STARPU_VECTOR_GET_PTR(descr[2]);
	const _cufftComplex * restrict roots1 = (_cufftComplex *)STARPU_VECTOR_GET_PTR(descr[3]);

	int workerid = starpu_worker_get_id();

	task_per_worker[workerid]++;

	cures = _cufftExecC2C(plan->plans[workerid].plan1_cuda, in, out, plan->sign == -1 ? CUFFT_FORWARD : CUFFT_INVERSE);
	STARPU_ASSERT(cures == CUFFT_SUCCESS);

	/* synchronization is done after the twiddling */
	STARPUFFT(cuda_twiddle_2d_host)(out, roots0, roots1, n2, m2, i, j);

	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}

/* fft2:
 *
 * Perform n3*m3 ffts of size n1,m1 */
static void
STARPUFFT(fft2_2d_plan_gpu(void *args))
{
	STARPUFFT(plan) plan = args;
	int n1 = plan->n1[0];
	int m1 = plan->n1[1];
	cufftResult cures;
	int workerid = starpu_worker_get_id();

	cures = cufftPlan2d(&plan->plans[workerid].plan2_cuda, n1, m1, _CUFFT_C2C);
	STARPU_ASSERT(cures == CUFFT_SUCCESS);
	cufftSetStream(plan->plans[workerid].plan2_cuda, starpu_cuda_get_local_stream());
	STARPU_ASSERT(cures == CUFFT_SUCCESS);
}

static void
STARPUFFT(fft2_2d_kernel_gpu)(void *descr[], void *_args)
{
	struct STARPUFFT(args) *args = _args;
	STARPUFFT(plan) plan = args->plan;
	int n1 = plan->n1[0];
	int n2 = plan->n2[0];
	int m1 = plan->n1[1];
	int m2 = plan->n2[1];
	int n3 = n2/DIV_2D_N;
	int m3 = m2/DIV_2D_M;
	int n;
	cufftResult cures;

	_cufftComplex * restrict in = (_cufftComplex *)STARPU_VECTOR_GET_PTR(descr[0]);
	_cufftComplex * restrict out = (_cufftComplex *)STARPU_VECTOR_GET_PTR(descr[1]);

	int workerid = starpu_worker_get_id();

	task_per_worker[workerid]++;

	for (n = 0; n < n3*m3; n++) {
		cures = _cufftExecC2C(plan->plans[workerid].plan2_cuda, in + n * n1*m1, out + n * n1*m1, plan->sign == -1 ? CUFFT_FORWARD : CUFFT_INVERSE);
		STARPU_ASSERT(cures == CUFFT_SUCCESS);
	}

	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
#endif

/* Twist the full vector into a n2,m2 chunk */
static void
STARPUFFT(twist1_2d_kernel_cpu)(void *descr[], void *_args)
{
	struct STARPUFFT(args) *args = _args;
	STARPUFFT(plan) plan = args->plan;
	int i = args->i;
	int j = args->j;
	int k, l;
	int n1 = plan->n1[0];
	int n2 = plan->n2[0];
	int m1 = plan->n1[1];
	int m2 = plan->n2[1];
	int m = plan->n[1];

	STARPUFFT(complex) * restrict in = (STARPUFFT(complex) *)STARPU_VECTOR_GET_PTR(descr[0]);
	STARPUFFT(complex) * restrict twisted1 = (STARPUFFT(complex) *)STARPU_VECTOR_GET_PTR(descr[1]);

	/* printf("twist1 %d %d %g\n", i, j, (double) cabs(plan->in[i+j])); */

	for (k = 0; k < n2; k++)
		for (l = 0; l < m2; l++)
			twisted1[k*m2+l] = in[i*m+j+k*m*n1+l*m1];
}

#ifdef STARPU_HAVE_FFTW
/* Perform an n2,m2 fft */
static void
STARPUFFT(fft1_2d_kernel_cpu)(void *descr[], void *_args)
{
	struct STARPUFFT(args) *args = _args;
	STARPUFFT(plan) plan = args->plan;
	int i = args->i;
	int j = args->j;
	int k, l;
	int n2 = plan->n2[0];
	int m2 = plan->n2[1];
	int workerid = starpu_worker_get_id();

	task_per_worker[workerid]++;

	STARPUFFT(complex) *twisted1 = (STARPUFFT(complex) *)STARPU_VECTOR_GET_PTR(descr[0]);
	STARPUFFT(complex) *fft1 = (STARPUFFT(complex) *)STARPU_VECTOR_GET_PTR(descr[1]);

	/* printf("fft1 %d %d %g\n", i, j, (double) cabs(twisted1[0])); */

	_FFTW(execute_dft)(plan->plans[workerid].plan1_cpu, twisted1, fft1);
	for (k = 0; k < n2; k++)
		for (l = 0; l < m2; l++)
			fft1[k*m2 + l] = fft1[k*m2 + l] * plan->roots[0][i*k] * plan->roots[1][j*l];
}
#endif

/* Twist the full vector into a package of n2/DIV_2D_N,m2/DIV_2D_M (n1,m1) chunks */
static void
STARPUFFT(twist2_2d_kernel_cpu)(void *descr[], void *_args)
{
	struct STARPUFFT(args) *args = _args;
	STARPUFFT(plan) plan = args->plan;
	int kk = args->kk;	/* between 0 and DIV_2D_N */
	int ll = args->ll;	/* between 0 and DIV_2D_M */
	int kkk, lll;		/* beetween 0,0 and n3,m3 */
	int i, j;
	int n1 = plan->n1[0];
	int n2 = plan->n2[0];
	int m1 = plan->n1[1];
	int m2 = plan->n2[1];
	int n3 = n2/DIV_2D_N;
	int m3 = m2/DIV_2D_M;

	STARPUFFT(complex) * restrict twisted2 = (STARPUFFT(complex) *)STARPU_VECTOR_GET_PTR(descr[0]);

	/* printf("twist2 %d %d %g\n", kk, ll, (double) cabs(plan->fft1[kk+ll])); */

	for (kkk = 0; kkk < n3; kkk++) {
		int k = kk * n3 + kkk;
		for (lll = 0; lll < m3; lll++) {
			int l = ll * m3 + lll;
			for (i = 0; i < n1; i++)
				for (j = 0; j < m1; j++)
					twisted2[kkk*m3*n1*m1+lll*n1*m1+i*m1+j] = plan->fft1[i*n1*n2*m2+j*n2*m2+k*m2+l];
		}
	}
}

#ifdef STARPU_HAVE_FFTW
/* Perform (n2/DIV_2D_N)*(m2/DIV_2D_M) (n1,m1) ffts */
static void
STARPUFFT(fft2_2d_kernel_cpu)(void *descr[], void *_args)
{
	struct STARPUFFT(args) *args = _args;
	STARPUFFT(plan) plan = args->plan;
	/* int kk = args->kk; */
	/* int ll = args->ll; */
	int workerid = starpu_worker_get_id();

	task_per_worker[workerid]++;

	STARPUFFT(complex) *twisted2 = (STARPUFFT(complex) *)STARPU_VECTOR_GET_PTR(descr[0]);
	STARPUFFT(complex) *fft2 = (STARPUFFT(complex) *)STARPU_VECTOR_GET_PTR(descr[1]);

	/* printf("fft2 %d %d %g\n", kk, ll, (double) cabs(twisted2[plan->totsize4-1])); */

	_FFTW(execute_dft)(plan->plans[workerid].plan2_cpu, twisted2, fft2);
}
#endif

/* Spread the package of (n2/DIV_2D_N)*(m2/DIV_2D_M) (n1,m1) chunks into the full vector */
static void
STARPUFFT(twist3_2d_kernel_cpu)(void *descr[], void *_args)
{
	struct STARPUFFT(args) *args = _args;
	STARPUFFT(plan) plan = args->plan;
	int kk = args->kk;	/* between 0 and DIV_2D_N */
	int ll = args->ll;	/* between 0 and DIV_2D_M */
	int kkk, lll;		/* beetween 0,0 and n3,m3 */
	int i, j;
	int n1 = plan->n1[0];
	int n2 = plan->n2[0];
	int m1 = plan->n1[1];
	int m2 = plan->n2[1];
	int n3 = n2/DIV_2D_N;
	int m3 = m2/DIV_2D_M;
	int m = plan->n[1];

	const STARPUFFT(complex) * restrict fft2 = (STARPUFFT(complex) *)STARPU_VECTOR_GET_PTR(descr[0]);

	/* printf("twist3 %d %d %g\n", kk, ll, (double) cabs(fft2[0])); */

	for (kkk = 0; kkk < n3; kkk++) {
		int k = kk * n3 + kkk;
		for (lll = 0; lll < m3; lll++) {
			int l = ll * m3 + lll;
			for (i = 0; i < n1; i++)
				for (j = 0; j < m1; j++)
					plan->out[i*n2*m+j*m2+k*m+l] = fft2[kkk*m3*n1*m1+lll*n1*m1+i*m1+j];
		}
	}
}

struct starpu_perfmodel STARPUFFT(twist1_2d_model) = {
	.type = STARPU_HISTORY_BASED,
	.symbol = TYPE"twist1_2d"
};

struct starpu_perfmodel STARPUFFT(fft1_2d_model) = {
	.type = STARPU_HISTORY_BASED,
	.symbol = TYPE"fft1_2d"
};

struct starpu_perfmodel STARPUFFT(twist2_2d_model) = {
	.type = STARPU_HISTORY_BASED,
	.symbol = TYPE"twist2_2d"
};

struct starpu_perfmodel STARPUFFT(fft2_2d_model) = {
	.type = STARPU_HISTORY_BASED,
	.symbol = TYPE"fft2_2d"
};

struct starpu_perfmodel STARPUFFT(twist3_2d_model) = {
	.type = STARPU_HISTORY_BASED,
	.symbol = TYPE"twist3_2d"
};

static struct starpu_codelet STARPUFFT(twist1_2d_codelet) = {
	.where =
#ifdef __STARPU_USE_CUDA
		STARPU_CUDA|
#endif
		STARPU_CPU,
#ifdef __STARPU_USE_CUDA
	.cuda_funcs = {STARPUFFT(twist1_2d_kernel_gpu), NULL},
#endif
	.cpu_funcs = {STARPUFFT(twist1_2d_kernel_cpu), NULL},
	CAN_EXECUTE
	.model = &STARPUFFT(twist1_2d_model),
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_W}
};

static struct starpu_codelet STARPUFFT(fft1_2d_codelet) = {
	.where =
#ifdef __STARPU_USE_CUDA
		STARPU_CUDA|
#endif
#ifdef STARPU_HAVE_FFTW
		STARPU_CPU|
#endif
		0,
#ifdef __STARPU_USE_CUDA
	.cuda_funcs = {STARPUFFT(fft1_2d_kernel_gpu), NULL},
#endif
#ifdef STARPU_HAVE_FFTW
	.cpu_funcs = {STARPUFFT(fft1_2d_kernel_cpu), NULL},
#endif
	CAN_EXECUTE
	.model = &STARPUFFT(fft1_2d_model),
	.nbuffers = 4,
	.modes = {STARPU_R, STARPU_W, STARPU_R, STARPU_R}
};

static struct starpu_codelet STARPUFFT(twist2_2d_codelet) = {
	.where = STARPU_CPU,
	.cpu_funcs = {STARPUFFT(twist2_2d_kernel_cpu), NULL},
	CAN_EXECUTE
	.model = &STARPUFFT(twist2_2d_model),
	.nbuffers = 1,
	.modes = {STARPU_W}
};

static struct starpu_codelet STARPUFFT(fft2_2d_codelet) = {
	.where =
#ifdef __STARPU_USE_CUDA
		STARPU_CUDA|
#endif
#ifdef STARPU_HAVE_FFTW
		STARPU_CPU|
#endif
		0,
#ifdef __STARPU_USE_CUDA
	.cuda_funcs = {STARPUFFT(fft2_2d_kernel_gpu), NULL},
#endif
#ifdef STARPU_HAVE_FFTW
	.cpu_funcs = {STARPUFFT(fft2_2d_kernel_cpu), NULL},
#endif
	CAN_EXECUTE
	.model = &STARPUFFT(fft2_2d_model),
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_W}
};

static struct starpu_codelet STARPUFFT(twist3_2d_codelet) = {
	.where = STARPU_CPU,
	.cpu_funcs = {STARPUFFT(twist3_2d_kernel_cpu), NULL},
	CAN_EXECUTE
	.model = &STARPUFFT(twist3_2d_model),
	.nbuffers = 1,
	.modes = {STARPU_R}
};

/*
 *
 * Sequential version
 *
 */

#ifdef __STARPU_USE_CUDA
/* Perform one fft of size n,m */
static void
STARPUFFT(fft_2d_plan_gpu)(void *args)
{
	STARPUFFT(plan) plan = args;
	cufftResult cures;
	int n = plan->n[0];
	int m = plan->n[1];
	int workerid = starpu_worker_get_id();

	cures = cufftPlan2d(&plan->plans[workerid].plan_cuda, n, m, _CUFFT_C2C);
	STARPU_ASSERT(cures == CUFFT_SUCCESS);
	cufftSetStream(plan->plans[workerid].plan_cuda, starpu_cuda_get_local_stream());
	STARPU_ASSERT(cures == CUFFT_SUCCESS);
}

static void
STARPUFFT(fft_2d_kernel_gpu)(void *descr[], void *args)
{
	STARPUFFT(plan) plan = args;
	cufftResult cures;

	_cufftComplex * restrict in = (_cufftComplex *)STARPU_VECTOR_GET_PTR(descr[0]);
	_cufftComplex * restrict out = (_cufftComplex *)STARPU_VECTOR_GET_PTR(descr[1]);

	int workerid = starpu_worker_get_id();

	task_per_worker[workerid]++;

	cures = _cufftExecC2C(plan->plans[workerid].plan_cuda, in, out, plan->sign == -1 ? CUFFT_FORWARD : CUFFT_INVERSE);
	STARPU_ASSERT(cures == CUFFT_SUCCESS);

	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
#endif

#ifdef STARPU_HAVE_FFTW
/* Perform one fft of size n,m */
static void
STARPUFFT(fft_2d_kernel_cpu)(void *descr[], void *_args)
{
	STARPUFFT(plan) plan = _args;
	int workerid = starpu_worker_get_id();

	task_per_worker[workerid]++;

	STARPUFFT(complex) * restrict in = (STARPUFFT(complex) *)STARPU_VECTOR_GET_PTR(descr[0]);
	STARPUFFT(complex) * restrict out = (STARPUFFT(complex) *)STARPU_VECTOR_GET_PTR(descr[1]);

	_FFTW(execute_dft)(plan->plans[workerid].plan_cpu, in, out);
}
#endif

static struct starpu_perfmodel STARPUFFT(fft_2d_model) = {
	.type = STARPU_HISTORY_BASED,
	.symbol = TYPE"fft_2d"
};

static struct starpu_codelet STARPUFFT(fft_2d_codelet) = {
	.where =
#ifdef __STARPU_USE_CUDA
		STARPU_CUDA|
#endif
#ifdef STARPU_HAVE_FFTW
		STARPU_CPU|
#endif
		0,
#ifdef __STARPU_USE_CUDA
	.cuda_funcs = {STARPUFFT(fft_2d_kernel_gpu), NULL},
#endif
#ifdef STARPU_HAVE_FFTW
	.cpu_funcs = {STARPUFFT(fft_2d_kernel_cpu), NULL},
#endif
	CAN_EXECUTE
	.model = &STARPUFFT(fft_2d_model),
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_W}
};

STARPUFFT(plan)
STARPUFFT(plan_dft_2d)(int n, int m, int sign, unsigned flags)
{
	int workerid;
	int n1 = DIV_2D_N;
	int n2 = n / n1;
	int n3;
	int m1 = DIV_2D_M;
	int m2 = m / m1;
	int m3;
	int z;
	struct starpu_task *task;

if (PARALLEL) {
	/*
	 * Simple strategy:
	 *
	 * - twist1: twist input in n1*m1 (n2,m2) chunks
	 * - fft1:   perform n1*m1 (n2,m2) ffts
	 * - twist2: twist into n2*m2 (n1,m1) chunks distributed in
	 *           DIV_2D_N*DIV_2D_M groups
	 * - fft2:   perform DIV_2D_N*DIV_2D_M times n3*m3 (n1,m1) ffts
	 * - twist3: twist back into output
	 */

#ifdef __STARPU_USE_CUDA
	/* cufft 2D-3D limited to [2,16384] */
	while (n2 > 16384) {
		n1 *= 2;
		n2 /= 2;
	}
#endif
	STARPU_ASSERT(n == n1*n2);
	STARPU_ASSERT(n1 < (1ULL << J_BITS));


#ifdef __STARPU_USE_CUDA
	/* cufft 2D-3D limited to [2,16384] */
	while (m2 > 16384) {
		m1 *= 2;
		m2 /= 2;
	}
#endif
	STARPU_ASSERT(m == m1*m2);
	STARPU_ASSERT(m1 < (1ULL << J_BITS));

	/* distribute the n2*m2 second ffts into DIV_2D_N*DIV_2D_M packages */
	n3 = n2 / DIV_2D_N;
	STARPU_ASSERT(n2 == n3*DIV_2D_N);
	m3 = m2 / DIV_2D_M;
	STARPU_ASSERT(m2 == m3*DIV_2D_M);
}

	/* TODO: flags? Automatically set FFTW_MEASURE on calibration? */
	STARPU_ASSERT(flags == 0);

	STARPUFFT(plan) plan = malloc(sizeof(*plan));
	memset(plan, 0, sizeof(*plan));

if (PARALLEL) {
	plan->number = STARPU_ATOMIC_ADD(&starpufft_last_plan_number, 1) - 1;

	/* 4bit limitation in the tag space */
	STARPU_ASSERT(plan->number < (1ULL << NUMBER_BITS));
}

	plan->dim = 2;
	plan->n = malloc(plan->dim * sizeof(*plan->n));
	plan->n[0] = n;
	plan->n[1] = m;

if (PARALLEL) {
	check_dims(plan);

	plan->n1 = malloc(plan->dim * sizeof(*plan->n1));
	plan->n1[0] = n1;
	plan->n1[1] = m1;
	plan->n2 = malloc(plan->dim * sizeof(*plan->n2));
	plan->n2[0] = n2;
	plan->n2[1] = m2;
}

	plan->totsize = n * m;

if (PARALLEL) {
	plan->totsize1 = n1 * m1;
	plan->totsize2 = n2 * m2;
	plan->totsize3 = DIV_2D_N * DIV_2D_M;
	plan->totsize4 = plan->totsize / plan->totsize3;
}
	plan->type = C2C;
	plan->sign = sign;

if (PARALLEL) {
	/* Compute the w^k just once. */
	compute_roots(plan);
}

	/* Initialize per-worker working set */
	for (workerid = 0; workerid < starpu_worker_get_count(); workerid++) {
		switch (starpu_worker_get_type(workerid)) {
		case STARPU_CPU_WORKER:
#ifdef STARPU_HAVE_FFTW
if (PARALLEL) {
			/* first fft plan: one n2*m2 fft */
			plan->plans[workerid].plan1_cpu = _FFTW(plan_dft_2d)(n2, m2, NULL, (void*) 1, sign, _FFTW_FLAGS);
			STARPU_ASSERT(plan->plans[workerid].plan1_cpu);

			/* second fft plan: n3*m3 n1*m1 ffts */
			plan->plans[workerid].plan2_cpu = _FFTW(plan_many_dft)(plan->dim,
					plan->n1, n3*m3,
					NULL, NULL, 1, plan->totsize1,
					(void*) 1, NULL, 1, plan->totsize1,
					sign, _FFTW_FLAGS);
			STARPU_ASSERT(plan->plans[workerid].plan2_cpu);
} else {
			/* fft plan: one fft of size n, m. */
			plan->plans[workerid].plan_cpu = _FFTW(plan_dft_2d)(n, m, NULL, (void*) 1, sign, _FFTW_FLAGS);
			STARPU_ASSERT(plan->plans[workerid].plan_cpu);
}
#else
/* #warning libstarpufft can not work correctly if libfftw3 is not installed */
#endif
			break;
		case STARPU_CUDA_WORKER:
			break;
		default:
			/* Do not care, we won't be executing anything there. */
			break;
		}
	}
#ifdef __STARPU_USE_CUDA
if (PARALLEL) {
	starpu_execute_on_each_worker(STARPUFFT(fft1_2d_plan_gpu), plan, STARPU_CUDA);
	starpu_execute_on_each_worker(STARPUFFT(fft2_2d_plan_gpu), plan, STARPU_CUDA);
} else {
	starpu_execute_on_each_worker(STARPUFFT(fft_2d_plan_gpu), plan, STARPU_CUDA);
}
#endif

if (PARALLEL) {
	/* Allocate buffers. */
	plan->twisted1 = STARPUFFT(malloc)(plan->totsize * sizeof(*plan->twisted1));
	memset(plan->twisted1, 0, plan->totsize * sizeof(*plan->twisted1));
	plan->fft1 = STARPUFFT(malloc)(plan->totsize * sizeof(*plan->fft1));
	memset(plan->fft1, 0, plan->totsize * sizeof(*plan->fft1));
	plan->twisted2 = STARPUFFT(malloc)(plan->totsize * sizeof(*plan->twisted2));
	memset(plan->twisted2, 0, plan->totsize * sizeof(*plan->twisted2));
	plan->fft2 = STARPUFFT(malloc)(plan->totsize * sizeof(*plan->fft2));
	memset(plan->fft2, 0, plan->totsize * sizeof(*plan->fft2));

	/* Allocate handle arrays */
	plan->twisted1_handle = malloc(plan->totsize1 * sizeof(*plan->twisted1_handle));
	plan->fft1_handle = malloc(plan->totsize1 * sizeof(*plan->fft1_handle));
	plan->twisted2_handle = malloc(plan->totsize3 * sizeof(*plan->twisted2_handle));
	plan->fft2_handle = malloc(plan->totsize3 * sizeof(*plan->fft2_handle));

	/* Allocate task arrays */
	plan->twist1_tasks = malloc(plan->totsize1 * sizeof(*plan->twist1_tasks));
	plan->fft1_tasks = malloc(plan->totsize1 * sizeof(*plan->fft1_tasks));
	plan->twist2_tasks = malloc(plan->totsize3 * sizeof(*plan->twist2_tasks));
	plan->fft2_tasks = malloc(plan->totsize3 * sizeof(*plan->fft2_tasks));
	plan->twist3_tasks = malloc(plan->totsize3 * sizeof(*plan->twist3_tasks));

	/* Allocate codelet argument arrays */
	plan->fft1_args = malloc(plan->totsize1 * sizeof(*plan->fft1_args));
	plan->fft2_args = malloc(plan->totsize3 * sizeof(*plan->fft2_args));

	/* Create first-round tasks */
	for (z = 0; z < plan->totsize1; z++) {
		int i = z / m1, j = z % m1;
#define STEP_TAG(step)	STEP_TAG_2D(plan, step, i, j)

		/* TODO: get rid of tags */

		plan->fft1_args[z].plan = plan;
		plan->fft1_args[z].i = i;
		plan->fft1_args[z].j = j;

		/* Register (n2,m2) chunks */
		starpu_vector_data_register(&plan->twisted1_handle[z], 0, (uintptr_t) &plan->twisted1[z*plan->totsize2], plan->totsize2, sizeof(*plan->twisted1));
		starpu_vector_data_register(&plan->fft1_handle[z], 0, (uintptr_t) &plan->fft1[z*plan->totsize2], plan->totsize2, sizeof(*plan->fft1));

		/* We'll need it on the CPU for the second twist anyway */
		starpu_data_set_wt_mask(plan->fft1_handle[z], 1<<0);

		/* Create twist1 task */
		plan->twist1_tasks[z] = task = starpu_task_create();
		task->cl = &STARPUFFT(twist1_2d_codelet);
		/* task->handles[0] = to be filled at execution */
		task->handles[1] = plan->twisted1_handle[z];
		task->cl_arg = &plan->fft1_args[z];
		task->tag_id = STEP_TAG(TWIST1);
		task->use_tag = 1;
		task->destroy = 0;

		/* Tell that fft1 depends on twisted1 */
		starpu_tag_declare_deps(STEP_TAG(FFT1),
				1, STEP_TAG(TWIST1));

		/* Create FFT1 task */
		plan->fft1_tasks[z] = task = starpu_task_create();
		task->cl = &STARPUFFT(fft1_2d_codelet);
		task->handles[0] = plan->twisted1_handle[z];
		task->handles[1] = plan->fft1_handle[z];
		task->handles[2] = plan->roots_handle[0];
		task->handles[3] = plan->roots_handle[1];
		task->cl_arg = &plan->fft1_args[z];
		task->tag_id = STEP_TAG(FFT1);
		task->use_tag = 1;
		task->destroy = 0;

		/* Tell that to be done with first step we need to have
		 * finished this fft1 */
		starpu_tag_declare_deps(STEP_TAG_2D(plan, JOIN, 0, 0),
				1, STEP_TAG(FFT1));
#undef STEP_TAG
	}

	/* Create join task */
	plan->join_task = task = starpu_task_create();
	task->cl = NULL;
	task->tag_id = STEP_TAG_2D(plan, JOIN, 0, 0);
	task->use_tag = 1;
	task->destroy = 0;

	/* Create second-round tasks */
	for (z = 0; z < plan->totsize3; z++) {
		int kk = z / DIV_2D_M, ll = z % DIV_2D_M;
#define STEP_TAG(step)	STEP_TAG_2D(plan, step, kk, ll)

		plan->fft2_args[z].plan = plan;
		plan->fft2_args[z].kk = kk;
		plan->fft2_args[z].ll = ll;

		/* Register n3*m3 (n1,m1) chunks */
		starpu_vector_data_register(&plan->twisted2_handle[z], 0, (uintptr_t) &plan->twisted2[z*plan->totsize4], plan->totsize4, sizeof(*plan->twisted2));
		starpu_vector_data_register(&plan->fft2_handle[z], 0, (uintptr_t) &plan->fft2[z*plan->totsize4], plan->totsize4, sizeof(*plan->fft2));

		/* We'll need it on the CPU for the last twist anyway */
		starpu_data_set_wt_mask(plan->fft2_handle[z], 1<<0);

		/* Tell that twisted2 depends on the whole first step to be
		 * done */
		starpu_tag_declare_deps(STEP_TAG(TWIST2),
				1, STEP_TAG_2D(plan, JOIN, 0, 0));

		/* Create twist2 task */
		plan->twist2_tasks[z] = task = starpu_task_create();
		task->cl = &STARPUFFT(twist2_2d_codelet);
		task->handles[0] = plan->twisted2_handle[z];
		task->cl_arg = &plan->fft2_args[z];
		task->tag_id = STEP_TAG(TWIST2);
		task->use_tag = 1;
		task->destroy = 0;

		/* Tell that fft2 depends on twisted2 */
		starpu_tag_declare_deps(STEP_TAG(FFT2),
				1, STEP_TAG(TWIST2));

		/* Create FFT2 task */
		plan->fft2_tasks[z] = task = starpu_task_create();
		task->cl = &STARPUFFT(fft2_2d_codelet);
		task->handles[0] = plan->twisted2_handle[z];
		task->handles[1] = plan->fft2_handle[z];
		task->cl_arg = &plan->fft2_args[z];
		task->tag_id = STEP_TAG(FFT2);
		task->use_tag = 1;
		task->destroy = 0;

		/* Tell that twist3 depends on fft2 */
		starpu_tag_declare_deps(STEP_TAG(TWIST3),
				1, STEP_TAG(FFT2));

		/* Create twist3 tasks */
		/* These run only on CPUs and thus write directly into the
		 * application output buffer. */
		plan->twist3_tasks[z] = task = starpu_task_create();
		task->cl = &STARPUFFT(twist3_2d_codelet);
		task->handles[0] = plan->fft2_handle[z];
		task->cl_arg = &plan->fft2_args[z];
		task->tag_id = STEP_TAG(TWIST3);
		task->use_tag = 1;
		task->destroy = 0;

		/* Tell that to be completely finished we need to have finished this twisted3 */
		starpu_tag_declare_deps(STEP_TAG_2D(plan, END, 0, 0),
				1, STEP_TAG(TWIST3));
#undef STEP_TAG
	}

	/* Create end task */
	plan->end_task = task = starpu_task_create();
	task->cl = NULL;
	task->tag_id = STEP_TAG_2D(plan, END, 0, 0);
	task->use_tag = 1;
	task->destroy = 0;

}

	return plan;
}

/* Actually submit all the tasks. */
static struct starpu_task *
STARPUFFT(start2dC2C)(STARPUFFT(plan) plan, starpu_data_handle_t in, starpu_data_handle_t out)
{
	STARPU_ASSERT(plan->type == C2C);
	int z;
	int ret;

if (PARALLEL) {
	for (z=0; z < plan->totsize1; z++) {
		ret = starpu_task_submit(plan->twist1_tasks[z]);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
		ret = starpu_task_submit(plan->fft1_tasks[z]);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	ret = starpu_task_submit(plan->join_task);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	for (z=0; z < plan->totsize3; z++) {
		ret = starpu_task_submit(plan->twist2_tasks[z]);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
		ret = starpu_task_submit(plan->fft2_tasks[z]);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
		ret = starpu_task_submit(plan->twist3_tasks[z]);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}

	ret = starpu_task_submit(plan->end_task);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

	return plan->end_task;
} else /* !PARALLEL */ {
	struct starpu_task *task;

	/* Create FFT task */
	task = starpu_task_create();
	task->detach = 0;
	task->cl = &STARPUFFT(fft_2d_codelet);
	task->handles[0] = in;
	task->handles[1] = out;
	task->cl_arg = plan;

	ret = starpu_task_submit(task);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	return task;
}
}

/* Free all the tags. The generic code handles freeing the buffers. */
static void
STARPUFFT(free_2d_tags)(STARPUFFT(plan) plan)
{
	unsigned i, j;
	int n1 = plan->n1[0];
	int m1 = plan->n1[1];

	if (!PARALLEL)
		return;

	for (i = 0; i < n1; i++) {
		for (j = 0; j < m1; j++) {
			starpu_tag_remove(STEP_TAG_2D(plan, TWIST1, i, j));
			starpu_tag_remove(STEP_TAG_2D(plan, FFT1, i, j));
		}
	}

	starpu_tag_remove(STEP_TAG_2D(plan, JOIN, 0, 0));

	for (i = 0; i < DIV_2D_N; i++) {
		for (j = 0; j < DIV_2D_M; j++) {
			starpu_tag_remove(STEP_TAG_2D(plan, TWIST2, i, j));
			starpu_tag_remove(STEP_TAG_2D(plan, FFT2, i, j));
			starpu_tag_remove(STEP_TAG_2D(plan, TWIST3, i, j));
		}
	}

	starpu_tag_remove(STEP_TAG_2D(plan, END, 0, 0));
}
