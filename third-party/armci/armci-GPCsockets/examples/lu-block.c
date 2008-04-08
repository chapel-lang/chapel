/**************************************************
 *             LU factorization                   *
 *             Armci Version                      *
 *             Block distribution                 *
 **************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h> 
#include "armci.h"

#define DEBUG_
#define MAXRAND                         32767.0
#define DEFAULT_N                        1500
#define DEFAULT_B                          16

/* ARMCI is message-passing ambivalent -  we define macros for common MP calls*/
#ifdef PVM
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
#elif defined(TCG)
#   include <sndrcv.h>
    long tcg_tag =30000;
#   define MP_BARRIER()      SYNCH_(&tcg_tag)
#   define MP_INIT(arc,argv) PBEGIN_((argc),(argv))
#   define MP_FINALIZE()     PEND_()
#   define MP_MYID(pid)      *(pid)   = (int)NODEID_()
#   define MP_PROCS(pproc)   *(pproc) = (int)NNODES_()
#else
#   include <mpi.h>
#   define MP_BARRIER()      MPI_Barrier(MPI_COMM_WORLD)
#   define MP_FINALIZE()     MPI_Finalize()
#   define MP_INIT(arc,argv) MPI_Init(&(argc),&(argv))
#   define MP_MYID(pid)      MPI_Comm_rank(MPI_COMM_WORLD, (pid))
#   define MP_PROCS(pproc)   MPI_Comm_size(MPI_COMM_WORLD, (pproc));
#endif

/* global variables */
int n = DEFAULT_N;         /* The size of the matrix */
int block_size = DEFAULT_B;/* Block dimension */
int nblocks;               /* Number of blocks in each dimension */
int num_rows;              /* Number of processors per row of processor grid */
int num_cols;              /* Number of processors per col of processor grid */
double **a;                /* a = lu; l and u both placed back in a */
int nproc, me = 0;
int proc_bytes;
int num;

int doprint = 0;

/* function declaration */
void lu(int, int, int);
void lu0(double *,int, int);
void bdiv(double *, double *, int, int, int, int);
void bmodd(double *, double*, int, int, int, int);
void bmod(double *, double *, double *, int, int, int, int, int, int);
void daxpy(double *, double *, int, double);
int block_owner(int, int);
void init_array();
double touch_array(int, int);
void print_block();
void print_array(int);
void get_remote(double *, int, int);

/* timing functions */
extern void start_timer(void);
extern double elapsed_time(void);
extern double stop_time(void);

main(int argc, char *argv[])
{
    int i, j;
    int ch;
    extern char *optarg;
    int edge;
    int size;
    
    /* ARMCI */
    void **ptr;
    double **ptr_loc;
    
    MP_INIT(arc,argv);
    MP_PROCS(&nproc);
    MP_MYID(&me);
    
    while ((ch = getopt(argc, argv, "n:b:p:h")) != -1) {
        switch(ch) {
            case 'n': n = atoi(optarg); break;
            case 'b': block_size = atoi(optarg); break;
            case 'p': nproc = atoi(optarg); break;
            case 'h': {
                printf("Usage: LU, or \n");
		printf("       LU -nMATRIXSIZE -bBLOCKSIZE -pNPROC\n");
                MP_BARRIER();
                MP_FINALIZE();
                exit(0);
            }            
        }
    }
    
    if(me == 0) {
        printf("\n Blocked Dense LU Factorization\n");
        printf("     %d by %d Matrix\n", n, n);
        printf("     %d Processors\n", nproc);
        printf("     %d by %d Element Blocks\n", block_size, block_size);
        printf("\n");
    }
    
    num_rows = (int) sqrt((double) nproc);
    for (;;) {
        num_cols = nproc/num_rows;
        if (num_rows*num_cols == nproc)
            break;
        num_rows--;
    }
    
    nblocks = n/block_size;
    if (block_size * nblocks != n) {
        nblocks++;
    }
    
    num = (nblocks * nblocks)/nproc;
    if((num * nproc) != (nblocks * nblocks))
        num++;

    edge = n%block_size;
    if (edge == 0) {
        edge = block_size;
    }
#ifdef DEBUG
    if(me == 0)
        for (i=0;i<nblocks;i++) {
            for (j=0;j<nblocks;j++) 
                printf("%d ", block_owner(i, j));
            printf("\n");
        }
    MP_BARRIER();
    MP_FINALIZE();
    exit(0);
#endif
    
    for (i=0;i<nblocks;i++) {
        for (j=0;j<nblocks;j++) {
            if(block_owner(i,j) == me) {
                if ((i == nblocks-1) && (j == nblocks-1)) {
                    size = edge*edge;
                }
                else if ((i == nblocks-1) || (j == nblocks-1)) {
                    size = edge*block_size;
                }
                else {
                    size = block_size*block_size;
                }
                proc_bytes += size*sizeof(double);
            }
        }
    }
    
    /* initialize ARMCI */
    ARMCI_Init();
    ptr = (void **)malloc(nproc * sizeof(void *));
    ARMCI_Malloc(ptr, proc_bytes);
    
    a = (double **)malloc(nblocks*nblocks*sizeof(double *));
    if (a == NULL) {
        fprintf(stderr, "Could not malloc memory for a\n");
        exit(-1);
    } 
    ptr_loc = (double **)malloc(nproc*sizeof(double *));
    for(i=0; i<nproc; i++) ptr_loc[i] = (double *)ptr[i];
    for(i=0; i<nblocks;i ++) {
        for(j=0; j<nblocks; j++) {
            a[i+j*nblocks] = ptr_loc[block_owner(i, j)];
            if ((i == nblocks-1) && (j == nblocks-1)) {
                size = edge*edge;
            } else if ((i == nblocks-1) || (j == nblocks-1)) {
                size = edge*block_size;
            } else {
                size = block_size*block_size;
            }
            ptr_loc[block_owner(i, j)] += size;
        }
    }
    
    /* initialize the array */
    init_array();
    
    /* barrier to ensure all initialization is done */
    MP_BARRIER();

    /* to remove cold-start misses, all processors touch their own data */
    touch_array(block_size, me);
    MP_BARRIER();

    if(doprint) {
        if(me == 0) {
            printf("Matrix before LU decomposition\n");
            print_array(me); 
        }
        MP_BARRIER();
    }
    

    /* Starting the timer */
    if(me == 0) start_timer();

    lu(n, block_size, me);
    
    MP_BARRIER();

    /* Timer Stops here */
    if(me == 0) 
        printf("\nRunning time = %lf milliseconds.\n\n",  elapsed_time());

    if(doprint) {        
        if(me == 0) {
            printf("after LU\n");
            print_array(me);
        }
        MP_BARRIER();
    }
    
    /* done */
    ARMCI_Free(ptr[me]);
    ARMCI_Finalize();
    MP_FINALIZE();
}

void lu(int n, int bs, int me)
{
    int i, il, j, jl, k, kl;
    int I, J, K;
    double *A, *B, *C, *D;
    int dimI, dimJ, dimK;
    int strI, strJ, strK;
    unsigned int t1, t2, t3, t4, t11, t22;
    int diagowner;
    double *buf1, *buf2;

    /* temporary memories */
    buf1 = (double *)malloc(block_size*block_size*sizeof(double));
    buf2 = (double *)malloc(block_size*block_size*sizeof(double));
    
    for (k=0, K=0; k<n; k+=bs, K++) {
        kl = k + bs; 
        if (kl > n) {
            kl = n;
            strK = kl - k;
        } else {
            strK = bs;
        }
    
        /* factor diagonal block */
        diagowner = block_owner(K, K);
        if (diagowner == me) {
            A = a[K+K*nblocks]; 
            lu0(A, strK, strK);
        }
        MP_BARRIER();
      
        /* divide column k by diagonal block */
        if(block_owner(K, K) == me)
            D = a[K+K*nblocks];
        else {
            D = buf1;
            get_remote(D, K, K);
        }
        for (i=kl, I=K+1; i<n; i+=bs, I++) {
            if (block_owner(I, K) == me) {  /* parcel out blocks */
                il = i + bs; 
                if (il > n) {
                    il = n;
                    strI = il - i;
                } else {
                    strI = bs;
                }
                A = a[I+K*nblocks]; 
                bdiv(A, D, strI, strK, strI, strK);
            }
        }

        /* modify row k by diagonal block */
        for (j=kl, J=K+1; j<n; j+=bs, J++) {
            if (block_owner(K, J) == me) {  /* parcel out blocks */
                jl = j+bs; 
                if (jl > n) {
                    jl = n;
                    strJ = jl - j;
                } else {
                    strJ = bs;
                }
                A = a[K+J*nblocks];
                bmodd(D, A, strK, strJ, strK, strK);
            }
        }
        
        MP_BARRIER();

        /* modify subsequent block columns */
        for (i=kl, I=K+1; i<n; i+=bs, I++) {
            il = i+bs; 
            if (il > n) {
                il = n;
                strI = il - i;
            } else {
                strI = bs;
            }

            if(block_owner(I,K) == me)
                A = a[I+K*nblocks];
            else {
                A = buf1;
                get_remote(A, I, K);
            }
            for (j=kl, J=K+1; j<n; j+=bs, J++) {
                jl = j + bs; 
                if (jl > n) {
                    jl = n;
                    strJ= jl - j;
                } else {
                    strJ = bs;
                }
                if (block_owner(I, J) == me) {  /* parcel out blocks */
                    if(block_owner(K,J) == me)
                        B = a[K+J*nblocks];
                    else {
                        B = buf2;
                        get_remote(B, K, J);
                    }
                    C = a[I+J*nblocks];
                    bmod(A, B, C, strI, strJ, strK, strI, strK, strI);
                }
            }
        }
    }

    free(buf1);
    free(buf2);
}

void get_remote(double *buf, int I, int J)
{
    int proc_owner;
    int edge, size;
    
    proc_owner = block_owner(I, J);
    
    edge = n%block_size;
    if (edge == 0) {
        edge = block_size;
    }

    if ((I == nblocks-1) && (J == nblocks-1)) {
        size = edge*edge;
    }
    else if ((I == nblocks-1) || (J == nblocks-1)) {
        size = edge*block_size;
    }
    else {
        size = block_size*block_size;
    }
    size = size * sizeof(double);
    
    ARMCI_Get(a[I+J*nblocks], buf, size, proc_owner);
}

void lu0(double *a, int n, int stride)
{
    int j; 
    int k; 
    int length;
    double alpha;
    
    for (k=0; k<n; k++) {
        /* modify subsequent columns */
        for (j=k+1; j<n; j++) {
            a[k+j*stride] /= a[k+k*stride];
            alpha = -a[k+j*stride];
            length = n-k-1;
            daxpy(&a[k+1+j*stride], &a[k+1+k*stride], n-k-1, alpha);
        }
    }
}

void bdiv(double *a, double *diag, int stride_a, int stride_diag,
          int dimi, int dimk)
{
    int j; 
    int k;
    double alpha;
    
    for (k=0; k<dimk; k++) {
        for (j=k+1; j<dimk; j++) {
            alpha = -diag[k+j*stride_diag];
            daxpy(&a[j*stride_a], &a[k*stride_a], dimi, alpha);
        }
    }
}

void bmodd(double *a, double *c, int dimi, int dimj,
           int stride_a, int stride_c)
{
    int i; 
    int j; 
    int k; 
    int length;
    double alpha;
    
    for (k=0; k<dimi; k++) {
        for (j=0; j<dimj; j++) {
            c[k+j*stride_c] /= a[k+k*stride_a];
            alpha = -c[k+j*stride_c];
            length = dimi - k - 1;
            daxpy(&c[k+1+j*stride_c], &a[k+1+k*stride_a], dimi-k-1, alpha);
        }
    }
}

void bmod(double *a, double *b, double *c, int dimi, int dimj, int dimk,
          int stridea, int strideb, int stridec)
{
    int i; 
    int j; 
    int k;
    double alpha;
    
    for (k=0; k<dimk; k++) {
        for (j=0; j<dimj; j++) {
            alpha = -b[k+j*strideb]; 
            daxpy(&c[j*stridec], &a[k*stridea], dimi, alpha);
        }
    }
}

void daxpy(double *a, double *b, int n, double alpha)
{
    int i;
    
    for (i=0; i<n; i++) {
        a[i] += alpha*b[i];
    }
}

int block_owner(int I, int J)
{
/*    return((J%num_cols) + (I%num_rows)*num_cols); */
    return((I+J*nblocks)/num);
}

void init_array()
{
    int i, j;
    int ii, jj;
    int edge;
    int ibs;
    int jbs, skip;
    
    srand48((long) 1);
    edge = n%block_size;
    for (j=0; j<n; j++) {
        for (i=0; i<n; i++) {
            if(block_owner((i/block_size), (j/block_size)) == me) {
                if ((n - i) <= edge) {
                    ibs = edge;
                    ibs = n-edge;
                    skip = edge;
                } else {
                    ibs = block_size;
                    skip = block_size;
                }
                if ((n - j) <= edge) {
                    jbs = edge;
                    jbs = n-edge;
                } else {
                    jbs = block_size;
                }
                ii = (i/block_size) + (j/block_size)*nblocks;
                jj = (i%ibs)+(j%jbs)*skip;
/*            a[ii][jj] = ((double) lrand48())/MAXRAND; */
                a[ii][jj] = i + j*6 + 1;
                if (i == j) {
                    a[ii][jj] *= 10;
                }
            }
        }
    }
}

double touch_array(int bs, int me)
{
    int i, j, I, J;
    double tot = 0.0;
    int ibs;
    int jbs;
    
    /* touch my portion of A[] */
    
    for (J=0; J<nblocks; J++) {
        for (I=0; I<nblocks; I++) {
            if (block_owner(I, J) == me) {
                if (J == nblocks-1) {
                    jbs = n%bs;
                    if (jbs == 0) {
                        jbs = bs;
                    }
                } else {
                    jbs = bs;
                }
                if (I == nblocks-1) {
                    ibs = n%bs;
                    if (ibs == 0) {
                        ibs = bs;
                    }
                } else {
                    ibs = bs;
                }
                for (j=0; j<jbs; j++) {
                    for (i=0; i<ibs; i++) {
                        tot += a[I+J*nblocks][i+j*ibs];
                    }
                }
            }
        }
    } 
    return(tot);
}

void print_array(int myid)
{
    int i, j, k;
    double **buf;

    int ii, jj;
    int edge;
    int ibs, jbs, skip;

    buf = (double **)malloc(nblocks*nblocks*sizeof(double *));
    for(i=0; i<nblocks; i++) 
        for(j=0; j<nblocks; j++) 
            if(block_owner(i, j) == myid)
                buf[i+j*nblocks] = a[i+j*nblocks];
            else {
                buf[i+j*nblocks] = (double *)malloc(block_size*block_size*
                                                   sizeof(double));
                get_remote(buf[i+j*nblocks], i, j);
            }

    /* copied from lu.C */
    edge = n%block_size;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            if ((n - i) <= edge) {
                ibs = edge;
                ibs = n-edge;
                skip = edge;
            } else {
                ibs = block_size;
                skip = block_size;
            }
            if ((n - j) <= edge) {
                jbs = edge;
                jbs = n-edge;
            } else {
                jbs = block_size;
            }
            ii = (i/block_size) + (j/block_size)*nblocks;
            jj = (i%ibs)+(j%jbs)*skip;
            printf("%8.1f ", buf[ii][jj]);   
        }
        printf("\n");
    }
    fflush(stdout);      
    
    for(i=0; i<nblocks; i++) 
        for(j=0; j<nblocks; j++) 
            if(block_owner(i, j) != myid) free(buf[i+j*nblocks]);
    free(buf);
}

void print_block()
{
    int i, j, k;

    for(i=0; i<nblocks; i++)
        for(j=0; j<nblocks; j++)
            if(block_owner(i,j) == me) {
                printf("Block %d (%d,%d)\t", i+j*nblocks, i, j);
                for(k=0; k<block_size*block_size; k++)
                    printf("%8.1f ", a[i+j*nblocks][k]);
                printf("\t me = %d\n", me);
            }
}
