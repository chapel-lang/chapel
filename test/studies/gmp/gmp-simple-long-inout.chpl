type  c_int =  int(32),
     c_uint = uint(32),
       long =  int(64),
      c_ulong = uint(64);

_extern type __mpz_struct;
_extern type mpz_t = 1*__mpz_struct;

_extern def mpz_init(inout X: mpz_t);
_extern def mpz_set_ui(inout ROP: mpz_t, OP: c_ulong);

_extern def gmp_printf(fmt: string, arg: mpz_t);

var depth = 4: long;

var stackdom = [0..#depth];

var x: [stackdom] mpz_t;

def main() {
  mpz_init(x[0]);
  mpz_set_ui(x[0], 123);

  gmp_printf("x[0] is: %Zd\n", x[0]);
}
