#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
      //      BitMatPrint(mask[i][j]);
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
  unsigned long i;
  unsigned long long shift;
  double *v = malloc(sizeof(double)*(2 * n));

  unsigned long long mask  = 0x0102040810204080LL;
  shift = (unsigned long long) (log(n) / log(2));
  setupBitIndices();

  for (i = 0; i < n; i++) {
    unsigned long long ndx = BitMatMultOr(mask, BitMatMultOr(i, mask));
    ndx = bitRotLeft(ndx, shift);

    v[2 * ndx]     = w[2 * i];
    v[2 * ndx + 1] = w[2 * i + 1];
  }

  return(v);
}


int main() {
  int N = 32;
  double x[N/2];
  int i;
  for (i=0; i<N/2; i++) {
    x[i] = i;
  }
  for (i=0; i<N/2; i++) {
    printf("%lf ", x[i]);
  }
  printf("\n");
  double* y = bit_reverse(N/4, x);
  for (i=0; i<N/2; i++) {
    printf("%lf ", y[i]);
  }
  printf("\n");
}
