/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010  Mehdi Juhoor <mjuhoor@gmail.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>

#include <starpu.h>
#include <fftw3.h>
#ifdef STARPU_USE_CUDA
#include <cufft.h>
#endif

/* #define SAVE_RAW	1 */

#define DEFAULTINPUTFILE	"input.wav"
#define DEFAULTOUTPUTFILE	"output.wav"
#define NSAMPLES	(256*1024)
#define SAMPLERATE	44100

static unsigned nsamples = NSAMPLES;

/* This is a band filter, we want to stop everything that is not between LOWFREQ and HIGHFREQ*/
/* LOWFREQ < i * SAMPLERATE / NSAMPLE */
#define LOWFREQ	500U
#define HIFREQ	800U

static const size_t headersize = 37+9;

static FILE *infile, *outfile;
static FILE *infile_raw, *outfile_raw;
static char *inputfilename = DEFAULTINPUTFILE;
static char *outputfilename = DEFAULTOUTPUTFILE;
static unsigned use_pin = 0;

unsigned length_data;

/* buffer containing input WAV data */
float *A;

starpu_data_handle_t A_handle;

/* For performance evaluation */
static struct timeval start;
static struct timeval end;
static unsigned task_per_worker[STARPU_NMAXWORKERS] = {0};

/* 
 *	Functions to Manipulate WAV files 
 */

unsigned get_wav_data_bytes_length(FILE *file)
{
	/* this is clearly suboptimal !! */
	fseek(file, headersize, SEEK_SET);

	unsigned cnt = 0;
	while (fgetc(file) != EOF)
		cnt++;

	return cnt;
}

void copy_wav_header(FILE *srcfile, FILE *dstfile)
{
	unsigned char buffer[128];

	fseek(srcfile, 0, SEEK_SET);
	fseek(dstfile, 0, SEEK_SET);

	fread(buffer, 1, headersize, infile);	
	fwrite(buffer, 1, headersize, outfile);	
}

void read_16bit_wav(FILE *infile, unsigned size, float *arrayout, FILE *save_file)
{
	int v;
#if SAVE_RAW
	unsigned currentpos = 0;
#endif

	/* we skip the header to only keep the data */
	fseek(infile, headersize, SEEK_SET);
	
	for (v=0;v<size;v++)
	{
		signed char val = (signed char)fgetc(infile);
		signed char val2 = (signed char)fgetc(infile);

		arrayout[v] = 256*val2 + val;

#if SAVE_RAW
		fprintf(save_file, "%d %f\n", currentpos++, arrayout[v]);
#endif
	}
}

/* we only write the data, not the header !*/
void write_16bit_wav(FILE *outfile, unsigned size, float *arrayin, FILE *save_file)
{
	int v;
#if SAVE_RAW
	unsigned currentpos = 0;
#endif

	/* we assume that the header is copied using copy_wav_header */
	fseek(outfile, headersize, SEEK_SET);
	
	for (v=0;v<size;v++)
	{
		signed char val = ((int)arrayin[v]) % 256; 
		signed char val2  = ((int)arrayin[v]) / 256;

		fputc(val, outfile);
		fputc(val2, outfile);

#if SAVE_RAW
		if (save_file)
	                fprintf(save_file, "%d %f\n", currentpos++, arrayin[v]);
#endif
	}
}


/*
 *
 *	The actual kernels
 *
 */

/* we don't reinitialize the CUFFT plan for every kernel, so we "cache" it */
typedef struct
{
	unsigned is_initialized;
#ifdef STARPU_USE_CUDA
	cufftHandle plan;
	cufftHandle inv_plan;
	cufftComplex *localout;
#endif
	fftwf_complex *localout_cpu;
	float *Acopy;
	fftwf_plan plan_cpu;
	fftwf_plan inv_plan_cpu;
} fft_plan_cache;

static fft_plan_cache plans[STARPU_NMAXWORKERS];

#ifdef STARPU_USE_CUDA
static void band_filter_kernel_gpu(void *descr[], __attribute__((unused)) void *arg)
{
	cufftResult cures;

	float *localA = (float *)STARPU_VECTOR_GET_PTR(descr[0]);
	cufftComplex *localout;

	int workerid = starpu_worker_get_id();
	
	/* initialize the plane only during the first iteration */
	if (!plans[workerid].is_initialized)
	{
		cures = cufftPlan1d(&plans[workerid].plan, nsamples, CUFFT_R2C, 1);
		STARPU_ASSERT(cures == CUFFT_SUCCESS);

		cures = cufftPlan1d(&plans[workerid].inv_plan, nsamples, CUFFT_C2R, 1);
		STARPU_ASSERT(cures == CUFFT_SUCCESS);

		cudaMalloc((void **)&plans[workerid].localout,
					nsamples*sizeof(cufftComplex));
		STARPU_ASSERT(plans[workerid].localout);

		plans[workerid].is_initialized = 1;
	}

	localout = plans[workerid].localout;

	/* FFT */
	cures = cufftExecR2C(plans[workerid].plan, localA, localout);
	STARPU_ASSERT(cures == CUFFT_SUCCESS);
	
	/* filter low freqs */
	unsigned lowfreq_index = (LOWFREQ*nsamples)/SAMPLERATE;
	cudaMemset(&localout[0], 0, lowfreq_index*sizeof(fftwf_complex));

	/* filter high freqs */
	unsigned hifreq_index = (HIFREQ*nsamples)/SAMPLERATE;
	cudaMemset(&localout[hifreq_index], nsamples/2, (nsamples/2 - hifreq_index)*sizeof(fftwf_complex));

	/* inverse FFT */
	cures = cufftExecC2R(plans[workerid].inv_plan, localout, localA);
	STARPU_ASSERT(cures == CUFFT_SUCCESS);

	/* FFTW does not normalize its output ! */
	cublasSscal (nsamples, 1.0f/nsamples, localA, 1);
}
#endif

static pthread_mutex_t fftw_mutex = PTHREAD_MUTEX_INITIALIZER;

static void band_filter_kernel_cpu(void *descr[], __attribute__((unused)) void *arg)
{
	float *localA = (float *)STARPU_VECTOR_GET_PTR(descr[0]);

	int workerid = starpu_worker_get_id();
	
	/* initialize the plane only during the first iteration */
	if (!plans[workerid].is_initialized)
	{
		plans[workerid].localout_cpu = malloc(nsamples*sizeof(fftwf_complex)); 
		plans[workerid].Acopy = malloc(nsamples*sizeof(float));

		/* create plans, only "fftwf_execute" is thread safe in FFTW ... */
		pthread_mutex_lock(&fftw_mutex);
		plans[workerid].plan_cpu = fftwf_plan_dft_r2c_1d(nsamples,
					plans[workerid].Acopy,
					plans[workerid].localout_cpu,
					FFTW_ESTIMATE);
		plans[workerid].inv_plan_cpu = fftwf_plan_dft_c2r_1d(nsamples,
					plans[workerid].localout_cpu,
					plans[workerid].Acopy,
					FFTW_ESTIMATE);
		pthread_mutex_unlock(&fftw_mutex);

		plans[workerid].is_initialized = 1;
	}

	fftwf_complex *localout = plans[workerid].localout_cpu;

	/* copy data into the temporary buffer */
	memcpy(plans[workerid].Acopy, localA, nsamples*sizeof(float));

	/* FFT */
	fftwf_execute(plans[workerid].plan_cpu);
	
	/* filter low freqs */
	unsigned lowfreq_index = (LOWFREQ*nsamples)/SAMPLERATE;
	memset(&localout[0], 0, lowfreq_index*sizeof(fftwf_complex));

	/* filter high freqs */
	unsigned hifreq_index = (HIFREQ*nsamples)/SAMPLERATE;
	memset(&localout[hifreq_index], nsamples/2, (nsamples/2 - hifreq_index)*sizeof(fftwf_complex));

	/* inverse FFT */
	fftwf_execute(plans[workerid].inv_plan_cpu);

	/* copy data into the temporary buffer */
	memcpy(localA, plans[workerid].Acopy, nsamples*sizeof(float));

	/* FFTW does not normalize its output ! */
	/* TODO use BLAS ?*/
	int i;
	for (i = 0; i < nsamples; i++)
		localA[i] /= nsamples;
}

struct starpu_perfmodel band_filter_model =
{
	.type = STARPU_HISTORY_BASED,
	.symbol = "FFT_band_filter"
};

static struct starpu_codelet band_filter_cl =
{
	.modes = { STARPU_RW },
	.where = STARPU_CPU|STARPU_CUDA,
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {band_filter_kernel_gpu, NULL},
#endif
	.cpu_funcs = {band_filter_kernel_cpu, NULL},
	.model = &band_filter_model,
	.nbuffers = 1
};

void callback(void *arg)
{
	/* do some accounting */
	int id = starpu_worker_get_id();
	task_per_worker[id]++;
}

void create_starpu_task(unsigned iter)
{
	int ret;
	struct starpu_task *task = starpu_task_create();

	task->cl = &band_filter_cl;

	task->handles[0] = starpu_data_get_sub_data(A_handle, 1, iter);

	task->callback_func = callback;
	task->callback_arg = NULL;

	ret = starpu_task_submit(task);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
}

static void init_problem(void)
{
	infile = fopen(inputfilename, "r");

	if (outputfilename)
		outfile = fopen(outputfilename, "w+");

#if SAVE_RAW
	infile_raw = fopen("input.raw", "w");
	outfile_raw = fopen("output.raw", "w");
#endif

	/* copy input's header into output WAV  */
	if (outputfilename)
		copy_wav_header(infile, outfile);

	/* read length of input WAV's data */
	/* each element is 2 bytes long (16bits)*/
	length_data = get_wav_data_bytes_length(infile)/2;

	/* allocate a buffer to store the content of input file */
	if (use_pin)
	{
		starpu_malloc((void **)&A, length_data*sizeof(float));
	}
	else
	{
		A = malloc(length_data*sizeof(float));
	}

	/* allocate working buffer (this could be done online, but we'll keep it simple) */
	/* starpu_data_malloc_pinned_if_possible((void **)&outdata, length_data*sizeof(fftwf_complex)); */

	/* read input data into buffer "A" */
	read_16bit_wav(infile, length_data, A, infile_raw);
}

static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0)
		{
			fprintf(stderr, "Usage: %s [-pin] [-nsamples block_size] [-i input.wav] [-o output.wav | -no-output] [-h]\n", argv[0]);
			exit(-1);
		}

		if (strcmp(argv[i], "-i") == 0)
		{
			inputfilename = argv[++i];;
		}

		if (strcmp(argv[i], "-o") == 0)
		{
			outputfilename = argv[++i];;
		}

		if (strcmp(argv[i], "-no-output") == 0)
		{
			outputfilename = NULL;;
		}

		/* block size */
		if (strcmp(argv[i], "-nsamples") == 0)
		{
			char *argptr;
			nsamples = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-pin") == 0)
		{
			use_pin = 1;
		}
	}
}

int main(int argc, char **argv)
{
	unsigned iter;
	int ret;

	parse_args(argc, argv);

	fprintf(stderr, "Reading input data\n");

	init_problem();

	unsigned niter = length_data/nsamples;

	fprintf(stderr, "input: %s\noutput: %s\n#chunks %d\n", inputfilename, outputfilename, niter);

	/* launch StarPU */
	ret = starpu_init(NULL);
	if (ret == -ENODEV)
		return 77;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_vector_data_register(&A_handle, 0, (uintptr_t)A, niter*nsamples, sizeof(float));

	struct starpu_data_filter f =
	{
		.filter_func = starpu_block_filter_func_vector,
		.nchildren = niter
	};

	starpu_data_partition(A_handle, &f);

	for (iter = 0; iter < niter; iter++)
		starpu_data_set_wt_mask(starpu_data_get_sub_data(A_handle, 1, iter), 1<<0);

	gettimeofday(&start, NULL);

	for (iter = 0; iter < niter; iter++)
	{
		create_starpu_task(iter);
	}

	starpu_task_wait_for_all();

	gettimeofday(&end, NULL);

	double timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));
	fprintf(stderr, "Computation took %2.2f ms\n", timing/1000);

	int worker;
	for (worker = 0; worker < STARPU_NMAXWORKERS; worker++)
	{
		if (task_per_worker[worker])
		{
			char name[32];
			starpu_worker_get_name(worker, name, 32);

			unsigned long bytes = nsamples*sizeof(float)*task_per_worker[worker];

			fprintf(stderr, "\t%s -> %2.2f MB\t%2.2f\tMB/s\t%2.2f %%\n", name, (1.0*bytes)/(1024*1024), bytes/timing, (100.0*task_per_worker[worker])/niter);
		}
	}

	if (outputfilename)
		fprintf(stderr, "Writing output data\n");

	/* make sure that the output is in RAM before quitting StarPU */
	starpu_data_unpartition(A_handle, 0);
	starpu_data_unregister(A_handle);

	/* we are done ! */
	starpu_shutdown();

	fclose(infile);

	if (outputfilename)
	{
		write_16bit_wav(outfile, length_data, A, outfile_raw);
		fclose(outfile);
	}

#if SAVE_RAW
	fclose(infile_raw);
	fclose(outfile_raw);
#endif

	return 0;
}
