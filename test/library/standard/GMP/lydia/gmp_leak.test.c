#include <stdio.h>
#include <gmp.h>

int main(int argc, char *argv[]) {
  // This test demonstrates a leak with our build of GMP
  mpz_t a;
  mpz_init_set_si(a, 14);
  mpz_fac_ui(a, 140000);
  int size = mpz_sizeinbase(a, 10);
  printf("%d\n", size); // 659660
  return 0;
}
