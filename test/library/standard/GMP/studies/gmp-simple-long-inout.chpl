use GMP, SysCTypes;

var depth = 4: c_long;

var stackdom = {0..#depth};

var x: [stackdom] mpz_t;

proc main() {
  mpz_init(x[0]);
  mpz_set_ui(x[0], 123);

  gmp_printf("x[0] is: %Zd\n", c_ptrTo(x[0]));

  mpz_clear(x[0]);
}
