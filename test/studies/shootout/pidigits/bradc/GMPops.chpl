use GMP, SysCTypes;

inline proc +=(ref x: mpz_t, ref y: mpz_t) {
  mpz_add(x, x, y);
}

inline proc *=(ref x: mpz_t, y: c_ulong) {
  mpz_mul_ui(x, x, y);
}

inline proc >(ref x: mpz_t, ref y: mpz_t) {
  return (mpz_cmp(x, y) > 0);
}

inline proc >=(ref x: mpz_t, ref y: mpz_t) {
  return (mpz_cmp(x,y) >= 0);
}

//
// TODO: I feel very nervous that 'rhs' is not passed by reference;
// from past experience, I thought that all mpz_t's had to be passed
// by reference in order to make the GMP library work correctly.
// However, if I make it ref here, I get a complaint if I send in the
// result of another operation like the *() below.
//
inline proc =(ref lhs: mpz_t, rhs: mpz_t) {
  mpz_set(lhs, rhs);
}

//
// TODO: This is not ideal due to the reliance on a local temp --
// results in extra copies and a leak.
//
//
inline proc *(ref x: mpz_t, y: c_ulong) {
  var res: mpz_t;
  mpz_init(res);
  mpz_mul_ui(res, x, y);
  return res;
}

//
// TODO: Why doesn't this work?
//
/*
inline proc -(ref x: mpz_t, ref y: mpz_t) {
  var res: mpz_t;
  mpz_init(res);
  mpz_sub(res, x, y);
  return res;
}
*/
