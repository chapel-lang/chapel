/*              
 * Copyright (c)  1999 Pacific Northwest National Laboratory
 * All rights reserved.
 *
 *	Author: Jialin  Ju Account, PNNL
 */

/***
   NAME
     lu-thread.c
   PURPOSE
     Thread version of lu factorization     
   NOTES
     
   HISTORY
     jju - Apr 19, 1999: Created.
***/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include "barrier.h"
#include "errors.h"

#define MAXRAND                         32767.0
#define DEFAULT_N                         512
#define DEFAULT_P                           1
#define DEFAULT_B                          16
#define min(a,b) ((a) < (b) ? (a) : (b))

#define MAXTHR 16

barrier_t barrier;
pthread_t thread[MAXTHR];
int global_id;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_attr_t attr;
int num;

/* timing functions */
extern void start_timer(void);
extern double elapsed_time(void);
extern double stop_time(void);

int n = DEFAULT_N;          /* The size of the matrix */
int P = DEFAULT_P;          /* Number of processors */
int block_size = DEFAULT_B; /* Block dimension */
int nblocks;                /* Number of blocks in each dimension */
int num_rows;               /* Number of processors per row of processor grid */
int num_cols;               /* Number of processors per col of processor grid */
double **a;                 /* a = lu; l and u both placed back in a */
double *rhs;
int *proc_bytes;            /* Bytes to malloc per processor to hold blocks 
			       of A*/
double **last_malloc;       /* Starting point of last block of A */

int test_result = 0;        /* Test result of factorization? */
int doprint = 0;            /* Print out matrix values? */
int dostats = 0;            /* Print out individual processor statistics? */

void *SlaveStart();
void OneSolve(int, int, double **, int, int);
void lu0(double *,int, int);
void bdiv(double *, double *, int, int, int, int);
void bmodd(double *, double*, int, int, int, int);
void bmod(double *, double *, double *, int, int, int, int, int, int);
void daxpy(double *, double *, int, double);
int BlockOwner(int, int);
void lu(int, int, int, int);
void InitA(double *);
double TouchA(int, int);
void PrintA();
void CheckResult(int, double **, double *);
void printerr(char *);


main(argc, argv)

int argc;
char *argv[];

{
    int i, j;
    int ch;
    extern char *optarg;
    int MyNum=0;
    double mint, maxt, avgt;
    double min_fac, min_solve, min_mod, min_bar;
    double max_fac, max_solve, max_mod, max_bar;
    double avg_fac, avg_solve, avg_mod, avg_bar;
    int last_page;
    int proc_num;
    int edge;
    int size;
    unsigned int start;
    int status;
    
    while ((ch = getopt(argc, argv, "n:p:b:toh")) != -1) {
        switch(ch) {
            case 'n': n = atoi(optarg); break;
            case 'p': P = atoi(optarg); break;
            case 'b': block_size = atoi(optarg); break;
            case 't': test_result = !test_result; break;
            case 'o': doprint = !doprint; break;
            case 'h': printf("Usage: LU <options>\n\n");
                printf("options:\n");
                printf("  -nN : Decompose NxN matrix.\n");
                printf("  -pP : P = number of processors.\n");
                printf("  -bB : Use a block size of B.\n");
                printf("  -t  : Test output.\n");
                printf("  -o  : Print out matrix values.\n");
                printf("  -h  : Print out command line options.\n\n");
                printf("Default: LU -n%1d -p%1d -b%1d\n",
                       DEFAULT_N,DEFAULT_P,DEFAULT_B);
                exit(0);
                break;
        }
    }
    
    printf("\n");
    printf("Blocked Dense LU Factorization\n");
    printf("     %d by %d Matrix\n",n,n);
    printf("     %d Processors\n",P);
    printf("     %d by %d Element Blocks\n",block_size,block_size);
    printf("\n");
    printf("\n");
    
    num_rows = (int) sqrt((double) P);
    for (;;) {
        num_cols = P/num_rows;
        if (num_rows*num_cols == P)
            break;
        num_rows--;
    }
    nblocks = n/block_size;
    if (block_size * nblocks != n) {
        nblocks++;
    }

    num = (nblocks * nblocks)/P;
    if((num * P) != (nblocks * nblocks))
        num++;

    edge = n%block_size;
    if (edge == 0) {
        edge = block_size;
    }
    proc_bytes = (int *) malloc(P*sizeof(int));
    last_malloc = (double **) malloc(P*sizeof(double *));
    for (i=0;i<P;i++) {
        proc_bytes[i] = 0;
        last_malloc[i] = NULL;
    }
    for (i=0;i<nblocks;i++) {
        for (j=0;j<nblocks;j++) {
            proc_num = BlockOwner(i,j);
            if ((i == nblocks-1) && (j == nblocks-1)) {
                size = edge*edge;
            } else if ((i == nblocks-1) || (j == nblocks-1)) {
                size = edge*block_size;
            } else {
                size = block_size*block_size;
            }
            proc_bytes[proc_num] += size*sizeof(double);
        }
    }
    for (i=0;i<P;i++) {
        /* last_malloc[i] = (double *) malloc(proc_bytes[i] + PAGE_SIZE); */
        last_malloc[i] = (double *) malloc(proc_bytes[i]);
        
        if (last_malloc[i] == NULL) {
            fprintf(stderr,"Could not malloc memory blocks for proc %d\n",i);
            exit(-1);
        } 
/*        last_malloc[i] = (double *)(((unsigned) last_malloc[i]) + PAGE_SIZE -
                                     ((unsigned) last_malloc[i]) % PAGE_SIZE);
*/     
    /* Note that this causes all blocks to start out page-aligned, and that
       for block sizes that exceed cache line size, blocks start at cache-line
       aligned addresses as well.  This reduces false sharing */
        
    }
    a = (double **) malloc(nblocks*nblocks*sizeof(double *));
    if (a == NULL) {
        printerr("Could not malloc memory for a\n");
        exit(-1);
    } 
    for (i=0;i<nblocks;i++) {
        for (j=0;j<nblocks;j++) {
            proc_num = BlockOwner(i,j);
            a[i+j*nblocks] = last_malloc[proc_num];
            if ((i == nblocks-1) && (j == nblocks-1)) {
                size = edge*edge;
            } else if ((i == nblocks-1) || (j == nblocks-1)) {
                size = edge*block_size;
            } else {
                size = block_size*block_size;
            }
            last_malloc[proc_num] += size;
        }
    }
    
    rhs = (double *) malloc(n*sizeof(double));
    if (rhs == NULL) {
        printerr("Could not malloc memory for rhs\n");
        exit(-1);
    } 
    
    barrier_init(&barrier, P);
    
    /* LOCKINIT(Global->idlock); */
    global_id = 0;
    
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    
    for (i=1; i<P; i++) {
        /* CREATE(SlaveStart);*/
        status = pthread_create(&thread[i], &attr,SlaveStart, NULL);
        if (status != 0) err_abort (status, "Create thread");
    }

    pthread_attr_destroy(&attr);
    
    InitA(rhs);
    if (doprint) {
        printf("Matrix before decomposition:\n");
        PrintA();
    }

    pthread_setconcurrency(P);
    
    printf("the concurrency level is %d \n", pthread_getconcurrency());
    pthread_setscope_np(4);
    
    SlaveStart(MyNum);

    /* WAIT_FOR_END(P-1) */
    for (i=1; i<P; i++) {
        status = pthread_join(thread[i], NULL);
        if (status != 0) err_abort (status, "Join thread");
    }
    
    if (doprint) {
        printf("\nMatrix after decomposition:\n");
        PrintA();
    }
    
    if (test_result) {
        printf("                             TESTING RESULTS\n");
        CheckResult(n, a, rhs);
    }
    
    barrier_destroy (&barrier);
}


void *SlaveStart()

{
    int i; 
    int j; 
    int cluster; 
    int max_block;
    int MyNum;
    int status;
    
    /* LOCK(Global->idlock) */
    status = pthread_mutex_lock(&mutex);
    if(status != 0) err_abort(status, "Lock mutex");
    
    MyNum = global_id;
    global_id ++;
    /* UNLOCK(Global->idlock) */
    status = pthread_mutex_unlock(&mutex);
    if(status != 0) err_abort(status, "Unlock mutex");

/*    printf("binging..\n");
    bindprocessor(BINDTHREAD, pthread_self(), MyNum);
    printf("binging in %d \n", sysconf(_SC_NPROCESSORS_CONF)); */
/* POSSIBLE ENHANCEMENT:  Here is where one might pin processes to
   processors to avoid migration */
    
    OneSolve(n, block_size, a, MyNum, dostats);
}


void OneSolve(n, block_size, a, MyNum, dostats)

double **a;
int n;
int block_size;
int MyNum;
int dostats;

{
    unsigned int i; 
    unsigned int myrs; 
    unsigned int myrf; 
    unsigned int mydone;
    int status;
    
    /* barrier to ensure all initialization is done */
    /* BARRIER(Global->start, P); */
    status = barrier_wait (&barrier);
    if (status > 0) err_abort (status, "Wait on barrier");
    
    
    /* to remove cold-start misses, all processors touch their own data */
    TouchA(block_size, MyNum);
    
    /* BARRIER(Global->start, P);*/
    status = barrier_wait (&barrier);
    if (status > 0) err_abort (status, "Wait on barrier");

    /* Starting the timer */
    if(MyNum == 0) start_timer();

    lu(n, block_size, MyNum, dostats);
    
    /* BARRIER(Global->start, P); */
    status = barrier_wait (&barrier);
    if (status > 0) err_abort (status, "Wait on barrier");

    /* Timer Stops here */
    if(MyNum == 0)
        printf("\nRunning time = %lf milliseconds.\n\n",  elapsed_time());
}


void lu0(a, n, stride)

double *a;
int n; 
int stride;

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


void bdiv(a, diag, stride_a, stride_diag, dimi, dimk)

double *a; 
double *diag;
int stride_a; 
int stride_diag; 
int dimi; 
int dimk;

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


void bmodd(a, c, dimi, dimj, stride_a, stride_c)

double *a; 
double *c;
int dimi; 
int dimj; 
int stride_a; 
int stride_c;

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


void bmod(a, b, c, dimi, dimj, dimk, stridea, strideb, stridec)

double *a; 
double *b; 
double *c;
int dimi; 
int dimj; 
int dimk; 
int stridea;
int strideb;
int stridec;

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


void daxpy(a, b, n, alpha)

double *a; 
double *b; 
double alpha;
int n;

{
    int i;
    
    for (i=0; i<n; i++) {
        a[i] += alpha*b[i];
    }
}


int BlockOwner(I, J)

int I; 
int J;

{
/*    return((J%num_cols) + (I%num_rows)*num_cols); */
     return((I+J*nblocks)/num); 
}


void lu(n, bs, MyNum, dostats)

int n;
int bs;
int MyNum;
int dostats;

{
    int i, il, j, jl, k, kl;
    int I, J, K;
    double *A, *B, *C, *D;
    int dimI, dimJ, dimK;
    int strI, strJ, strK;
    unsigned int t1, t2, t3, t4, t11, t22;
    int diagowner;
    int colowner;
    int status;
    
    for (k=0, K=0; k<n; k+=bs, K++) {
        kl = k + bs; 
        if (kl > n) {
            kl = n;
            strK = kl - k;
        } else {
            strK = bs;
        }
#ifdef DEBUG
        printf("k = %d, before factorization\n", k);
        PrintA();
#endif
        
        /* factor diagonal block */
        diagowner = BlockOwner(K, K);
        if (diagowner == MyNum) {
            A = a[K+K*nblocks]; 
            lu0(A, strK, strK);
        }
#ifdef DEBUG
        printf("k = %d, after factorization\n", k);    
        PrintA();
#endif
        
        /* BARRIER(Global->start, P); */
        status = barrier_wait (&barrier);
        if (status > 0) err_abort (status, "Wait on barrier");
        
        /* divide column k by diagonal block */
        D = a[K+K*nblocks];
        for (i=kl, I=K+1; i<n; i+=bs, I++) {
            if (BlockOwner(I, K) == MyNum) {  /* parcel out blocks */
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
#ifdef DEBUG
        printf("k = %d, after divide column k by diagonal block\n", k);    
        PrintA();
#endif
        
        /* modify row k by diagonal block */
        for (j=kl, J=K+1; j<n; j+=bs, J++) {
            if (BlockOwner(K, J) == MyNum) {  /* parcel out blocks */
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
#ifdef DEBUG
        printf("k = %d, after modify row k by diagonal block\n", k);    
        PrintA();
#endif
        
        /* BARRIER(Global->start, P); */
        status = barrier_wait (&barrier);
        if (status > 0) err_abort (status, "Wait on barrier");
        
        /* modify subsequent block columns */
        for (i=kl, I=K+1; i<n; i+=bs, I++) {
            il = i+bs; 
            if (il > n) {
                il = n;
                strI = il - i;
            } else {
                strI = bs;
            }
            colowner = BlockOwner(I,K);
            A = a[I+K*nblocks]; 
            for (j=kl, J=K+1; j<n; j+=bs, J++) {
                jl = j + bs; 
                if (jl > n) {
                    jl = n;
                    strJ= jl - j;
                } else {
                    strJ = bs;
                }
                if (BlockOwner(I, J) == MyNum) {  /* parcel out blocks */
                    B = a[K+J*nblocks]; 
                    C = a[I+J*nblocks];
                    bmod(A, B, C, strI, strJ, strK, strI, strK, strI);
                }
            }
        }
#ifdef DEBUG
        printf("k = %d, after modify subsequent block columns\n", k);    
        PrintA();
#endif
    }
}


void InitA(rhs)

double *rhs;

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
/*      a[ii][jj] = ((double) lrand48())/MAXRAND; */
            a[ii][jj] = i + j*6 + 1;
            if (i == j) {
                a[ii][jj] *= 10;
            }
        }
    }
    
    for (j=0; j<n; j++) {
        rhs[j] = 0.0;
    }
    for (j=0; j<n; j++) {
        for (i=0; i<n; i++) {
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
            rhs[i] += a[ii][jj];
        }
    }
}


double TouchA(bs, MyNum)

int bs; 
int MyNum;

{
    int i, j, I, J;
    double tot = 0.0;
    int ibs;
    int jbs;
    
    /* touch my portion of A[] */
    
    for (J=0; J<nblocks; J++) {
        for (I=0; I<nblocks; I++) {
            if (BlockOwner(I, J) == MyNum) {
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


void PrintA()

{
    int i, j;
    int ii, jj;
    int edge;
    int ibs, jbs, skip;
    
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
            printf("%8.1f ", a[ii][jj]);   
        }
        printf("\n");
    }
    fflush(stdout);
}


void CheckResult(n, a, rhs)

int n;
double **a; 
double *rhs;

{
    int i, j, bogus = 0;
    double *y, diff, max_diff;
    int ii, jj;
    int edge;
    int ibs, jbs, skip;
    
    edge = n%block_size;
    y = (double *) malloc(n*sizeof(double));  
    if (y == NULL) {
        printerr("Could not malloc memory for y\n");
        exit(-1);
    }
    for (j=0; j<n; j++) {
        y[j] = rhs[j];
    }
    for (j=0; j<n; j++) {
        if ((n - j) <= edge) {
            jbs = edge;
            jbs = n-edge;
            skip = edge;
        } else {
            jbs = block_size;
            skip = block_size;
        }
        ii = (j/block_size) + (j/block_size)*nblocks;
        jj = (j%jbs)+(j%jbs)*skip;
        
        y[j] = y[j]/a[ii][jj];
        for (i=j+1; i<n; i++) {
            if ((n - i) <= edge) {
                ibs = edge;
                ibs = n-edge;
                skip = edge;
            } else {
                ibs = block_size;
                skip = block_size;
            }
            ii = (i/block_size) + (j/block_size)*nblocks;
            jj = (i%ibs)+(j%jbs)*skip;
            
            y[i] -= a[ii][jj]*y[j];
        }
    }
    
    for (j=n-1; j>=0; j--) {
        for (i=0; i<j; i++) {
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
            y[i] -= a[ii][jj]*y[j];
        }
    }
    
    max_diff = 0.0;
    for (j=0; j<n; j++) {
        diff = y[j] - 1.0;
        if (fabs(diff) > 0.00001) {
            bogus = 1;
            max_diff = diff;
        }
    }
    if (bogus) {
        printf("TEST FAILED: (%.5f diff)\n", max_diff);
    } else {
    printf("TEST PASSED\n");
    }
    free(y);
}


void printerr(s)

char *s;

{
    fprintf(stderr,"ERROR: %s\n",s);
}
