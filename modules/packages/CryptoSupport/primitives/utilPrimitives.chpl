module utilPrimitives {
  extern type EVP_MD;
  extern type EVP_MD_PTR = c_ptr(EVP_MD);
  extern proc OpenSSL_add_all_digests();
  extern proc EVP_get_digestbyname(name: c_string): EVP_MD_PTR;
  extern proc PKCS5_PBKDF2_HMAC(pass: c_string,
                                passlen: c_int,
                                const salt: c_ptr(c_uchar),
                                saltlen: c_int,
                                iterCount: c_int,
                                const digest: EVP_MD_PTR,
                                keylen: c_int,
                                outx: c_ptr(c_uchar)): c_int;
  extern proc RAND_bytes(buf: c_ptr(c_uchar), num: c_int): c_int;
  extern proc RAND_seed(const buf: c_void_ptr, num: c_int);
}
