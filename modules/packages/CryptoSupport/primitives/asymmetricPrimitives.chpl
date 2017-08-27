module asymmetricPrimitives {

  extern type ENGINE;
  extern type EVP_PKEY_CTX;
  extern type EVP_CIPHER_CTX;
  extern type EVP_PKEY;
  extern type EVP_CIPHER;
  extern var EVP_PKEY_RSA: c_int;

  extern type ENGINE_PTR = c_ptr(ENGINE);
  extern type EVP_PKEY_CTX_PTR = c_ptr(EVP_PKEY_CTX);
  extern type EVP_CIPHER_CTX_PTR = c_ptr(EVP_CIPHER_CTX);
  extern type EVP_PKEY_PTR = c_ptr(EVP_PKEY);
  extern type EVP_CIPHER_PTR = c_ptr(EVP_CIPHER);

  extern proc EVP_aes_256_cbc(): EVP_CIPHER_PTR;

  extern proc EVP_CIPHER_CTX_init(ref c: EVP_CIPHER_CTX): c_int;
  extern proc EVP_CIPHER_iv_length(const e: EVP_CIPHER_PTR): c_int;
  extern proc EVP_PKEY_size(pkey: EVP_PKEY_PTR): c_int;
  extern proc EVP_PKEY_CTX_new_id(id: c_int, e: ENGINE_PTR): EVP_PKEY_CTX_PTR;
  extern proc EVP_PKEY_keygen_init(ctx: EVP_PKEY_CTX_PTR): c_int;
  extern proc EVP_PKEY_CTX_set_rsa_keygen_bits(ctx: EVP_PKEY_CTX_PTR, mbits: c_int): c_int;
  extern proc EVP_PKEY_keygen(ctx: EVP_PKEY_CTX_PTR, ref ppkey: EVP_PKEY_PTR): c_int;
  extern proc EVP_PKEY_CTX_free(ctx: EVP_PKEY_CTX_PTR);

  extern proc EVP_SealInit(ref ctx: EVP_CIPHER_CTX, const types: EVP_CIPHER_PTR,
                           ek: c_ptr(c_ptr(c_uchar)), ekl: c_ptr(c_int),
                           iv: c_ptr(c_uchar), pubk: c_ptr(EVP_PKEY_PTR), npubk: c_int): c_int;
  extern proc EVP_SealUpdate(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar),
                             outl: c_ptr(c_int), inp: c_ptr(c_uchar), inl: c_int): c_int;
  extern proc EVP_SealFinal(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar), outl: c_ptr(c_int)): c_int;

  extern proc EVP_OpenInit(ref ctx: EVP_CIPHER_CTX, types: EVP_CIPHER_PTR,
                           ek: c_ptr(c_uchar), ekl: c_int, iv: c_ptr(c_uchar), priv: EVP_PKEY_PTR): c_int;
  extern proc EVP_OpenUpdate(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar),
                             outl: c_ptr(c_int), inp: c_ptr(c_uchar), inl: c_int): c_int;
  extern proc EVP_OpenFinal(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar), outl: c_ptr(c_int)): c_int;
}
