#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#ifndef useInt
#define useInt 0
#endif

typedef int indexType;
const int sliceMax = INT_MAX; // to express an unbounded range
const int sliceMin = INT_MIN;

#if useInt

typedef int elemType;
#define elemFmt "%d"
#define absFun abs
#define elemMin INT_MIN
#define elemTypeString "int"

elemType myrnd(void) { return rand(); }

#else

typedef double elemType;
#define elemFmt "%10.6f"
#define absFun fabs
#define elemMin ((double)(-1e300))
#define elemTypeString "real"

elemType myrnd(void) { return ((double)(rand() - RAND_MAX/2)) / (RAND_MAX/2); }

#endif //useInt

// static buffer for the results of tost()
#define tostLen 16
char tosts[10][tostLen];
//
char* tost(int tostNo, elemType arg) {
  assert(tostNo >= 0 && tostNo < 10);
  char* buf = &tosts[tostNo][0];
  *(buf++) = ' ';
  snprintf(buf, tostLen-1, elemFmt, arg);
  return buf-1;
}

void writeln(void) { printf("\n"); }

int n=5;
int blkSize=2;
int seed = 31415;

int verb = 1;
int showresult = 1;
int dev  = 1;


/////////////////////////////////////////////////////////////////////////////
// domains, arrays, for-loops

// name structure:  purpose numDims elementType [sli]
// purpose:
//   'dom' 'ar' 'tup' -> data type
//   'sli' -> array slice
//   'create' etc. -> operations
// numDims
//   1 or 2 etc.
// elementType:
//   d -> domain  e -> array of elemType  i -> array of indexType
// sli:
//   operation is on an array slice

typedef struct { int l, h; } dom1d;
typedef struct { int l1, h1, l2, h2; } dom2d;
typedef struct { indexType* elms; int l,h; } ar1i;
typedef struct { elemType*  elms; int l,h; } ar1e;
typedef struct { elemType* elms; dom2d dom; } ar2e;
typedef struct { ar2e origarr; dom2d slicedom; } sli2e;

// for-loops over ranges
#define for_low_hi(var, low, hi)  for_low_hi_step(var, (low), (hi), 1)
#define for_dom1d(var, dom)       for_low_hi_step(var, (dom).l, (dom).h, 1)
#define for_dom_d1(var, dom)      for_dom_d1_step(var, (dom), 1)
#define for_dom_d2(var, dom)      for_dom_d2_step(var, (dom), 1)
//
#define for_dom_d1_step(var, dom, step)  for_low_hi_step(var, (dom.l1), (dom.h1), (step))
#define for_dom_d2_step(var, dom, step)  for_low_hi_step(var, (dom.l2), (dom.h2), (step))
#define for_low_hi_step(var, low, hi, step)  for (int var = (low); var <= (hi); var += (step))
#define for_hi_low(var, low, hi)  for (int var = (hi); var >= (low); var--)

// for zippered iterations, skip length checking
#define for_zipdoms_d1_d1(varx, vary, domx, domy) \
  for_zippered(varx, vary, (domx).l1, (domx).h1, (domy).l1, (domy).h1)
#define for_zipdoms_d2_d1(varx, vary, domx, domy) \
  for_zippered(varx, vary, (domx).l2, (domx).h2, (domy).l1, (domy).h1)
#define for_zipdoms_d1_d2(varx, vary, domx, domy) \
  for_zippered(varx, vary, (domx).l1, (domx).h1, (domy).l2, (domy).h2)
#define for_zipdoms_d2_d2(varx, vary, domx, domy) \
  for_zippered(varx, vary, (domx).l2, (domx).h2, (domy).l2, (domy).h2)
#define for_zippered(varx, vary, lx, hx, ly, hy) \
  for (int varx = (lx), vary = (ly); varx <= (hx); varx++, vary++)


/////////////////////////////////////////////////////////////////////////////
// basic domain/array utilities

// ne: numElements
int ne1d(int l, int h) { return h-l+1; }
int ne2d(dom2d dom) { return ne1d(dom.l1, dom.h1) * ne1d(dom.l2, dom.h2); }

void show2d(const char* msg, dom2d dom) {
  printf("%s [%d..%d, %d..%d]\n", msg, dom.l1, dom.h1, dom.l2, dom.h2);
}

// show as range, no new line
void show1drnnl(dom1d dom) {
  printf("%d..%d", dom.l, dom.h);
}

ar2e* create2e(ar2e* arr, dom2d dom) {
  //if (dev) show2d("create2e", dom);
  void* mem = malloc(ne2d(dom) * sizeof(elemType));
  arr->elms = (elemType*)mem;
  arr->dom  = dom;
  return arr;
}
ar1i* create1i(ar1i* arr, int l, int h) {
  //if (dev) printf("create1i [%d..%d]\n", l, h);
  void* mem = malloc(ne1d(l,h) * sizeof(indexType));
  arr->elms = (indexType*)mem;
  arr->l    = l;
  arr->h    = h;
  return arr;
}
ar1e* create1e(ar1e* arr, int l, int h) {
  //if (dev) printf("create1e [%d..%d]\n", l, h);
  void* mem = malloc(ne1d(l,h) * sizeof(elemType));
  arr->elms = (elemType*)mem;
  arr->l    = l;
  arr->h    = h;
  return arr;
}
elemType* adr2e(ar2e* arr, int ix1, int ix2) {
  dom2d dom = arr->dom;
  // 2nd index changes faster
  return &arr->elms[(ix1 - dom.l1) * ne1d(dom.l2, dom.h2) +
                    (ix2 - dom.l2)];
}
elemType get2e(ar2e* arr, int ix1, int ix2) {
  return *adr2e(arr, ix1, ix2);
}
void show2e(ar2e* arr) {
  dom2d dom = arr->dom;
  for_dom_d1(i1, dom) {
    for_dom_d2(i2, dom)
      printf("%s", tost(0, get2e(arr, i1, i2)));
    writeln();
  }
}
elemType* adr2esli(sli2e* slice, int ix1, int ix2) {
  return adr2e(&(slice->origarr), ix1, ix2);
}
elemType get2esli(sli2e* slice, int ix1, int ix2) {
  return *adr2esli(slice, ix1, ix2);
}
void show2esli(sli2e* slice) {
  dom2d dom = slice->slicedom;
  for_dom_d1(i1, dom) {
    for_dom_d2(i2, dom)
      printf("%s", tost(0, get2esli(slice, i1, i2)));
    writeln();
  }
}

int* adr1i(ar1i* arr, int ix1) {
  return &arr->elms[ix1 - arr->l];
}
int get1i(ar1i* arr, int ix1) {
  return *adr1i(arr, ix1);
}
void show1i(ar1i* arr) {
  for_low_hi(i1, arr->l, arr->h) {
    if (i1 != arr->l) printf(" ");
    printf("%d", get1i(arr, i1));
  }
  writeln();
}
elemType* adr1e(ar1e* arr, int ix1) {
  return &arr->elms[ix1 - arr->l];
}
elemType get1e(ar1e* arr, int ix1) {
  return *adr1e(arr, ix1);
}
void show1e(ar1e* arr) {
  for_low_hi(i1, arr->l, arr->h) {
    printf("%s", tost(0, get1e(arr, i1)));
  }
  writeln();
}

#define setMax(dest,s1,s2) ( (dest) = ( (s1) > (s2) ) ? (s1) : (s2) )
#define setMin(dest,s1,s2) ( (dest) = ( (s1) < (s2) ) ? (s1) : (s2) )

// slicing
dom2d slice2d(dom2d slicee, int nl1, int nh1, int nl2, int nh2) {
  dom2d result;
  setMax(result.l1, slicee.l1, nl1);
  setMin(result.h1, slicee.h1, nh1);
  setMax(result.l2, slicee.l2, nl2);
  setMin(result.h2, slicee.h2, nh2);

  return result;
}

sli2e* slice2e(sli2e* result, ar2e* origarr, dom2d slicedom) {
  result->origarr = *origarr;
  result->slicedom = slicedom;
  return result;
}
sli2e* slice2esli(sli2e* result, sli2e* origslice, dom2d slicedom) {
  result->origarr = origslice->origarr;
  result->slicedom = slicedom;
  return result;
}


/////////////////////////////////////////////////////////////////////////////
// problem domain utilities

void printConfiguration() {
  int bytesPerArray = n * n * sizeof(elemType);
  printf("Problem size = %d x %d\n", n, n);
  printf("Bytes per array = %d\n", bytesPerArray);
  printf("block size = %d\n", blkSize);
  writeln();
}

void verifyResults(ar2e* Ab) {
  if (verb) { show2e(Ab); writeln(); }
}

void initAB(ar2e* Ab) {
  srand(seed);

  int numElms = ne2d(Ab->dom);
  elemType* curAddr = Ab->elms;
  for_low_hi(i, 1, numElms) *(curAddr++) = myrnd();
}

typedef struct { int ix1; int ix2; } tup2i;
typedef struct { elemType val; tup2i ix; } helptup1;

// &helpfun1 is of the type helpfun1type
typedef elemType helpfun1type(elemType ab);
elemType helpfun1(elemType ab) {
  return absFun(ab);
}

//typedef helptup1 helpfun1type(ar2e* Ab, tup2i ix);
//helptup1 helpfun1(ar2e* Ab, tup2i ix) {
//  helptup1 result = { absFun(get2e(Ab, ix.ix1, ix.ix2)), ix };
//  return result;
//}

// maxloc reduce(abs(Ab(col)), col);
// todo: pass a slice of Ab
helptup1 maxlocreduce1(ar2e* Ab, dom2d col, helpfun1type f) {
  tup2i first = { col.l1, col.l2 };
  helptup1 result = { elemMin, first };
  for_dom_d1(ix1, col) {
    for_dom_d2(ix2, col) {
      tup2i curIx = { ix1, ix2 };
      elemType curVal = f(get2e(Ab, curIx.ix1, curIx.ix2));
      if ( (curVal > result.val) ||
           (curVal == result.val &&
            ( (curIx.ix1 < result.ix.ix1) ||
              (curIx.ix1 == result.ix.ix1 &&
               curIx.ix2 < result.ix.ix2) )
           )
         )
        result.val = curVal, result.ix = curIx;
    } // for ix2
  } // for ix1
  return result;
}

#define swap(type,e1,e2) { type tmp = (e1); (e1) = (e2); (e2) = tmp; }

void panelSolve(ar2e* Ab, dom2d panel, ar1i* piv) {
  if (verb) printf("panelSolve\n");

  // for k in panel.dim(1)
  for_dom_d2(k, panel) {
    if (verb) printf("== %d\n", k);
    dom2d col = slice2d(panel, k, sliceMax, k, k);

    if (ne2d(col) == 0) return;

    helptup1 pivotRed = maxlocreduce1(Ab, col, &helpfun1);
    int pivotRow = pivotRed.ix.ix1;
    elemType pivotVal = get2e(Ab, pivotRed.ix.ix1, k);

    if (verb) printf("( " elemFmt ",  " elemFmt ", %d, %d)\n",
                pivotVal, pivotRed.val, pivotRow, pivotRed.ix.ix2);

    for_dom_d2(ix2, Ab->dom)
      swap(elemType, *adr2e(Ab, k, ix2), *adr2e(Ab, pivotRow, ix2));
    swap(int, *adr1i(piv, k), *adr1i(piv, pivotRow));

    if (verb) { printf("after swapping\n"); show1i(piv); show2e(Ab); }

    if (pivotVal == 0) {
      printf("halt reached - Matrix cannot be factorized\n");
      exit(1);
    }

    for_low_hi(ix1, k+1, Ab->dom.h1) *adr2e(Ab, ix1, k) /= pivotVal;

    if (verb) { printf("after normalizing\n"); show1i(piv); show2e(Ab); }

    for_low_hi(i, k+1, panel.h1)
      for_low_hi(j, k+1, panel.h2)
        *adr2e(Ab, i, j) -= get2e(Ab, i, k) * get2e(Ab, k, j);

    if (verb) { printf("after updating\n"); show1i(piv); show2e(Ab); writeln(); }

  } // for k
}

void updateBlockRow(ar2e* Ab, dom2d tl, dom2d tr) {
  if (verb) printf("updateBlockRow\n");

  // for row in tr.dim(0)
  for_dom_d1(row, tr) {

    dom2d activeRow = slice2d(tr, row, row, sliceMin, sliceMax);
    dom1d prevRows  = { tr.l1, row-1 };
    if (verb) {
      printf("== %d  ", row);
      show1drnnl(prevRows);
      show2d(" ", activeRow);
    }

    for_dom_d1(i, activeRow)
      for_dom_d2(j, activeRow)
        for_dom1d(k, prevRows)
          *adr2e(Ab, i, j) -= get2e(Ab, i, k) * get2e(Ab, k, j);

    if (verb) { show2e(Ab); writeln(); }
  } // for row
}

void dgemmNativeInds(sli2e* A, sli2e* B, sli2e* C) {
  if (verb) { show2esli(A); printf("=\n"); show2esli(B); printf("=\n");
              show2esli(C); printf("===\n"); }
  if (1) {
    for_zipdoms_d1_d1(iA, iC, A->slicedom, C->slicedom) {
      for_zipdoms_d2_d1(jA, iB, A->slicedom, B->slicedom) {
        for_zipdoms_d2_d2(jB, jC, B->slicedom, C->slicedom) {
          *adr2esli(C, iC, jC) -= get2esli(A, iA, jA) * get2esli(B, iB, jB);
        }
      }
    }
  }
}

void schurComplement(ar2e* Ab, dom2d AD, dom2d BD, dom2d Rest) {
  if (verb) printf("schurComplement\n");

  dom2d AbD = Ab->dom;
  dom2d replAD = AD;
  dom2d replBD = BD;

  // how to translate slicing?
  sli2e replA_r, *replA = slice2e(&replA_r, Ab, replAD);
  sli2e replB_r, *replB = slice2e(&replB_r, Ab, replBD);

  // forall (row,col) in Rest by (blkSize, blkSize)
  for_dom_d1_step(row, Rest, blkSize) {
    for_dom_d2_step(col, Rest, blkSize) {

      int rowP = row+blkSize-1, colP = col+blkSize-1;
      dom2d aBlkD = slice2d(replAD, row, rowP, sliceMin, sliceMax),
            bBlkD = slice2d(replBD, sliceMin, sliceMax, col, colP),
            cBlkD = slice2d(AbD, row, rowP, col, colP);

      if (verb) {
        printf("== %d,%d\n", row, col);
        show2d("aBlkD", aBlkD);
        show2d("bBlkD", bBlkD);
        show2d("cBlkD", cBlkD);
      }

      sli2e replASlice_r, *replASlice = slice2esli(&replASlice_r, replA, aBlkD);
      sli2e replBSlice_r, *replBSlice = slice2esli(&replBSlice_r, replB, bBlkD);
      sli2e AbSlice_r,    *AbSlice    = slice2e(&AbSlice_r, Ab, cBlkD);

      // how to express the slices?
      dgemmNativeInds(replASlice, replBSlice, AbSlice);

      if (verb) { show2e(Ab); writeln(); }
    } // for col
  } // for row
}

void LUFactorize(int n, ar2e* Ab, ar1i* piv) {
  dom2d AbD = Ab->dom;

  // piv = 1..n
  indexType* curi = piv->elms;
  for_low_hi(i, 1, n) *(curi++) = i;

  for_low_hi_step(blk, 1, n, blkSize) {
    const int blkP = blk+blkSize-1;

    dom2d tl = slice2d(AbD, blk, blkP, blk, blkP),
          tr = slice2d(AbD, blk, blkP, blk+blkSize, sliceMax),
          bl = slice2d(AbD, blk+blkSize, sliceMax, blk, blkP),
          br = slice2d(AbD, blk+blkSize, sliceMax, blk+blkSize, sliceMax),
          l  = slice2d(AbD, blk, sliceMax, blk, blkP);

    if (verb) {
      printf("blk = %d\n", blk);
      show2d("tl", tl);
      show2d("tr", tr);
      show2d("bl", bl);
      show2d("br", br);
      show2d("l ", l);
      printf("piv = "); show1i(piv);
      writeln();
    }

    panelSolve(Ab, l, piv);
    updateBlockRow(Ab, tl, tr);
    schurComplement(Ab, bl, tr, br);
  }
}

// &helpful2 is of the type helpfun2type
typedef elemType helpfun2type(ar2e*, int, int, ar1e*);
elemType helpfun2(ar2e* Ab, int i, int j, ar1e* x) {
  return get2e(Ab, i, j) * get1e(x, j);
}

// + reduce [j in i+1..bd.high] (Ab[i,j] * x[j])
elemType reduce2(int i, int l, int h, ar2e* Ab, ar1e* x, helpfun2type f) {
  elemType result = 0;
  for_low_hi(j, l, h)
    result += f(Ab, i, j, x);
  return result;
}

void backwardSub(ar1e* x, int n, ar2e* Ab) {
  dom1d bd = { Ab->dom.l1, Ab->dom.h1 };
  create1e(x, bd.l, bd.h);

  if (verb) printf("backwardSub\n");

  for_hi_low(i, bd.l, bd.h) {
    elemType reduc2 = reduce2(i, i+1, bd.h, Ab, x, &helpfun2);
    elemType newval = (get2e(Ab, i, n+1) - reduc2) / get2e(Ab, i, i);
    *adr1e(x, i) = newval;
    if (verb) printf("== %d%s\n", i, tost(0, get1e(x, i)));
  }
}


/////////////////////////////////////////////////////////////////////////////
// main

#include "wer.c"

int main(int argc, char *argv[]) {
  processOptions(argc, argv);
  printConfiguration();

  dom2d MatVectSpace = {1,n,1,n+1};
  dom2d MatrixSpace = {1,n,1,n};

  ar2e Ab_r,  *Ab  = create2e(&Ab_r, MatVectSpace);
  ar1i piv_r, *piv = create1i(&piv_r, 1, n);

  initAB(Ab);

  if (verb || showresult) {
    printf("initial " elemTypeString " %d\n", seed);
    show2e(Ab); writeln();
  }

  LUFactorize(n, Ab, piv);

  ar1e x_r, *x = &x_r;
  backwardSub(x, n, Ab);

  if (verb || showresult) { printf("result\n"); show1e(x); writeln(); }

  verifyResults(Ab);
  printf("Validation: SUCCESS\n");

  return 0;
}
