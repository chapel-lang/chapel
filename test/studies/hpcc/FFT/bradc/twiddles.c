void twiddles(int n, double *w)
{ int i;
  double delta = atan(1.0) / n;

  w[0]     = 1;
  w[1]     = 0;
  w[n]     = cos(delta * n);
  w[n + 1] = w[n];

  printf("delta is: %lg\n", delta);
#pragma mta assert no dependence
  for (i = 2; i < n; i += 2) {
      double x = cos(delta * i);
      double y = sin(delta * i);
      printf("i is: %d\n", i);
      printf("x is: %lg, y is: %lg\n", x, y);
      w[i]             = x;
      w[i + 1]         = y;
      w[2 * n - i]     = y;
      w[2 * n - i + 1] = x;
} 
  printf("w is:");
  for (i=0;i<n*2; i++) {
    printf(" %lg", w[i]);
  }
  printf("\n");
}

int main() {
  int N = 32;
  double x[N/2];
  twiddles(N/4, x);
}
