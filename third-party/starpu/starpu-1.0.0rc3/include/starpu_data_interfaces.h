/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
 * Copyright (C) 2011-2012  Institut National de Recherche en Informatique et Automatique
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

#ifndef __STARPU_DATA_INTERFACES_H__
#define __STARPU_DATA_INTERFACES_H__

#include <starpu.h>

#ifdef STARPU_USE_GORDON
/* to get the gordon_strideSize_t data structure from gordon */
#include <gordon.h>
#endif

#ifdef STARPU_USE_CUDA
/* to use CUDA streams */
# ifdef STARPU_DONT_INCLUDE_CUDA_HEADERS
typedef void *cudaStream_t;
# else
#  include <cuda_runtime.h>
# endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* The following structures are used to describe data interfaces */

/* This structure contains the different methods to transfer data between the
 * different types of memory nodes */
struct starpu_data_copy_methods
{
	/* src type is ram */
	int (*ram_to_ram)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*ram_to_cuda)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*ram_to_opencl)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*ram_to_spu)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);

	/* src type is cuda */
	int (*cuda_to_ram)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*cuda_to_cuda)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*cuda_to_opencl)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*cuda_to_spu)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);

	/* src type is spu */
	int (*spu_to_ram)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*spu_to_cuda)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*spu_to_opencl)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*spu_to_spu)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);

	/* src type is opencl */
	int (*opencl_to_ram)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*opencl_to_cuda)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*opencl_to_opencl)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);
	int (*opencl_to_spu)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node);

#ifdef STARPU_USE_CUDA
	/* for asynchronous CUDA transfers */
	int (*ram_to_cuda_async)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, cudaStream_t stream);
	int (*cuda_to_ram_async)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, cudaStream_t stream);
	int (*cuda_to_cuda_async)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, cudaStream_t stream);
#endif

#ifdef STARPU_USE_OPENCL
	/* for asynchronous OpenCL transfers */
	/* XXX we do not use a cl_event *event type for the last argument
	 * because nvcc does not like when we have to include OpenCL headers */
        int (*ram_to_opencl_async)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, /* cl_event * */ void *event);
	int (*opencl_to_ram_async)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, /* cl_event * */ void *event);
	int (*opencl_to_opencl_async)(void *src_interface, unsigned src_node, void *dst_interface, unsigned dst_node, /* cl_event * */ void *event);
#endif
};

enum starpu_data_interface_id
{
	STARPU_MATRIX_INTERFACE_ID=0,
	STARPU_BLOCK_INTERFACE_ID=1,
	STARPU_VECTOR_INTERFACE_ID=2,
	STARPU_CSR_INTERFACE_ID=3,
	STARPU_BCSR_INTERFACE_ID=4,
	STARPU_VARIABLE_INTERFACE_ID=5,
	STARPU_VOID_INTERFACE_ID=6,
	STARPU_MULTIFORMAT_INTERFACE_ID=7,
	STARPU_MAX_INTERFACE_ID=8 /* maximum number of data interfaces */
};

struct starpu_data_interface_ops
{
	/* Register an existing interface into a data handle. */
	void (*register_data_handle)(starpu_data_handle_t handle,
					uint32_t home_node, void *data_interface);
	/* Allocate data for the interface on a given node. */
	starpu_ssize_t (*allocate_data_on_node)(void *data_interface, uint32_t node);
	/* Free data of the interface on a given node. */
	void (*free_data_on_node)(void *data_interface, uint32_t node);
	/* ram/cuda/spu/opencl synchronous and asynchronous transfer methods */
	const struct starpu_data_copy_methods *copy_methods;
	/* Return the current pointer (if any) for the handle on the given node. */
	void * (*handle_to_pointer)(starpu_data_handle_t handle, uint32_t node);
	/* Return an estimation of the size of data, for performance models */
	size_t (*get_size)(starpu_data_handle_t handle);
	/* Return a 32bit footprint which characterizes the data size */
	uint32_t (*footprint)(starpu_data_handle_t handle);
	/* Compare the data size of two interfaces */
	int (*compare)(void *data_interface_a, void *data_interface_b);
	/* Dump the sizes of a handle to a file */
	void (*display)(starpu_data_handle_t handle, FILE *f);
#ifdef STARPU_USE_GORDON
	/* Convert the data size to the spu size format */
	int (*convert_to_gordon)(void *data_interface, uint64_t *ptr, gordon_strideSize_t *ss); 
#endif
	/* an identifier that is unique to each interface */
	enum starpu_data_interface_id interfaceid;
	/* The size of the interface data descriptor */
	size_t interface_size;

	int is_multiformat;
	struct starpu_multiformat_data_interface_ops* (*get_mf_ops)(void *data_interface);
};

/* Return the next available id for a data interface */
int starpu_data_interface_get_next_id();

void starpu_data_register(starpu_data_handle_t *handleptr, uint32_t home_node, void *data_interface, struct starpu_data_interface_ops *ops);
void starpu_data_register_same(starpu_data_handle_t *handledst, starpu_data_handle_t handlesrc);

/* Return the pointer associated with HANDLE on node NODE or NULL if HANDLE's
 * interface does not support this operation or data for this handle is not
 * allocated on that node. */
void *starpu_handle_to_pointer(starpu_data_handle_t handle, uint32_t node);

/* Return the local pointer associated with HANDLE or NULL if HANDLE's
 * interface does not have data allocated locally */
void *starpu_handle_get_local_ptr(starpu_data_handle_t handle);

/* "node" means memory node: 0 for main RAM, then 1, 2, etc. for various GPUs,
 * etc.
 *
 * On registration, the source of data is usually a pointer in RAM, in which
 * case 0 should be passed.
 */

void *starpu_data_get_interface_on_node(starpu_data_handle_t handle, unsigned memory_node);

#ifdef STARPU_DEVEL
#  warning the declaration below is needed for the spvm example (dw_block_spmv.c:110) which filters a data in sub-data with a different interface. However exposing a private object is certainly not something to do. Either turn the object public or find another way to specify interfaces when filtering
#endif /* STARPU_DEVEL */
extern struct starpu_data_interface_ops _starpu_interface_matrix_ops;

/* Matrix interface for dense matrices */
struct starpu_matrix_interface
{
	uintptr_t ptr;
        uintptr_t dev_handle;
        size_t offset;
	uint32_t nx;
	uint32_t ny;
	uint32_t ld;
	size_t elemsize;
};

void starpu_matrix_data_register(starpu_data_handle_t *handle, uint32_t home_node, uintptr_t ptr, uint32_t ld, uint32_t nx, uint32_t ny, size_t elemsize);
uint32_t starpu_matrix_get_nx(starpu_data_handle_t handle);
uint32_t starpu_matrix_get_ny(starpu_data_handle_t handle);
uint32_t starpu_matrix_get_local_ld(starpu_data_handle_t handle);
uintptr_t starpu_matrix_get_local_ptr(starpu_data_handle_t handle);
size_t starpu_matrix_get_elemsize(starpu_data_handle_t handle);

/* helper methods */
#define STARPU_MATRIX_GET_PTR(interface)	(((struct starpu_matrix_interface *)(interface))->ptr)
#define STARPU_MATRIX_GET_DEV_HANDLE(interface)	(((struct starpu_matrix_interface *)(interface))->dev_handle)
#define STARPU_MATRIX_GET_OFFSET(interface)	(((struct starpu_matrix_interface *)(interface))->offset)
#define STARPU_MATRIX_GET_NX(interface)	(((struct starpu_matrix_interface *)(interface))->nx)
#define STARPU_MATRIX_GET_NY(interface)	(((struct starpu_matrix_interface *)(interface))->ny)
#define STARPU_MATRIX_GET_LD(interface)	(((struct starpu_matrix_interface *)(interface))->ld)
#define STARPU_MATRIX_GET_ELEMSIZE(interface)	(((struct starpu_matrix_interface *)(interface))->elemsize)


/* BLOCK interface for 3D dense blocks */
/* TODO: rename to 3dmatrix? */
struct starpu_block_interface
{
	uintptr_t ptr;
        uintptr_t dev_handle;
        size_t offset;
	uint32_t nx;
	uint32_t ny;
	uint32_t nz;
	uint32_t ldy;	/* number of elements between two lines */
	uint32_t ldz;	/* number of elements between two planes */
	size_t elemsize;
};

void starpu_block_data_register(starpu_data_handle_t *handle, uint32_t home_node, uintptr_t ptr, uint32_t ldy, uint32_t ldz, uint32_t nx, uint32_t ny, uint32_t nz, size_t elemsize);
uint32_t starpu_block_get_nx(starpu_data_handle_t handle);
uint32_t starpu_block_get_ny(starpu_data_handle_t handle);
uint32_t starpu_block_get_nz(starpu_data_handle_t handle);
uint32_t starpu_block_get_local_ldy(starpu_data_handle_t handle);
uint32_t starpu_block_get_local_ldz(starpu_data_handle_t handle);
uintptr_t starpu_block_get_local_ptr(starpu_data_handle_t handle);
size_t starpu_block_get_elemsize(starpu_data_handle_t handle);

/* helper methods */
#define STARPU_BLOCK_GET_PTR(interface)	(((struct starpu_block_interface *)(interface))->ptr)
#define STARPU_BLOCK_GET_DEV_HANDLE(interface)	(((struct starpu_block_interface *)(interface))->dev_handle)
#define STARPU_BLOCK_GET_OFFSET(interface)	(((struct starpu_block_interface *)(interface))->offset)
#define STARPU_BLOCK_GET_NX(interface)	(((struct starpu_block_interface *)(interface))->nx)
#define STARPU_BLOCK_GET_NY(interface)	(((struct starpu_block_interface *)(interface))->ny)
#define STARPU_BLOCK_GET_NZ(interface)	(((struct starpu_block_interface *)(interface))->nz)
#define STARPU_BLOCK_GET_LDY(interface)	(((struct starpu_block_interface *)(interface))->ldy)
#define STARPU_BLOCK_GET_LDZ(interface)	(((struct starpu_block_interface *)(interface))->ldz)
#define STARPU_BLOCK_GET_ELEMSIZE(interface)	(((struct starpu_block_interface *)(interface))->elemsize)

/* vector interface for contiguous (non-strided) buffers */
struct starpu_vector_interface
{
	uintptr_t ptr;
        uintptr_t dev_handle;
        size_t offset;
	uint32_t nx;
	size_t elemsize;
};

void starpu_vector_data_register(starpu_data_handle_t *handle, uint32_t home_node, uintptr_t ptr, uint32_t nx, size_t elemsize);
uint32_t starpu_vector_get_nx(starpu_data_handle_t handle);
size_t starpu_vector_get_elemsize(starpu_data_handle_t handle);
uintptr_t starpu_vector_get_local_ptr(starpu_data_handle_t handle);

/* helper methods */
#define STARPU_VECTOR_GET_PTR(interface)	(((struct starpu_vector_interface *)(interface))->ptr)
#define STARPU_VECTOR_GET_DEV_HANDLE(interface)	(((struct starpu_vector_interface *)(interface))->dev_handle)
#define STARPU_VECTOR_GET_OFFSET(interface)	(((struct starpu_vector_interface *)(interface))->offset)
#define STARPU_VECTOR_GET_NX(interface)	(((struct starpu_vector_interface *)(interface))->nx)
#define STARPU_VECTOR_GET_ELEMSIZE(interface)	(((struct starpu_vector_interface *)(interface))->elemsize)

/* variable interface for a single data (not a vector, a matrix, a list, ...) */
struct starpu_variable_interface
{
	uintptr_t ptr;
	size_t elemsize;
	/* No dev_handle, since it can not be filtered, offset will always be zero */
};

void starpu_variable_data_register(starpu_data_handle_t *handle, uint32_t home_node, uintptr_t ptr, size_t size);
size_t starpu_variable_get_elemsize(starpu_data_handle_t handle);
uintptr_t starpu_variable_get_local_ptr(starpu_data_handle_t handle);

/* helper methods */
#define STARPU_VARIABLE_GET_PTR(interface)	(((struct starpu_variable_interface *)(interface))->ptr)
#define STARPU_VARIABLE_GET_ELEMSIZE(interface)	(((struct starpu_variable_interface *)(interface))->elemsize)

/* void interface. There is no data really associated to that interface, but it
 * may be used as a synchronization mechanism. It also permits to express an
 * abstract piece of data that is managed by the application internally: this
 * makes it possible to forbid the concurrent execution of different tasks
 * accessing the same "void" data in read-write concurrently. */
void starpu_void_data_register(starpu_data_handle_t *handle);

/* CSR interface for sparse matrices (compressed sparse row representation) */
struct starpu_csr_interface
{
	uint32_t nnz; /* number of non-zero entries */
	uint32_t nrow; /* number of rows */
	uintptr_t nzval; /* non-zero values */
	uint32_t *colind; /* position of non-zero entries on the row */
	uint32_t *rowptr; /* index (in nzval) of the first entry of the row */

        /* k for k-based indexing (0 or 1 usually) */
        /* also useful when partitionning the matrix ... */
        uint32_t firstentry;

	size_t elemsize;
};

void starpu_csr_data_register(starpu_data_handle_t *handle, uint32_t home_node, uint32_t nnz, uint32_t nrow, uintptr_t nzval, uint32_t *colind, uint32_t *rowptr, uint32_t firstentry, size_t elemsize);
uint32_t starpu_csr_get_nnz(starpu_data_handle_t handle);
uint32_t starpu_csr_get_nrow(starpu_data_handle_t handle);
uint32_t starpu_csr_get_firstentry(starpu_data_handle_t handle);
uintptr_t starpu_csr_get_local_nzval(starpu_data_handle_t handle);
uint32_t *starpu_csr_get_local_colind(starpu_data_handle_t handle);
uint32_t *starpu_csr_get_local_rowptr(starpu_data_handle_t handle);
size_t starpu_csr_get_elemsize(starpu_data_handle_t handle);

#define STARPU_CSR_GET_NNZ(interface)	(((struct starpu_csr_interface *)(interface))->nnz)
#define STARPU_CSR_GET_NROW(interface)	(((struct starpu_csr_interface *)(interface))->nrow)
#define STARPU_CSR_GET_NZVAL(interface)	(((struct starpu_csr_interface *)(interface))->nzval)
#define STARPU_CSR_GET_COLIND(interface)	(((struct starpu_csr_interface *)(interface))->colind)
#define STARPU_CSR_GET_ROWPTR(interface)	(((struct starpu_csr_interface *)(interface))->rowptr)
#define STARPU_CSR_GET_FIRSTENTRY(interface)	(((struct starpu_csr_interface *)(interface))->firstentry)
#define STARPU_CSR_GET_ELEMSIZE(interface)	(((struct starpu_csr_interface *)(interface))->elemsize)

/* BCSR interface for sparse matrices (blocked compressed sparse row
 * representation) */
struct starpu_bcsr_interface
{
	uint32_t nnz; /* number of non-zero BLOCKS */
	uint32_t nrow; /* number of rows (in terms of BLOCKS) */

	uintptr_t nzval; /* non-zero values */
	uint32_t *colind; /* position of non-zero entried on the row */
/*	uint32_t *rowind; */ /* position of non-zero entried on the col */
	uint32_t *rowptr; /* index (in nzval) of the first entry of the row */

        /* k for k-based indexing (0 or 1 usually) */
        /* also useful when partitionning the matrix ... */
        uint32_t firstentry;

	/* size of the blocks */
	uint32_t r;
	uint32_t c;

	size_t elemsize;
};

void starpu_bcsr_data_register(starpu_data_handle_t *handle, uint32_t home_node, uint32_t nnz, uint32_t nrow, uintptr_t nzval, uint32_t *colind, uint32_t *rowptr, uint32_t firstentry, uint32_t r, uint32_t c, size_t elemsize);

#define STARPU_BCSR_GET_NNZ(interface)        (((struct starpu_bcsr_interface *)(interface))->nnz)
#define STARPU_BCSR_GET_NZVAL(interface)      (((struct starpu_bcsr_interface *)(interface))->nzval)
#define STARPU_BCSR_GET_COLIND(interface)     (((struct starpu_bcsr_interface *)(interface))->colind)
#define STARPU_BCSR_GET_ROWPTR(interface)     (((struct starpu_bcsr_interface *)(interface))->rowptr)
uint32_t starpu_bcsr_get_nnz(starpu_data_handle_t handle);
uint32_t starpu_bcsr_get_nrow(starpu_data_handle_t handle);
uint32_t starpu_bcsr_get_firstentry(starpu_data_handle_t handle);
uintptr_t starpu_bcsr_get_local_nzval(starpu_data_handle_t handle);
uint32_t *starpu_bcsr_get_local_colind(starpu_data_handle_t handle);
uint32_t *starpu_bcsr_get_local_rowptr(starpu_data_handle_t handle);
uint32_t starpu_bcsr_get_r(starpu_data_handle_t handle);
uint32_t starpu_bcsr_get_c(starpu_data_handle_t handle);
size_t starpu_bcsr_get_elemsize(starpu_data_handle_t handle);

/*
 * Multiformat interface
 */
struct starpu_multiformat_data_interface_ops
{
	size_t cpu_elemsize;
	size_t opencl_elemsize;
	struct starpu_codelet *cpu_to_opencl_cl;
	struct starpu_codelet *opencl_to_cpu_cl;
	size_t cuda_elemsize;
	struct starpu_codelet *cpu_to_cuda_cl;
	struct starpu_codelet *cuda_to_cpu_cl;
};

struct starpu_multiformat_interface
{
	void *cpu_ptr;
	void *cuda_ptr;
	void *opencl_ptr;
	uint32_t nx;
	struct starpu_multiformat_data_interface_ops *ops;
};

void starpu_multiformat_data_register(starpu_data_handle_t *handle, uint32_t home_node, void *ptr, uint32_t nobjects, struct starpu_multiformat_data_interface_ops *format_ops);

#define STARPU_MULTIFORMAT_GET_CPU_PTR(interface)  (((struct starpu_multiformat_interface *)(interface))->cpu_ptr)
#define STARPU_MULTIFORMAT_GET_CUDA_PTR(interface) (((struct starpu_multiformat_interface *)(interface))->cuda_ptr)
#define STARPU_MULTIFORMAT_GET_OPENCL_PTR(interface) (((struct starpu_multiformat_interface *)(interface))->opencl_ptr)
#define STARPU_MULTIFORMAT_GET_NX(interface)  (((struct starpu_multiformat_interface *)(interface))->nx)

enum starpu_data_interface_id starpu_handle_get_interface_id(starpu_data_handle_t handle);

/* Lookup a ram pointer into a StarPU handle */
extern starpu_data_handle_t starpu_data_lookup(const void *ptr);


#ifdef __cplusplus
}
#endif

#endif /* __STARPU_DATA_INTERFACES_H__ */
