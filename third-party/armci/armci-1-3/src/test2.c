/* $Id: test2.c,v 1.1.2.1 2007/05/29 19:35:26 manoj Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

#define ARMCI_INT     -99
#define ARMCI_LONG    -101
#define ARMCI_FLOAT   -306
#define ARMCI_DOUBLE  -307

#define FLOAT_EPS  ((float) 1.0 / 4096)
#define DOUBLE_EPS ((double) 1.0 / 16384)

#ifdef WIN32
#  include <windows.h>
#  define sleep(x) Sleep(1000*(x))
#else
#  include <unistd.h>
#endif

/* ARMCI is impartial to message-passing libs - we handle them with MP macros */
#if defined(PVM)
#   include <pvm3.h>
#   ifdef CRAY
#     define MPGROUP         (char *)NULL
#     define MP_INIT(arc,argv)
#   else
#     define MPGROUP           "mp_working_group"
#     define MP_INIT(arc,argv) pvm_init(arc, argv)
#   endif
#   define MP_FINALIZE()     pvm_exit()
#   define MP_BARRIER()      pvm_barrier(MPGROUP,-1)
#   define MP_MYID(pid)      *(pid)   = pvm_getinst(MPGROUP,pvm_mytid())
#   define MP_PROCS(pproc)   *(pproc) = (int)pvm_gsize(MPGROUP)
void pvm_init(int argc, char *argv[]);
#elif defined(TCGMSG)
#   include <sndrcv.h>
long tcg_tag =30000;
#   define MP_BARRIER()      SYNCH_(&tcg_tag)
#   define MP_INIT(arc,argv) PBEGIN_((argc),(argv))
#   define MP_FINALIZE()     PEND_()
#   define MP_MYID(pid)      *(pid)   = (int)NODEID_()
#   define MP_PROCS(pproc)   *(pproc) = (int)NNODES_()
#elif defined(BGML)
# include<bgml.h>
# include <mpi.h>
#   define MP_BARRIER()      armci_msg_barrier()
# define MP_FINALIZE()      MPI_Finalize()
# define MP_INIT(arc, argv)    MPI_Init(&(argc), &(argv))
# define MP_MYID(pid)      MPI_Comm_rank(MPI_COMM_WORLD, (pid))
# define MP_PROCS(pproc)   MPI_Comm_size(MPI_COMM_WORLD, (pproc));
#else
#   include <mpi.h>
#   define MP_BARRIER()      MPI_Barrier(MPI_COMM_WORLD)
#   define MP_FINALIZE()     MPI_Finalize()
#   define MP_INIT(arc,argv) MPI_Init(&(argc),&(argv))
#   define MP_MYID(pid)      MPI_Comm_rank(MPI_COMM_WORLD, (pid))
#   define MP_PROCS(pproc)   MPI_Comm_size(MPI_COMM_WORLD, (pproc));
#endif

#include "armci.h"

#define DIM1 5
#define DIM2 3
#ifdef __sun
/* Solaris has shared memory shortages in the default system configuration */
# define DIM3 6
# define DIM4 5
# define DIM5 4
#elif defined(__alpha__)
# define DIM3 8
# define DIM4 5
# define DIM5 6
#else
# define DIM3 8
# define DIM4 9
# define DIM5 7
#endif
#define DIM6 3
#define DIM7 2


#define OFF 1
#define EDIM1 (DIM1+OFF)
#define EDIM2 (DIM2+OFF)
#define EDIM3 (DIM3+OFF)
#define EDIM4 (DIM4+OFF)
#define EDIM5 (DIM5+OFF)
#define EDIM6 (DIM6+OFF)
#define EDIM7 (DIM7+OFF)

#define DIMS 4
#define MAXDIMS 7
#define MAX_DIM_VAL 50 
#define LOOP 200

#define BASE 100.
#define MAXPROC 1024
#define TIMES 100

#ifdef CRAY
# define ELEMS 800
#else
# define ELEMS 200
#endif

typedef struct {
      float real;
      float imag;
} cmpl_t;

typedef struct {
      double real;
      double imag;
} dcmpl_t;

/***************************** macros ************************/
#define COPY(src, dst, bytes) memcpy((dst),(src),(bytes))
#define MAX(a,b) (((a) >= (b)) ? (a) : (b))
#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define ABS(a) (((a) <0) ? -(a) : (a))

/***************************** global data *******************/
int me, nproc;
void* work[MAXPROC]; /* work array for propagating addresses */



#ifdef PVM
void pvm_init(int argc, char *argv[])
{
    int mytid, mygid, ctid[MAXPROC];
    int np, i;

    mytid = pvm_mytid();
    if((argc != 2) && (argc != 1)) goto usage;
    if(argc == 1) np = 1;
    if(argc == 2)
       if((np = atoi(argv[1])) < 1) goto usage;
    if(np > MAXPROC) goto usage;

    mygid = pvm_joingroup(MPGROUP);

    if(np > 1)
       if (mygid == 0) 
          i = pvm_spawn(argv[0], argv+1, 0, "", np-1, ctid);

    while(pvm_gsize(MPGROUP) < np) sleep(1);

    /* sync */
    pvm_barrier(MPGROUP, np);
    
    printf("PVM initialization done!\n");
    
    return;

  usage:
    fprintf(stderr, "usage: %s <nproc>\n", argv[0]);
    pvm_exit();
    exit(-1);
}
#endif

void create_array(void *a[], int elem_size, int ndim, int dims[])
{
    int bytes=elem_size, i, rc;

    assert(ndim<=MAXDIMS);
    for(i=0;i<ndim;i++)bytes*=dims[i];

    rc = ARMCI_Malloc(a, bytes);
    assert(rc==0);
     
    assert(a[me]);
     
}

void destroy_array(void *ptr[])
{
    MP_BARRIER();

    assert(!ARMCI_Free(ptr[me]));
}


int loA[MAXDIMS], hiA[MAXDIMS];
int dimsA[MAXDIMS]={DIM1,DIM2,DIM3,DIM4,DIM5,DIM6, DIM7};
int loB[MAXDIMS], hiB[MAXDIMS];
int dimsB[MAXDIMS]={EDIM1,EDIM2,EDIM3,EDIM4,EDIM5,EDIM6,EDIM7};
int count[MAXDIMS];
int strideA[MAXDIMS], strideB[MAXDIMS];
int loC[MAXDIMS], hiC[MAXDIMS];
int idx[MAXDIMS]={0,0,0,0,0,0,0};


void test_brdcst(int datatype)
{
    void *a[6];
    int len[6] = {1, 10, 100, 1000, 10000, 100000};
    int datatype_size = 0;
    int i, j;

    switch(datatype)
    {
       case ARMCI_INT:
          datatype_size = sizeof(int);
          for(i = 0; i < 6; i++)
             a[i] = malloc(len[i] * datatype_size);
          for(i = 0; i < 6; i++)
             if(me == 0)
                for(j = 0; j < len[i]; j++)
                   ((int *) a[i])[j] = (int) j;
             else
                memset(a[i], 0x0, len[i] * datatype_size);
          break;
       case ARMCI_LONG:
          datatype_size = sizeof(long);
          for(i = 0; i < 6; i++)
             a[i] = malloc(len[i] * datatype_size);
          for(i = 0; i < 6; i++)
             if(me == 0)
                for(j = 0; j < len[i]; j++)
                   ((long *) a[i])[j] = (long) j;
             else
                memset(a[i], 0x0, len[i] * datatype_size);
          break;
       case ARMCI_FLOAT:
          datatype_size = sizeof(float);
          for(i = 0; i < 6; i++)
             a[i] = malloc(len[i] * datatype_size);
          for(i = 0; i < 6; i++)
             if(me == 0)
                for(j = 0; j < len[i]; j++)
                   ((float *) a[i])[j] = (float) j;
             else
                memset(a[i], 0x0, len[i] * datatype_size);
          break;
       case ARMCI_DOUBLE:
          datatype_size = sizeof(double);
          for(i = 0; i < 6; i++)
             a[i] = malloc(len[i] * datatype_size);
          for(i = 0; i < 6; i++)
             if(me == 0)
                for(j = 0; j < len[i]; j++)
                   ((double *) a[i])[j] = (double) j;
             else
                memset(a[i], 0x0, len[i] * datatype_size);
          break;
       default:
          break;
    }
    for(i = 0; i < 6; i++)
       armci_msg_brdcst(a[i], len[i] * datatype_size, 0);

    switch(datatype)
    {
       case ARMCI_INT:
          for(i = 0; i < 6; i++)
             for(j = 0; j < len[i]; j++)
                if(((int *) a[i])[j] != (int) j) 
                {
                   printf("ERROR a[%d][%d] = %d != %d\n", i, j, ((int *) a[i])[j], (int) j);
                   ARMCI_Error("armci_brdcst failed (int)\n",0);
                }
          break;
       case ARMCI_LONG:
          for(i = 0; i < 6; i++)
             for(j = 0; j < len[i]; j++)
                if(((long *) a[i])[j] != (long) j)
                {
                   printf("ERROR a[%d][%d] = %d != %d\n", i, j, ((long *) a[i])[j], (long) j);
                   ARMCI_Error("armci_brdcst failed (long)\n",0);
                }
          break;
       case ARMCI_FLOAT:
          for(i = 0; i < 6; i++)
             for(j = 0; j < len[i]; j++)
                if(((float *) a[i])[j] != (float) j)
                {
                   printf("ERROR a[%d][%d] = %f != %f\n", i, j, ((float *) a[i])[j], (float) j);
                   ARMCI_Error("armci_brdcst failed (float)\n",0);
                }
          break;
       case ARMCI_DOUBLE:
          for(i = 0; i < 6; i++)
             for(j = 0; j < len[i]; j++)
                if(((double *) a[i])[j] != (double) j)
                {
                   printf("ERROR a[%d][%d] = %f != %f\n", i, j, ((double *) a[i])[j], (double) j);
                   ARMCI_Error("armci_brdcst failed (double)\n",0);
                }
          break;
       default:
          break;
    }

    for(i = 0; i < 6; i++)
       free(a[i]);
}

void test_gop2_or_reduce(const int datatype, const char * op, const int reduce_test)
{
    void *a[6];
    int len[6] = {1, 10, 100, 1000, 10000, 100000};
    int len_length = 3;
    int datatype_size = 0;
    int i, j;
    char * test_type;
    int verbose = 0;
    if(reduce_test == 0)
       test_type = "gop2";
    else
       test_type = "reduce";

    switch(datatype)
    {
       case ARMCI_INT:
       case ARMCI_LONG:
          datatype_size = sizeof(int);
          for(i = 0; i < len_length; i++)
             a[i] = malloc(len[i] * datatype_size);
          for(i = 0; i < len_length; i++)
             for(j = 0; j < len[i]; j++)
                ((int *) a[i])[j] = (int) (me + j) * (((me + j) % 2 == 0) ? 1 : -1);
          for(i = 0; i < len_length; i++)
          {
             if(me == 0 && verbose != 0)
                printf("testing %s %s message size = %d op = %s\n", test_type, (datatype == ARMCI_INT ? "ARMCI_INT" : "ARMCI_LONG"), len[i], op);
             if(reduce_test == 0)
                armci_msg_igop(a[i], len[i], op);
             else
                armci_msg_reduce(a[i], len[i], op, datatype);
          }
          if(me == 0 || reduce_test == 0)
             for(i = 0; i < len_length; i++)
             {
                if(me == 0 && verbose != 0)
                   printf("checking %s %s message size = %d op = %s\n", test_type, (datatype == ARMCI_INT ? "ARMCI_INT" : "ARMCI_LONG"), len[i], op);
                for(j = 0; j < len[i]; j++)
                   if(strncmp(op, "+", 1) == 0)
                   {
                      int compare = 0;
                      if(nproc % 2 == 0)
                      {
                         if(j % 2 == 0)
                            compare = -nproc / 2;
                         else
                            compare = nproc / 2;
                      }
                      else
                      {
                         if(j % 2 == 0)
                            compare = j + nproc / 2;
                         else
                            compare = -(j + nproc / 2);
                      }
                      if(((int *) a[i])[j] != compare) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %d != %d\n", test_type, (datatype == ARMCI_INT ? "ARMCI_INT" : "ARMCI_LONG"), op, i, j, ((int *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "*", 1) == 0)
                   {
                      int compare = 1;
                      int k = 0;
                      for(k = 0; k < nproc; k++)
                         compare *= (k + j) * (((k + j) % 2 == 0) ? 1 : -1);
                      if(((int *) a[i])[j] != compare) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %d != %d\n", test_type, (datatype == ARMCI_INT ? "ARMCI_INT" : "ARMCI_LONG"), op, i, j, ((int *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "min", 3) == 0)
                   {
                      int compare = -(j + nproc - 1);
                      if(compare % 2 == 0 && nproc > 1)
                         compare = -(j + nproc - 2);
                      if(((int *) a[i])[j] != compare) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %d != %d\n", test_type, (datatype == ARMCI_INT ? "ARMCI_INT" : "ARMCI_LONG"), op, i, j, ((int *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "max", 3) == 0)
                   {
                      int compare = j + nproc - 1;
                      if(compare % 2 != 0 && nproc > 1)
                         compare = j + nproc - 2;
                      if(((int *) a[i])[j] != compare) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %d != %d\n", test_type, (datatype == ARMCI_INT ? "ARMCI_INT" : "ARMCI_LONG"), op, i, j, ((int *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "absmax", 6) == 0)
                   {
                      int compare = j + nproc - 1;
                      if(((int *) a[i])[j] != compare) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %d != %d\n", test_type, (datatype == ARMCI_INT ? "ARMCI_INT" : "ARMCI_LONG"), op, i, j, ((int *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "absmin", 6) == 0)
                   {
                      int compare = j;
                      if(((int *) a[i])[j] != compare) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %d != %d\n", test_type, (datatype == ARMCI_INT ? "ARMCI_INT" : "ARMCI_LONG"), op, i, j, ((int *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "or", 2) == 0)
                   {
                   }
             }
          break;
       case ARMCI_FLOAT:
          datatype_size = sizeof(float);
          for(i = 0; i < len_length; i++)
             a[i] = malloc(len[i] * datatype_size);
          for(i = 0; i < len_length; i++)
             for(j = 0; j < len[i]; j++)
                ((float *) a[i])[j] = (float) (me + j) * (((me + j) % 2 == 0) ? 1.0 / nproc : -1.0 / nproc);
          for(i = 0; i < len_length; i++)
          {
             if(me == 0 && verbose != 0)
                printf("testing %s ARMCI_FLOAT message size = %d op = %s\n", test_type, len[i], op);
             if(reduce_test == 0)
                armci_msg_fgop(a[i], len[i], op);
             else
                armci_msg_reduce(a[i], len[i], op, datatype);
          }
          if(me == 0 || reduce_test == 0)
             for(i = 0; i < len_length; i++)
             {
                if(me == 0 && verbose != 0)
                   printf("checking %s ARMCI_FLOAT message size = %d op = %s\n", test_type, len[i], op);
                for(j = 0; j < len[i]; j++)
                   if(strncmp(op, "+", 1) == 0)
                   {
                      float compare = 0.0;
                      if(nproc % 2 == 0)
                      {
                         if(j % 2 == 0)
                            compare = -(((int)nproc / 2) / (float) nproc);
                         else
                            compare = ((int)nproc / 2) / (float) nproc;
                      }
                      else
                      {
                         if(j % 2 == 0)
                            compare = ((int) j + nproc / 2) / (float) nproc;
                         else
                            compare = -(((int) j + nproc / 2) / (float) nproc);
                      }
                      if(ABS(((float *) a[i])[j] - compare) > ABS(compare) * FLOAT_EPS)
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_FLOAT", op, i, j, ((float *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "*", 1) == 0)
                   {
                      float compare = 1.0;
                      int k = 0;
                      for(k = 0; k < nproc; k++)
                         compare *= ((float) k + j) / (float) nproc;
                      if((nproc / 2) % 2 != 0)
                         if(nproc % 2 != 0)
                            if(j % 2 == 0)
                               compare *= -1.0;

                      if(ABS(((float *) a[i])[j] - compare) > ABS(compare) * FLOAT_EPS) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_FLOAT", op, i, j, ((float *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "min", 3) == 0)
                   {
                      float compare = -((float) j + nproc - 1) / nproc;
                      if((j + nproc - 1)% 2 == 0 && nproc > 1)
                         compare = -((float) j + nproc - 2) / nproc;
                      if(((float *) a[i])[j] != compare) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_FLOAT", op, i, j, ((float *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "max", 3) == 0)
                   {
                      float compare = ((float) j + nproc - 1) / nproc;
                      if((j + nproc - 1) % 2 != 0 && nproc > 1)
                         compare = ((float) j + nproc - 2) / nproc;
                      if(((float *) a[i])[j] != compare) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_FLOAT", op, i, j, ((float *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "absmax", 6) == 0)
                   {
                      float compare = ((float) j + nproc - 1) / nproc;
                      if(((float *) a[i])[j] != compare) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_FLOAT", op, i, j, ((float *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "absmin", 6) == 0)
                   {
                      float compare = (float) j / nproc;
                      if(((float *) a[i])[j] != compare) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_FLOAT", op, i, j, ((float *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
             }
          break;
       case ARMCI_DOUBLE:
          datatype_size = sizeof(double);
          for(i = 0; i < len_length; i++)
             a[i] = malloc(len[i] * datatype_size);
          for(i = 0; i < len_length; i++)
             for(j = 0; j < len[i]; j++)
                ((double *) a[i])[j] = (double) (me + j) * (((me + j) % 2 == 0) ? 1.0 / nproc : -1.0 / nproc);
          for(i = 0; i < len_length; i++)
          {
             if(me == 0 && verbose != 0)
                printf("testing %s ARMCI_DOUBLE message size = %d op = %s\n", test_type, len[i], op);
             if(reduce_test == 0)
                armci_msg_dgop(a[i], len[i], op);
             else
                armci_msg_reduce(a[i], len[i], op, datatype);
          }
          if(me == 0 || reduce_test == 0)
             for(i = 0; i < len_length; i++)
             {
                if(me == 0 && verbose != 0)
                   printf("checking %s ARMCI_DOUBLE message size = %d op = %s\n", test_type, len[i], op);
                for(j = 0; j < len[i]; j++)
                   if(strncmp(op, "+", 1) == 0)
                   {
                      double compare = 0.0;
                      if(nproc % 2 == 0)
                      {
                         if(j % 2 == 0)
                            compare = -(((int)nproc / 2) / (double) nproc);
                         else
                            compare = ((int)nproc / 2) / (double) nproc;
                      }
                      else
                      {
                         if(j % 2 == 0)
                            compare = ((int) j + nproc / 2) / (double) nproc;
                         else
                            compare = -(((int) j + nproc / 2) / (double) nproc);
                      }
                      if(ABS(((double *) a[i])[j] - compare) > ABS(compare) * DOUBLE_EPS) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_DOUBLE", op, i, j, ((double *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "*", 1) == 0)
                   {
                      double compare = 1.0;
                      int k = 0;
                      for(k = 0; k < nproc; k++)
                         compare *= ((float) k + j) / (float) nproc;
                      if((nproc / 2) % 2 != 0)
                         if(nproc % 2 != 0)
                            if(j % 2 == 0)
                               compare *= -1.0;
                      if(ABS(((double *) a[i])[j] - compare) > ABS(compare) * DOUBLE_EPS) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_DOUBLE", op, i, j, ((double *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "min", 3) == 0)
                   {
                      double compare = -((double) j + nproc - 1) / nproc;
                      if((j + nproc - 1)% 2 == 0 && nproc > 1)
                         compare = -((double) j + nproc - 2) / nproc;
                      if(ABS(((double *) a[i])[j] - compare) > ABS(compare) * DOUBLE_EPS) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_DOUBLE", op, i, j, ((double *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "max", 3) == 0)
                   {
                      double compare = ((double) j + nproc - 1) / nproc;
                      if((j + nproc - 1) % 2 != 0 && nproc > 1)
                         compare = ((double) j + nproc - 2) / nproc;
                      if(ABS(((double *) a[i])[j] - compare) > ABS(compare) * DOUBLE_EPS) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_DOUBLE", op, i, j, ((double *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "absmax", 6) == 0)
                   {
                      double compare = ((double) j + nproc - 1) / nproc;
                      if(ABS(((double *) a[i])[j] - compare) > ABS(compare) * DOUBLE_EPS) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_DOUBLE", op, i, j, ((double *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
                   else if(strncmp(op, "absmin", 6) == 0)
                   {
                      double compare = (double) j / nproc;
                      if(ABS(((double *) a[i])[j] - compare) > ABS(compare) * DOUBLE_EPS) 
                      {
                         printf("ERROR %s %s %s a[%d][%d] = %f != %f\n", test_type, "ARMCI_DOUBLE", op, i, j, ((double *) a[i])[j], compare);
                         ARMCI_Error("test_gop2_or_reduce failed\n",0);
                      }
                   }
             }
          break;
       default:
          break;
    }
    for(i = 0; i < len_length; i++)
       free(a[i]);
}

void test_collective(const int datatype)
{
    char * op[7] = {"+", "*", "min", "max", "absmax", "absmin", "or"};
    int i = 0;
    int num_tests = 7;
    if(datatype == ARMCI_DOUBLE || datatype == ARMCI_FLOAT)
       num_tests = 6;
    
    /* test armci_msg_brdcst */
    test_brdcst(datatype);
    
    /* test armci_msg_gop2 */
    for(i = 0; i < num_tests; i++)
       test_gop2_or_reduce(datatype, op[i], 0);

    /* test armci_msg_reduce */
    for(i = 0; i < num_tests; i++)
       test_gop2_or_reduce(datatype, op[i], 1);

    MP_BARRIER();
    ARMCI_AllFence();
    MP_BARRIER();
    
    if(me==0){printf("O.K.\n\n"); fflush(stdout);}    
}

void test_acc_type(const int datatype)
{
    int i = 0;
    int datatype_size = 0;
    void * scale;
    void * a;
    void *b[MAXPROC];
    int elems = ELEMS;
    int dim = 1;
    int count = 0;
    int strideA = 0;
    int strideB = 0;

    switch(datatype)
    {
       case ARMCI_ACC_INT:
          datatype_size = sizeof(int);
          scale = malloc(datatype_size);
          *((int *) scale) = 1;
          a = malloc(elems * datatype_size);
          create_array((void**)b, datatype_size, dim, &elems);
          for(i = 0; i < elems; i++)
          {
             ((int *) a)[i] = i + me;
             ((int *) b[me])[i] = 0;
          }
          break;
       case ARMCI_ACC_LNG:
          datatype_size = sizeof(long);
          scale = malloc(datatype_size);
          *((long *) scale) = 1;
          a = malloc(elems * datatype_size);
          create_array((void**)b, datatype_size, dim, &elems);
          for(i = 0; i < elems; i++)
          {
             ((long *) a)[i] = i + me;
             ((long *) b[me])[i] = 0;
          }
          break;
       case ARMCI_ACC_FLT:
          datatype_size = sizeof(float);
          scale = malloc(datatype_size);
          *((float *) scale) = 1.0;
          a = malloc(elems * datatype_size);
          create_array((void**)b, datatype_size, dim, &elems);
          for(i = 0; i < elems; i++)
          {
             ((float *) a)[i] = (float) i + me;
             ((float *) b[me])[i] = 0.0;
          }
          break;
       case ARMCI_ACC_DBL:
          datatype_size = sizeof(double);
          scale = malloc(datatype_size);
          *((double *) scale) = 1.0;
          a = malloc(elems * datatype_size);
          create_array((void**)b, datatype_size, dim, &elems);
          for(i = 0; i < elems; i++)
          {
             ((double *) a)[i] = (double) i + me;
             ((double *) b[me])[i] = 0.0;
          }
          break;
       case ARMCI_ACC_CPL:
          datatype_size = sizeof(cmpl_t);
          scale = malloc(datatype_size);
          ((cmpl_t *) scale)->real = 2.0;
          ((cmpl_t *) scale)->imag = 1.0;
          a = malloc(elems * datatype_size);
          create_array((void**)b, datatype_size, dim, &elems);
          for(i = 0; i < elems; i++)
          {
             ((cmpl_t *) a)[i].real = ((float) i + me);
             ((cmpl_t *) a)[i].imag = ((float) i + me);
             ((cmpl_t *) b[me])[i].real = 0.0;
             ((cmpl_t *) b[me])[i].imag = 0.0;
          }
          break;
       case ARMCI_ACC_DCP:
          datatype_size = sizeof(dcmpl_t);
          scale = malloc(datatype_size);
          ((dcmpl_t *) scale)->real = 2.0;
          ((dcmpl_t *) scale)->imag = 1.0;
          a = malloc(elems * datatype_size);
          create_array((void**)b, datatype_size, dim, &elems);
          for(i = 0; i < elems; i++)
          {
             ((dcmpl_t *) a)[i].real = ((double) i + me);
             ((dcmpl_t *) a)[i].imag = ((double) i + me);
             ((dcmpl_t *) b[me])[i].real = 0.0;
             ((dcmpl_t *) b[me])[i].imag = 0.0;
          }
          break;
       default:
          return;
          break;
    }

    count = elems * datatype_size;
    strideA = elems * datatype_size;
    strideB = elems * datatype_size;

    ARMCI_AllFence();
    MP_BARRIER();

    for(i = 0; i < nproc; i++)
       ARMCI_AccS(datatype, scale, a, &strideA, b[(me + i) % nproc], &strideB, &count, 0, (me + i) % nproc);

    ARMCI_AllFence();
    MP_BARRIER();

    switch(datatype)
    {
       case ARMCI_ACC_INT:
          for(i = 0; i < elems; i++)
          {
             int compare = (i * nproc) + nproc / 2 * (nproc - 1);
             if(((int *)b[me])[i] != compare) 
             {
                printf("ERROR accumulate ARMCI_ACC_INT [%d] = %d != %d\n", i, ((int *)b[me])[i], compare);
                ARMCI_Error("test_acc_type failed\n",0);
             }
          }
          break;
       case ARMCI_ACC_LNG:
          for(i = 0; i < elems; i++)
          {
             long compare = (i * nproc) + nproc / 2 * (nproc - 1);
             if(((long *)b[me])[i] != compare) 
             {
                printf("ERROR accumulate ARMCI_ACC_LNG [%d] = %d != %d\n", i, ((int *)b[me])[i], compare);
                ARMCI_Error("test_acc_type failed\n",0);
             }
          }
          break;
       case ARMCI_ACC_FLT:
          for(i = 0; i < elems; i++)
          {
             float compare = (float) ((i * nproc) + nproc / 2 * (nproc - 1));
             if(((float *)b[me])[i] != compare) 
             {
                printf("ERROR accumulate ARMCI_ACC_FLT [%d] = %f != %f\n", i, ((float *)b[me])[i], compare);
                ARMCI_Error("test_acc_type failed\n",0);
             }
          }
          break;
       case ARMCI_ACC_DBL:
          for(i = 0; i < elems; i++)
          {
             double compare = (double) ((i * nproc) + nproc / 2 * (nproc - 1));
             if(((double *)b[me])[i] != (double) ((i * nproc) + nproc / 2 * (nproc - 1))) 
             {
                printf("ERROR accumulate ARMCI_ACC_DBL [%d] = %f != %f \n", i, ((double *)b[me])[i], compare);
                ARMCI_Error("test_acc_type failed\n",0);
             }
          }
          break;
       case ARMCI_ACC_CPL:
          for(i = 0; i < elems; i++)
          {
             float compare = (float) ((i * nproc) + nproc / 2 * (nproc - 1));
             if(((cmpl_t *)b[me])[i].real != compare && ((cmpl_t *)b[me])[i].imag != 3 * compare) 
             {
                printf("ERROR accumulate ARMCI_ACC_CPL [%d] = %f + %fj != %f + %fj\n", i, ((cmpl_t *)b[me])[i].real, ((cmpl_t *)b[me])[i].imag, compare, 3 * compare);
                ARMCI_Error("test_acc_type failed\n",0);
             }
          }
          break;
       case ARMCI_ACC_DCP:
          for(i = 0; i < elems; i++)
          {
             double compare = (double) ((i * nproc) + nproc / 2 * (nproc - 1));
             if(((dcmpl_t *)b[me])[i].real != compare && ((dcmpl_t *)b[me])[i].imag != 3 * compare) 
             {
                printf("ERROR accumulate ARMCI_ACC_DCP [%d] = %f + %fj != %f + %fj\n", i, ((dcmpl_t *)b[me])[i].real, ((dcmpl_t *)b[me])[i].imag, compare, 3 * compare);
                ARMCI_Error("test_acc_type failed\n",0);
             }
          }
          break;
       default:
          break;
    }

    MP_BARRIER();
    ARMCI_AllFence();
    MP_BARRIER();
    
    if(me==0){printf("O.K.\n\n"); fflush(stdout);}    
    destroy_array((void**)b);
    free(a);
    free(scale);
}

/* we need to rename main if linking with frt compiler */
#ifdef FUJITSU_FRT
#define main MAIN__
#endif

int main(int argc, char* argv[])
{
    int i;
    struct timeval start_time[14];
    struct timeval stop_time[14];
    /*
      char * test_name[14] = {
      "dim", "nbdim", "vec_small", "acc",
      "vector", "vector_acc", "fetch_add",
      "swap", "rput", "aggregate", "implicit",
      "memlock", "acc_type", "collective"
      };
      int test_flags[14] = {
      1, 1, 1, 1,
      1, 1, 1,
      1, 1, 0, 1,
      1, 1, 1
      };
    */
    char * test_name[2] = { "acc_type", "collective" };
    int test_flags[2]   = { 1, 1 };

#define TEST_ACC_TYPE   0
#define TEST_COLLECTIVE 1

    MP_INIT(argc, argv);
    ARMCI_Init();
    MP_PROCS(&nproc);
    MP_MYID(&me);

    if(nproc > MAXPROC && me == 0)
       ARMCI_Error("Test works for up to %d processors\n",MAXPROC);

    if(me == 0)
    {
       printf("ARMCI test program (%d processes)\n",nproc); 
       fflush(stdout);
       sleep(1);
    }    

    gettimeofday(&start_time[TEST_ACC_TYPE],NULL);
    if(test_flags[TEST_ACC_TYPE] == 1)
    {
       if(me == 0)
       {
          printf("\nTesting Accumulate Types\n");
          fflush(stdout);
       }
       
       MP_BARRIER();
       if(me == 0)
       {
          printf("Test Accumulate ARMCI_ACC_INT\n");
          fflush(stdout);
       }
       test_acc_type(ARMCI_ACC_INT);
       ARMCI_AllFence();
       MP_BARRIER();
       if(me == 0)
       {
          printf("Test Accumulate ARMCI_ACC_LNG\n");
          fflush(stdout);
       }
       test_acc_type(ARMCI_ACC_LNG);
       ARMCI_AllFence();
       MP_BARRIER();
       if(me == 0)
       {
          printf("Test Accumulate ARMCI_ACC_FLT\n");
          fflush(stdout);
       }
       test_acc_type(ARMCI_ACC_FLT);
       ARMCI_AllFence();
       MP_BARRIER();
       if(me == 0)
       {
          printf("Test Accumulate ARMCI_ACC_DBL\n");
          fflush(stdout);
       }
       test_acc_type(ARMCI_ACC_DBL);
       ARMCI_AllFence();
       MP_BARRIER();
       if(me == 0)
       {
          printf("Test Accumulate ARMCI_ACC_CPL\n");
          fflush(stdout);
       }
       test_acc_type(ARMCI_ACC_CPL);
       ARMCI_AllFence();
       MP_BARRIER();
       if(me == 0)
       {
          printf("Test Accumulate ARMCI_ACC_DCP\n");
          fflush(stdout);
       }
       test_acc_type(ARMCI_ACC_DCP);
       ARMCI_AllFence();
       MP_BARRIER();
    }
    gettimeofday(&stop_time[TEST_ACC_TYPE],NULL);

    gettimeofday(&start_time[TEST_COLLECTIVE],NULL);
    if(test_flags[TEST_COLLECTIVE] == 1)
    {
       if(me == 0)
       {
          printf("\nTesting Collective Types\n");
          fflush(stdout);
       }
       if(me == 0)
       {
          printf("Test Collective ARMCI_INT\n");
          fflush(stdout);
       }
       MP_BARRIER();
       test_collective(ARMCI_INT);
       MP_BARRIER();
       if(me == 0)
       {
          printf("Test Collective ARMCI_LONG\n");
          fflush(stdout);
       }
       MP_BARRIER();
       test_collective(ARMCI_LONG);
       MP_BARRIER();
       if(me == 0)
       {
          printf("Test Collective ARMCI_FLOAT\n");
          fflush(stdout);
       }
       MP_BARRIER();
       test_collective(ARMCI_FLOAT);
       MP_BARRIER();
       if(me == 0)
       {
          printf("Test Collective ARMCI_DOUBLE\n");
          fflush(stdout);
       }
       MP_BARRIER();
       test_collective(ARMCI_DOUBLE);
       MP_BARRIER();
    }
    gettimeofday(&stop_time[TEST_COLLECTIVE],NULL);
    
    if(me == 0)
    {
       printf("Accumulate and Collective tests passed\n");
       fflush(stdout);
    }

    if(me == 0)
    {
       printf("Testcase runtime\n");
       printf("Name,Time(seconds)\n");
       for(i = 0; i < 2; i++)
          if(test_flags[i] == 1)
          {
             double time_spent = (stop_time[i].tv_sec - start_time[i].tv_sec) + ((double) stop_time[i].tv_usec - start_time[i].tv_usec) / 1E6;
             printf("%s,%.6f\n", test_name[i], time_spent);
          }
    }

    MP_BARRIER();
    ARMCI_Finalize();
    MP_FINALIZE();
    return(0);
}
