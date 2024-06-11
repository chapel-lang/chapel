use CTypes, GMP, IO;

var x:mpf_t;
mpf_init(x);

var y:mpz_t;
mpz_init(y);

mpf_set_d(x, 1.234567);
mpz_set_ui(y, 7654321);

gmp_printf("%.4Ff %Zd\n", x, y);  // Print a formatted string with 4 decimal
                                  // places followed by a bigint value

var ptr: c_ptr(c_uchar);
gmp_asprintf(ptr, "%.4Ff %Zd\n", x, y);
extern proc printf(x...);
printf("%s", ptr);
deallocate(ptr);

gmp_fprintf(chpl_cstdout(), "%.4Ff %Zd\n", x, y);
extern proc fflush(s: c_ptr(c_FILE)): c_int;
fflush(chpl_cstdout());

mpf_clear(x);
mpz_clear(y);