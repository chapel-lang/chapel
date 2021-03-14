static inline void cprintarr(double* arr, int size) {
  for (int i=0; i<size; i++) {
    printf("%lf ", arr[i]);
  }
  printf("\n");
}
