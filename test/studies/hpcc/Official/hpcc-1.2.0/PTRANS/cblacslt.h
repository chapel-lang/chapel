
#define SGET_SYSCONTXT    0
#define SGET_BLACSCONTXT 10

extern double dcputime00(void);
extern double dwalltime00(void);
extern void Cblacs_abort(int ConTxt, int ErrNo);
extern void Cblacs_barrier(int ConTxt, char *scope);
extern void Cblacs_exit(int NotDone);
extern void Cblacs_get(int ConTxt, int what, int *val);
extern void Cblacs_gridexit(int ConTxt);
extern void Cblacs_gridinfo(int ConTxt, int *nprow, int *npcol, int *myrow, int *mycol);
extern void Cblacs_gridinit(int *ConTxt, char *order, int nprow, int npcol);
extern void Cblacs_gridmap(int *ConTxt, int *umap, int ldumap, int nprow, int npcol);
extern void Cblacs_pinfo(int *mypnum, int *nprocs);
extern void Cdgamn2d(int ConTxt, char *scope, char *top, int m, int n, double *A, int lda, int *rA,
  int *cA, int ldia, int rdest, int cdest);
extern void Cdgamx2d(int ConTxt, char *scope, char *top, int m, int n, double *A, int lda,
  int *rA, int *cA, int ldia, int rdest, int cdest);
extern void Cdgebr2d(int ConTxt, char *scope, char *top, int m, int n, double *A,
  int lda, int rsrc, int csrc);
extern void Cdgebs2d(int ConTxt, char *scope, char *top, int m, int n, double *A, int lda);
extern void Cdgerv2d(int ConTxt, int m, int n, double *A, int lda, int rsrc, int csrc);
extern void Cdgesd2d(int ConTxt, int m, int n, double *A, int lda, int rdest, int cdest);
extern void Cdgsum2d(int ConTxt, char *scope, char *top, int m, int n, double *A, int lda,
  int rdest, int cdest);
extern void Cigebr2d(int ConTxt, char *scope, char *top, int m, int n, int *A, int lda, int rsrc,
  int csrc);
extern void Cigebs2d(int ConTxt, char *scope, char *top, int m, int n, int *A, int lda);
extern void Cigsum2d(int ConTxt, char *scope, char *top, int m, int n, int *A, int lda,
  int rdest, int cdest);
extern void Cblacs_dSendrecv(int ctxt, int mSrc, int nSrc, double *Asrc, int ldaSrc, int rdest,
  int cdest, int mDest, int nDest, double *Adest, int ldaDest, int rsrc, int csrc);
