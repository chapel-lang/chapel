//cft1st() optimized
//          
//        dfft()
//        - cft1st() does first
//        - cftmd1() does next set: S outside, T inside
//          - cftmd0() does first
//       - cftmd2() does next set: T outside, S inside
//          - cftmd0() does first
//          - cftmd21() does special-case





#include <math.h>
#include <stdlib.h>
#include <stdio.h>


int N2;

void resetA(double* a) {
  /*
  int i;
  for (i=0; i<N2; i++) {
    a[i] = i;
  }
  */
}


void writeA(double* a) {
  printf("a[] =");
  for (int i=0; i<N2; i++) {
    printf(" %g", a[i]);
  }
  printf("\n");
}



void btrfly(int j, double wk1r, double wk1i, double wk2r, double wk2i, double wk3r, double wk3i, double* a, double* b, double* c, double* d)
{ double x0r = a[j    ] + b[j    ];
  double x0i = a[j + 1] + b[j + 1];
  double x1r = a[j    ] - b[j    ];
  double x1i = a[j + 1] - b[j + 1];
  double x2r = c[j    ] + d[j    ];
  double x2i = c[j + 1] + d[j + 1];
  double x3r = c[j    ] - d[j    ];
  double x3i = c[j + 1] - d[j + 1];

  printf("    a={%g,%g}, b={%g,%g}, c={%g,%g}, d={%g,%g}\n", 
         a[j], a[j+1], b[j], b[j+1], c[j], c[j+1], d[j], d[j+1]);
  printf("      wk1=%g + %gi\n"
         "      wk2=%g + %gi\n"
         "      wk3=%g + %gi\n",
         wk1r, wk1i, wk2r, wk2i, wk3r, wk3i);

  a[j    ] = x0r + x2r;
  a[j + 1] = x0i + x2i;
  x0r -= x2r;
  x0i -= x2i;
  c[j    ] = wk2r * x0r - wk2i * x0i;
  c[j + 1] = wk2r * x0i + wk2i * x0r;
  x0r = x1r - x3i;
  x0i = x1i + x3r;
  b[j    ] = wk1r * x0r - wk1i * x0i;
  b[j + 1] = wk1r * x0i + wk1i * x0r;
  x0r = x1r + x3i;
  x0i = x1i - x3r;
  d[j    ] = wk3r * x0r - wk3i * x0i;
  d[j + 1] = wk3r * x0i + wk3i * x0r;

  printf("    a={%g,%g}, b={%g,%g}, c={%g,%g}, d={%g,%g}\n\n", 
         a[j], a[j+1], b[j], b[j+1], c[j], c[j+1], d[j], d[j+1]);
}

void BitMatPrint(unsigned long long x) {
  unsigned long long mask = 0x8000000000000000LL;
  int i, j;

  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      if (x & mask) {
        printf("1");
      } else {
        printf("0");
      }
      mask >>= 1;
    }
    printf("\n");
  }
  printf("\n");
}


static unsigned long long bitInd[8][8];

static void setupBitIndices(void) {
  int i, j;
  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      bitInd[i][j] = 0x1LL << (((7-i)*8) + (7-j));
      //      printf("\n");
      //      BitMatPrint(bitInd[i][j]);
    }
    //    printf("\n");
  }
}

unsigned long long BitMatMultOr(unsigned long long x, unsigned long long y) {
  int i, j, k;
  unsigned long long result = 0x0LL;

  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      int temp = 0;
      for (k=0; k<8; k++) {
        temp |= ((bitInd[i][k] & x) != 0) && ((bitInd[k][j] & y) != 0);
      }
      if (temp) {
        result |= bitInd[i][j];
      }
    }
  }
  //  BitMatPrint(result);

  return result;
}


unsigned long long bitRotLeft(unsigned long long x, unsigned long long y) {
  unsigned long long result;

  result = (x << y) | (x >> (64-y));
  
  return result;
}

double * bit_reverse(int n, double *w) {
  unsigned long long i;
  unsigned long long mask;
  unsigned long long shift;
  double *v = new double[2 * n];

  mask  = 0x0102040810204080LL;
  int ncopy = n;
  shift = 0;
  while (ncopy != 1) {
    ncopy = ncopy >> 1;
    shift++;
  }
  /*  if (n%2 == 0) {
    shift += 1;
  }
  */

#pragma mta use 100 streams
#pragma mta assert no dependence
  for (i = 0; i < n; i++) {
    //    printf("i = %d\n", i);
      unsigned long long ndx = BitMatMultOr(mask, BitMatMultOr(i, mask));
      ndx = bitRotLeft(ndx, shift);
      //      printf("ndx = %llu\n", ndx);
      //      printf("i = %d\n", i);
      v[2 * ndx]     = w[2 * i];
      v[2 * ndx + 1] = w[2 * i + 1];
  }

  free(w);
  return(v);
}

void twiddles(int n, double *w)
{ int i;
  double delta = atan(1.0) / n;

  w[0]     = 1;
  w[1]     = 0;
  w[n]     = cos(delta * n);
  w[n + 1] = w[n];

#pragma mta assert no dependence
  for (i = 2; i < n; i += 2) {
      double x = cos(delta * i);
      double y = sin(delta * i);
      w[i]             = x;
      w[i + 1]         = y;
      w[2 * n - i]     = y;
      w[2 * n - i + 1] = x;
} }

void cft1st(int n, double *a, double *w)
{ int j, k1;

  double *v   = w + 1;
  double *b   = a + 2;
  double *c   = a + 4;
  double *d   = a + 6;
  double wk1r = w[2];

  double x0r = a[0] + a[2];
  double x0i = a[1] + a[3];
  double x1r = a[0] - a[2];
  double x1i = a[1] - a[3];
  double x2r = a[4] + a[6];
  double x2i = a[5] + a[7];
  double x3r = a[4] - a[6];
  double x3i = a[5] - a[7];

  a[0] = x0r + x2r;
  a[1] = x0i + x2i;
  a[4] = x0r - x2r;
  a[5] = x0i - x2i;
  a[2] = x1r - x3i;
  a[3] = x1i + x3r;
  a[6] = x1r + x3i;
  a[7] = x1i - x3r;

  x0r  = a[8]  + a[10];
  x0i  = a[9]  + a[11];
  x1r  = a[8]  - a[10];
  x1i  = a[9]  - a[11];
  x2r  = a[12] + a[14];
  x2i  = a[13] + a[15];
  x3r  = a[12] - a[14];
  x3i  = a[13] - a[15];
  a[8] = x0r + x2r;
  a[9] = x0i + x2i;
  a[12] = x2i - x0i;
  a[13] = x0r - x2r;
  x0r   = x1r - x3i;
  x0i   = x1i + x3r;
  a[10] = wk1r * (x0r - x0i);
  a[11] = wk1r * (x0r + x0i);
  x0r   = x3i + x1r;
  x0i   = x3r - x1i;
  a[14] = wk1r * (x0i - x0r);
  a[15] = wk1r * (x0i + x0r);

  printf("  // computes first 8 complexes manually\n");

#pragma mta use 100 streams
#pragma mta no scalar expansion
#pragma mta assert no dependence
  for (j = 16, k1 = 2; j < n; j += 16, k1 += 2) {
      double wk2r = w[k1];
      double wk2i = v[k1];
      double wk1r = w[k1 + k1];
      double wk1i = v[k1 + k1];
      double wk3r = wk1r - 2 * wk2i * wk1i;
      double wk3i = 2 * wk2i * wk1r - wk1i;

      resetA(a);
      btrfly (j, wk1r, wk1i, wk2r, wk2i, wk3r, wk3i, a, b, c, d);

      //      writeln("accessing: %d\n", k1+k1+2/2);
      wk1r = w[k1 + k1 + 2];
      wk1i = v[k1 + k1 + 2];
      wk3r = wk1r - 2 * wk2r * wk1i;
      wk3i = 2 * wk2r * wk1r - wk1i;

      resetA(a);
      btrfly (j + 8, wk1r, wk1i, - wk2i, wk2r, wk3r, wk3i, a, b, c, d);
} }

void cftmd0(int n, int l, double *a, double *w)
{ int j, m = l << 2;

  double wk1r = w[2];

  double *v = w + 1;
  double *b = a + l;
  double *c = a + l + l;
  double *d = a + l + l + l;

  writeA(a);

#pragma mta use 100 streams
#pragma mta assert no dependence
  for (j = 0; j < l; j += 2) {
    resetA(a);
    btrfly(j, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, a, b, c, d);
  }

#pragma mta use 100 streams
#pragma mta assert no dependence
  for (j = m; j < l + m; j += 2) {
    resetA(a);
    btrfly(j, wk1r, wk1r, 0.0, 1.0, - wk1r, wk1r, a, b, c, d);
  }
}

void cftmd1(int n, int l, double *a, double *w)
{ int j, k, k1;

  int m  = l << 2;
  int m2 = 2 * m;

  double *v = w + 1;
  double *b = a + l;
  double *c = a + l + l;
  double *d = a + l + l + l;

  printf("  cftmd0(n=%d, l=%d, a, w)\n", n, l);
  cftmd0(n, l, a, w);

#pragma mta use 100 streams
#pragma mta no scalar expansion
#pragma mta assert no dependence
  for (k = m2, k1 = 2; k < n; k += m2, k1 += 2) {
      double wk2r = w[k1];
      double wk2i = v[k1];
      double wk1r = w[k1 + k1];
      double wk1i = v[k1 + k1];
      double wk3r = wk1r - 2 * wk2i * wk1i;
      double wk3i = 2 * wk2i * wk1r - wk1i;

      for (j = k; j < l + k; j += 2) {
        resetA(a);
        btrfly (j, wk1r, wk1i, wk2r, wk2i, wk3r, wk3i, a, b, c, d);
      }

      wk1r = w[k1 + k1 + 2];
      wk1i = v[k1 + k1 + 2];
      wk3r = wk1r - 2 * wk2r * wk1i;
      wk3i = 2 * wk2r * wk1r - wk1i;

      for (j = k + m; j < l + k + m; j += 2) {
        resetA(a);
        btrfly (j, wk1r, wk1i, - wk2i, wk2r, wk3r, wk3i, a, b, c, d);
      }
} }

void cftmd21(int n, int l, double *a, double *w)
{ int j, k, k1;
  int m  = l << 2;
  int m2 = 2 * m;
  int m3 = 3 * m;

  double *v = w + 1;
  double *b = a + l;
  double *c = a + l + l;
  double *d = a + l + l + l;

  for (k = m2, k1 = 2; k < n; k += m2, k1 += 2) {
      double wk2r = w[k1];
      double wk2i = v[k1];
      double wk1r = w[k1 + k1];
      double wk1i = v[k1 + k1];
      double wk3r = wk1r - 2 * wk2i * wk1i;
      double wk3i = 2 * wk2i * wk1r - wk1i;

#pragma mta use 100 streams
#pragma mta assert no dependence
      for (j = k; j < k + l; j += 2) {
        resetA(a);
        btrfly (j, wk1r, wk1i, wk2r, wk2i, wk3r, wk3i, a, b, c, d);
      }

      wk1r = w[k1 + k1 + 2];
      wk1i = v[k1 + k1 + 2];
      wk3r = wk1r - 2 * wk2r * wk1i;
      wk3i = 2 * wk2r * wk1r - wk1i;

#pragma mta use 100 streams
#pragma mta assert no dependence
      for (j = k + m; j < k + m + l; j += 2) {
        resetA(a);
        btrfly (j, wk1r, wk1i, - wk2i, wk2r, wk3r, wk3i, a, b, c, d);
      }

} }

void cftmd2(int n, int l, double *a, double *w)
{ int j, k, k1;

  int m  = l << 2;
  int m2 = 2 * m;

  double *v = w + 1;
  double *b = a + l;
  double *c = a + l + l;
  double *d = a + l + l + l;

  printf("  cftmd0(n=%d, l=%d, a, w)\n", n, l);
  cftmd0(n, l, a, w);

  if (m2 >= n) return;
  if (m2 >= n / 8) { 
    printf("  cftmd21(n=%d, l=%d, a, w)\n", n, l); 
    cftmd21(n, l, a, w); 
    return;
  }

#pragma mta use 100 streams
#pragma mta assert no dependence
  for (j = 0; j < l; j += 2)  {
#pragma mta assert no dependence
  for (k = m2, k1 = 2; k < n; k += m2, k1 += 2) {
      double wk2r = w[k1];
      double wk2i = v[k1];
      double wk1r = w[k1 + k1];
      double wk1i = v[k1 + k1];
      double wk3r = wk1r - 2 * wk2i * wk1i;
      double wk3i = 2 * wk2i * wk1r - wk1i;

      resetA(a);
      btrfly (j + k, wk1r, wk1i, wk2r, wk2i, wk3r, wk3i, a, b, c, d);
  }

#pragma mta assert no dependence
  for (k = m2, k1 = 2; k < n; k += m2, k1 += 2) {
      double wk2r = w[k1];
      double wk2i = v[k1];
      double wk1r = w[k1 + k1 + 2];
      double wk1i = v[k1 + k1 + 2];
      double wk3r = wk1r - 2 * wk2r * wk1i;
      double wk3i = 2 * wk2r * wk1r - wk1i;

      resetA(a);
      btrfly (j + k + m, wk1r, wk1i, - wk2i, wk2r, wk3r, wk3i, a, b, c, d);

} } }


void dfft(int n, int logn, double *a, double *w)
{ int i, l, j;
  double *v, *b, *c, *d;

  printf("w[] =");
  for (i=0; i< n/4; i++) {
    printf(" %g", w[i]);
  }
  printf("\n");

  printf("cft1st();\n", n);
  cft1st(n, a, w);

  i = 4; l = 8;

  for ( ; i <= logn / 2; i += 2, l *= 4) {
    printf("cftmd1(n=%d, l=%d, a, w)\n", n, l, a, w);
    cftmd1(n, l, a, w);
  }
  for ( ; i <= logn - 1; i += 2, l *= 4) {
    printf("i=%d  cftmd2(n=%d, l=%d, a, w)\n", i, n, l, a, w);
    cftmd2(n, l, a, w);
  }

  v = w + 1;
  b = a + l;
  c = a + l + l;
  d = a + l + l + l;

  if ((l << 2) == n) {
    printf("l << 2 == n\n");
#pragma mta use 100 streams
#pragma mta assert no dependence
    for (j = 0; j < l; j += 2) {
      resetA(a);
      btrfly(j, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, a, b, c, d);
    }
  } else {
    printf("l << 2 != n\n");

#pragma mta use 100 streams
#pragma mta assert no dependence
     for (j = 0; j < l; j += 2) {
         double x0r = a[j    ];
         double x0i = a[j + 1];
         double x1r = b[j    ];
         double x1i = b[j + 1];
         a[j    ]   = x0r + x1r;
         a[j + 1]   = x0i + x1i;
         b[j    ]   = x0r - x1r;
         b[j + 1]   = x0i - x1i;
} }  }

int main(int argc, char *argv[])
{ int i;
  double gflop, maxerr, time;

  setupBitIndices();

  int logN;
  if (argc > 1) {
    logN = atoi(argv[1]);
  } else {
    logN = 5;
  }
  int N     = 1 << logN;

  double EPS = pow(2.0, -51.0);
  double THRESHOLD = 16.0;

  double *a = new double[N * 2];
  double *b = new double[N * 2];
  double *w = new double[N / 2];

  N2 = 2 * N;
  for (i=0; i<N2; i++) {
    a[i] = i;
  }

/* save a for verification step */
  for (i = 0; i < N2; i++) b[i] = a[i];

  twiddles(N / 4, w);
  /*
  printf("w[] =");
  for (i=0; i< N/2; i++) {
    printf(" %g", w[i]);
  }
  printf("\n");  
  */
  w = bit_reverse(N / 4, w);
  /*
  printf("w[] =");
  for (i=0; i< N/2; i++) {
    printf(" %g", w[i]);
  }
  printf("\n");
  */


/* conjugate data */
#pragma mta assert parallel
  //  for (i = 1; i < N2; i += 2) a[i] = -a[i];

  a = bit_reverse(N, a);
  /*
  printf("a[] =");
  for (i=0; i<N2; i++) {
    printf(" %g", a[i]);
  }
  printf("\n");
  */
  writeA(a);
  dfft(N2, logN, a, w);
  exit(0);
}
