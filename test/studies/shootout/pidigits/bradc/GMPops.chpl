use GMP, CTypes;

inline operator mpz_t.+=(ref x: mpz_t, ref y: mpz_t) {
  mpz_add(x, x, y);
}

inline operator *=(ref x: mpz_t, y: c_ulong) {
  mpz_mul_ui(x, x, y);
}

inline operator mpz_t.>(ref x: mpz_t, ref y: mpz_t) {
  return (mpz_cmp(x, y) > 0);
}

inline operator mpz_t.>=(ref x: mpz_t, ref y: mpz_t) {
  return (mpz_cmp(x,y) >= 0);
}

//
// TODO: This is not ideal due to the reliance on a local temp --
// results in extra copies and a leak.
//
//
inline operator *(ref x: mpz_t, y: c_ulong) {
  var res: mpz_t;
  mpz_init(res);
  mpz_mul_ui(res, x, y);
  return res;
}

//
// TODO: Why doesn't this work?
//
/*
inline operator mpz_t.-(ref x: mpz_t, ref y: mpz_t) {
  var res: mpz_t;
  mpz_init(res);
  mpz_sub(res, x, y);
  return res;
}
*/
