/* $Id$ */

#if defined(__crayx1)
#else
#define restrict
#endif

void  L_ACCUMULATE_2D(long* restrict alpha, int* restrict rows, 
                      int* restrict cols, long* restrict a, 
                      int* restrict lda, long* restrict b, int* restrict ldb)
{
int i,j;

#ifdef __crayx1
#pragma _CRI concurrent
#endif

   for(j=0;j< *cols; j++){
     long * restrict aa = a + j* *lda;
     long * restrict bb = b + j* *ldb;
     for(i=0;i< *rows; i++)
       aa[i] += *alpha * bb[i];
   }
}

void L_ACCUMULATE_1D(long * restrict alpha, long * restrict a, long * restrict b, 
                  int * restrict rows)
{
int i;
     for(i=0;i< *rows; i++)
       a[i] += *alpha * b[i];
}


#if defined(CRAY_T3E) || defined(CATAMOUNT)
void  F_ACCUMULATE_2D_(float* alpha, int* rows, int* cols, float* a,
                      int* lda, float* b, int* ldb)
{
int i,j;
   for(j=0;j< *cols; j++){
     float *aa = a + j* *lda;
     float *bb = b + j* *ldb;
     for(i=0;i< *rows; i++)
       aa[i] += *alpha * bb[i];
   }
}
#endif

#ifdef NOFORT

typedef struct {
  float imag;
  float real;
} cmpl_t;

typedef struct {
  double imag;
  double real;
} dcmpl_t;

void  I_ACCUMULATE_2D(int* alpha, int* rows, int* cols, int* a,
                      int* lda, int* b, int* ldb)
{
int i,j;

   for(j=0;j< *cols; j++){
     int *aa = a + j* *lda;
     int *bb = b + j* *ldb;
     for(i=0;i< *rows; i++)
       aa[i] += *alpha * bb[i];
   }
}

#if !defined(CRAY_T3E) && !defined(CATAMOUNT)
void  F_ACCUMULATE_2D(float* alpha, int* rows, int* cols, float* a,
                      int* lda, float* b, int* ldb)
{
int i,j;
   for(j=0;j< *cols; j++){
     float *aa = a + j* *lda;
     float *bb = b + j* *ldb;
     for(i=0;i< *rows; i++)
       aa[i] += *alpha * bb[i];
   }
}
#endif

void  D_ACCUMULATE_2D(double* alpha, int* rows, int* cols, double* a,
                      int* lda, double* b, int* ldb)
{
int i,j;

   for(j=0;j< *cols; j++){
     double *aa = a + j* *lda;
     double *bb = b + j* *ldb;
     for(i=0;i< *rows; i++)
       aa[i] += *alpha * bb[i];
   }
}


void  C_ACCUMULATE_2D(cmpl_t* alpha, int* rows, int* cols, cmpl_t* a,
                      int* lda, cmpl_t* b, int* ldb)
{
int i,j;

   for(j=0;j< *cols; j++){
     cmpl_t *aa = a + j* *lda;
     cmpl_t *bb = b + j* *ldb;
     for(i=0;i< *rows; i++){
       aa[i].real  += alpha->real * bb[i].real - alpha->imag * bb[i].imag;
       aa[i].imag  += alpha->imag * bb[i].real + alpha->real * bb[i].imag;
     }
   }
}


void  Z_ACCUMULATE_2D(dcmpl_t* alpha, int* rows, int* cols, dcmpl_t* a,
                      int* lda, dcmpl_t* b, int* ldb)
{
int i,j;


   for(j=0;j< *cols; j++){
     dcmpl_t *aa = a + j* *lda;
     dcmpl_t *bb = b + j* *ldb;
     for(i=0;i< *rows; i++){
       aa[i].real  += alpha->real * bb[i].real - alpha->imag * bb[i].imag;
       aa[i].imag  += alpha->imag * bb[i].real + alpha->real * bb[i].imag;
     }
   }
}

void FORT_DADD(int *n, double *x, double *work){
int i;
    for(i=0;i<*n;i++)
       x[i] = x[i] + work[i];
}
void FORT_DADD2(int *n, double *x, double *work, double *work2){
int i;
    for(i=0;i<*n;i++)
       x[i] = work[i] + work2[i];
}
void FORT_DMULT(int *n, double *x, double *work){
int i;
    for(i=0;i<*n;i++)
       x[i] = x[i]*work[i];
}
void FORT_DMULT2(int *n, double *x, double *work, double *work2){
int i;
    for(i=0;i<*n;i++)
       x[i] = work[i]*work2[i];
}

#endif
