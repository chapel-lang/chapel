module symmetricPrimitives  {
  extern type EVP_CIPHER;
  extern type EVP_CIPHER_CTX;
  extern type EVP_MD;
  extern type ENGINE;

  extern type EVP_CIPHER_PTR = c_ptr(EVP_CIPHER);
  extern type EVP_CIPHER_CTX_PTR = c_ptr(EVP_CIPHER_CTX);
  extern type EVP_MD_PTR = c_ptr(EVP_MD);
  extern type ENGINE_PTR = c_ptr(ENGINE);

  extern proc RAND_bytes(buf: c_ptr(c_uchar), num: c_int) : c_int;

  extern proc EVP_sha256(): EVP_MD_PTR;

  extern proc EVP_CIPHER_CTX_free(ref c: EVP_CIPHER_CTX);
  extern proc EVP_CIPHER_CTX_init(ref c: EVP_CIPHER_CTX): c_int;
  extern proc EVP_EncryptInit_ex(ref ctx: EVP_CIPHER_CTX,
                                const cipher: EVP_CIPHER_PTR,
                                impl: ENGINE_PTR,
                                const key: c_ptr(c_uchar),
                                const iv: c_ptr(c_uchar)): c_int;
  extern proc EVP_EncryptUpdate(ref ctx: EVP_CIPHER_CTX,
                                outm: c_ptr(c_uchar),
                                outl: c_ptr(c_int),
                                const ins: c_ptr(c_uchar),
                                inl: c_int): c_int;
  extern proc EVP_EncryptFinal_ex(ref ctx: EVP_CIPHER_CTX,
                                  outm: c_ptr(c_uchar),
                                  outl: c_ptr(c_int)): c_int;
  extern proc EVP_DecryptInit_ex(ref ctx: EVP_CIPHER_CTX,
                                const cipher: EVP_CIPHER_PTR,
                                impl: ENGINE_PTR,
                                const key: c_ptr(c_uchar),
                                const iv: c_ptr(c_uchar)): c_int;
  extern proc EVP_DecryptUpdate(ref ctx: EVP_CIPHER_CTX,
                                outm: c_ptr(c_uchar),
                                outl: c_ptr(c_int),
                                const ins: c_ptr(c_uchar),
                                inl: c_int): c_int;
  extern proc EVP_DecryptFinal_ex(ref ctx: EVP_CIPHER_CTX,
                                  outm: c_ptr(c_uchar),
                                  outl: c_ptr(c_int)): c_int;

  extern proc EVP_aes_128_cbc(): EVP_CIPHER_PTR;
  extern proc EVP_aes_128_ecb(): EVP_CIPHER_PTR;
  extern proc EVP_aes_128_cfb(): EVP_CIPHER_PTR;
  extern proc EVP_aes_128_ofb(): EVP_CIPHER_PTR;
  extern proc EVP_aes_192_cbc(): EVP_CIPHER_PTR;
  extern proc EVP_aes_192_ecb(): EVP_CIPHER_PTR;
  extern proc EVP_aes_192_cfb(): EVP_CIPHER_PTR;
  extern proc EVP_aes_192_ofb(): EVP_CIPHER_PTR;
  extern proc EVP_aes_256_cbc(): EVP_CIPHER_PTR;
  extern proc EVP_aes_256_ecb(): EVP_CIPHER_PTR;
  extern proc EVP_aes_256_cfb(): EVP_CIPHER_PTR;
  extern proc EVP_aes_256_ofb(): EVP_CIPHER_PTR;
}
