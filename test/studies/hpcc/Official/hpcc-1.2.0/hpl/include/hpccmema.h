/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; fill-column: 79; coding: iso-latin-1-unix -*- */

#ifndef HPCCMEMA_H
#define HPCCMEMA_H 1

#ifdef HPCC_MEMALLCTR
extern int HPCC_alloc_init(size_t total_size);
extern int HPCC_alloc_finalize();
extern void *HPCC_malloc(size_t size);
extern void HPCC_free(void *ptr);
#define HPCC_fftw_malloc HPCC_malloc
#define HPCC_fftw_free HPCC_free
#define HPCC_XMALLOC(t,s) ((t*)HPCC_malloc(sizeof(t)*(s)))
#else
#define HPCC_malloc malloc
#define HPCC_free free
#define HPCC_fftw_malloc fftw_malloc
#define HPCC_fftw_free fftw_free
#define HPCC_XMALLOC(t,s) XMALLOC(t,s)
#endif

#endif
