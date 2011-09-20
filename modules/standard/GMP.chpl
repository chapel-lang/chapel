type  c_int =  int(32),
     c_uint = uint(32),
       long =  int(64),
      c_ulong = uint(64);

extern type __mpf_struct;
extern type mpf_t = 1*__mpf_struct;
extern type __mpz_struct;
extern type mpz_t = 1*__mpz_struct;

extern proc mpz_init(inout X: mpz_t);
extern proc mpz_set_ui(inout ROP: mpz_t, OP: c_ulong);
extern proc mpz_init_set_ui(inout ROP: mpz_t, OP: c_ulong);
extern proc mpz_mul_ui(inout ROP: mpz_t, inout OP1: mpz_t, OP2: c_ulong);
extern proc mpz_submul_ui(inout ROP: mpz_t, inout OP1: mpz_t, OP2: c_ulong);
extern proc mpz_add_ui(inout ROP: mpz_t, inout OP1: mpz_t, OP2: c_ulong);
extern proc mpz_mul(inout ROP: mpz_t, inout OP1: mpz_t, inout OP2: mpz_t);
extern proc mpz_mul_2exp(inout ROP: mpz_t, inout OP1: mpz_t, OP2: c_ulong);
extern proc mpz_neg(inout ROP: mpz_t, inout OP: mpz_t);
extern proc mpz_clear(inout X: mpz_t);
extern proc mpz_tdiv_q(inout Q: mpz_t, inout N: mpz_t, inout D: mpz_t);
extern proc mpz_fdiv_qr(inout Q: mpz_t, inout R: mpz_t, inout N: mpz_t, inout D: mpz_t);
extern proc mpz_add(inout ROP: mpz_t, inout OP1: mpz_t, inout OP2: mpz_t);
extern proc mpz_cmp(inout OP1: mpz_t, inout OP2: mpz_t) : int;
extern proc mpz_get_ui(OP: mpz_t): uint(64);

extern type mp_bitcnt_t = c_ulong;
extern type size_t = int(64);

extern proc mpf_set_default_prec(PREC: mp_bitcnt_t);
extern proc mpz_sizeinbase(inout OP: mpz_t, BASE: c_int): size_t;

extern proc mpz_addmul_ui(inout ROP: mpz_t, inout OP1: mpz_t, OPT2: c_ulong);

extern type double = real(64);

extern proc mpf_init(inout X: mpf_t);
extern proc mpf_set_z(inout ROP: mpf_t, inout OP: mpz_t);
extern proc mpf_get_prec(inout OP: mpf_t): mp_bitcnt_t;
extern proc mpf_get_d(inout OP: mpf_t): double;
extern proc mpf_set_d(inout ROP: mpf_t, OP: double);
extern proc mpf_set_prec_raw(inout ROP: mpf_t, PREC: mp_bitcnt_t);
extern proc mpf_ui_div(inout ROP: mpf_t, OP1: c_ulong, inout OP2: mpf_t);
extern proc mpf_mul(inout ROP: mpf_t, inout OP1: mpf_t, inout OP2: mpf_t);
extern proc mpf_ui_sub(inout ROP: mpf_t, OP1: c_ulong, inout OP2: mpf_t);
extern proc mpf_add(inout ROP: mpf_t, inout OP1: mpf_t, inout OP2: mpf_t);
extern proc mpf_sub(inout ROP: mpf_t, inout OP1: mpf_t, inout OP2: mpf_t);
extern proc mpf_mul_ui(inout ROP: mpf_t, inout OP1: mpf_t, OP2: c_ulong);
extern proc mpf_div_2exp(inout ROP: mpf_t, inout OP1: mpf_t, OP2: mp_bitcnt_t);
extern proc mpf_out_str(STREAM: _file, BASE: c_int, N_DIGITS: size_t, inout OP: mpf_t);
extern proc mpf_clear(inout X: mpf_t);

extern proc gmp_printf(fmt: string, arg...);
extern proc gmp_fprintf(fp: _file, fmt: string, arg...);
