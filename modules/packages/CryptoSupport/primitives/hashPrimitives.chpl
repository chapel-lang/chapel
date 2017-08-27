module hashPrimitives {
  extern type EVP_MD;
  extern type EVP_MD_CTX;
  extern type ENGINE;

  extern type EVP_MD_PTR = c_ptr(EVP_MD);
  extern type EVP_MD_CTX_PTR = c_ptr(EVP_MD_CTX);
  extern type ENGINE_PTR = c_ptr(ENGINE);

  extern proc OpenSSL_add_all_digests();
  extern proc EVP_get_digestbyname(name: c_string): EVP_MD_PTR;
  extern proc EVP_MD_CTX_init(ref ctx: EVP_MD_CTX): c_int;
  extern proc EVP_DigestInit_ex(ref ctx: EVP_MD_CTX, const types: EVP_MD_PTR, impl: ENGINE_PTR): c_int;
  extern proc EVP_DigestUpdate(ref ctx: EVP_MD_CTX, const d: c_void_ptr, cnt: size_t): c_int;
  extern proc EVP_DigestFinal_ex(ref ctx: EVP_MD_CTX, md: c_ptr(c_uchar), ref s: c_uint): c_int;
}
