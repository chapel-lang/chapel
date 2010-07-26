type  c_int =  int(32),
     c_uint = uint(32),
       long =  int(64),
      c_ulong = uint(64);

_extern type __mpf_struct;
_extern type mpf_t = 1*__mpf_struct;
_extern type __mpz_struct;
_extern type mpz_t = 1*__mpz_struct;

_extern def mpz_init(inout X: mpz_t);
_extern def mpz_set_ui(inout ROP: mpz_t, OP: c_ulong);
_extern def mpz_mul_ui(inout ROP: mpz_t, inout OP1: mpz_t, OP2: c_ulong);
_extern def mpz_add_ui(inout ROP: mpz_t, inout OP1: mpz_t, OP2: c_ulong);
_extern def mpz_mul(inout ROP: mpz_t, inout OP1: mpz_t, inout OP2: mpz_t);
_extern def mpz_neg(inout ROP: mpz_t, inout OP: mpz_t);
_extern def mpz_clear(inout X: mpz_t);
_extern def mpz_tdiv_q(inout Q: mpz_t, inout N: mpz_t, inout D: mpz_t);
_extern def mpz_add(inout ROP: mpz_t, inout OP1: mpz_t, inout OP2: mpz_t);

_extern type mp_bitcnt_t = c_ulong;
_extern type size_t = int(64);

_extern def mpf_set_default_prec(PREC: mp_bitcnt_t);
_extern def mpz_sizeinbase(inout OP: mpz_t, BASE: c_int): size_t;

_extern def mpz_addmul_ui(inout ROP: mpz_t, inout OP1: mpz_t, OPT2: c_ulong);

_extern type double = real(64);

_extern def mpf_init(inout X: mpf_t);
_extern def mpf_set_z(inout ROP: mpf_t, inout OP: mpz_t);
_extern def mpf_get_prec(inout OP: mpf_t): mp_bitcnt_t;
_extern def mpf_get_d(inout OP: mpf_t): double;
_extern def mpf_set_d(inout ROP: mpf_t, OP: double);
_extern def mpf_set_prec_raw(inout ROP: mpf_t, PREC: mp_bitcnt_t);
_extern def mpf_ui_div(inout ROP: mpf_t, OP1: c_ulong, inout OP2: mpf_t);
_extern def mpf_mul(inout ROP: mpf_t, inout OP1: mpf_t, inout OP2: mpf_t);
_extern def mpf_ui_sub(inout ROP: mpf_t, OP1: c_ulong, inout OP2: mpf_t);
_extern def mpf_add(inout ROP: mpf_t, inout OP1: mpf_t, inout OP2: mpf_t);
_extern def mpf_sub(inout ROP: mpf_t, inout OP1: mpf_t, inout OP2: mpf_t);
_extern def mpf_mul_ui(inout ROP: mpf_t, inout OP1: mpf_t, OP2: c_ulong);
_extern def mpf_div_2exp(inout ROP: mpf_t, inout OP1: mpf_t, OP2: mp_bitcnt_t);
_extern def mpf_out_str(STREAM: _file, BASE: c_int, N_DIGITS: size_t, inout OP: mpf_t);
_extern def mpf_clear(inout X: mpf_t);

_extern def gmp_printf(fmt: string, arg...);
_extern def gmp_fprintf(fp: _file, fmt: string, arg...);
