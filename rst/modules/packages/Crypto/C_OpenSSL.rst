.. default-domain:: chpl

.. module:: C_OpenSSL
   :synopsis: Support for low-level native C_OpenSSL bindings.

C_OpenSSL
=========
**Usage**

.. code-block:: chapel

   use Crypto.C_OpenSSL;


Support for low-level native C_OpenSSL bindings.
This submodule wraps the C_OpenSSL implementation, providing access to
most of the C_OpenSSL calls.

Refer to the `C_OpenSSL documentation <https://www.openssl.org/docs/manmaster/man3/>`_
of the reference version for the usage of this module.



.. type:: type EVP_PKEY_CTX

.. type:: type EVP_PKEY

.. data:: var EVP_PKEY_RSA: c_int

.. type:: type EVP_PKEY_CTX_PTR = c_ptr(EVP_PKEY_CTX)

.. type:: type EVP_PKEY_PTR = c_ptr(EVP_PKEY)

.. type:: type CONST_EVP_MD_PTR

.. type:: type CONST_EVP_CIPHER_PTR

.. function:: proc EVP_CIPHER_iv_length(e: CONST_EVP_CIPHER_PTR): c_int

.. function:: proc EVP_PKEY_size(pkey: EVP_PKEY_PTR): c_int

.. function:: proc EVP_PKEY_CTX_new_id(id: c_int, e: ENGINE_PTR): EVP_PKEY_CTX_PTR

.. function:: proc EVP_PKEY_keygen_init(ctx: EVP_PKEY_CTX_PTR): c_int

.. function:: proc EVP_PKEY_CTX_set_rsa_keygen_bits(ctx: EVP_PKEY_CTX_PTR, mbits: c_int): c_int

.. function:: proc EVP_PKEY_keygen(ctx: EVP_PKEY_CTX_PTR, ref ppkey: EVP_PKEY_PTR): c_int

.. function:: proc EVP_PKEY_CTX_free(ctx: EVP_PKEY_CTX_PTR)

.. function:: proc EVP_SealInit(ref ctx: EVP_CIPHER_CTX, types: CONST_EVP_CIPHER_PTR, ek: c_ptr(c_ptr(c_uchar)), ekl: c_ptr(c_int), iv: c_ptr(c_uchar), pubk: c_ptr(EVP_PKEY_PTR), npubk: c_int): c_int

.. function:: proc EVP_SealUpdate(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar), outl: c_ptr(c_int), inp: c_ptr(c_uchar), inl: c_int): c_int

.. function:: proc EVP_SealFinal(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar), outl: c_ptr(c_int)): c_int

.. function:: proc EVP_OpenInit(ref ctx: EVP_CIPHER_CTX, types: CONST_EVP_CIPHER_PTR, ek: c_ptr(c_uchar), ekl: c_int, iv: c_ptr(c_uchar), priv: EVP_PKEY_PTR): c_int

.. function:: proc EVP_OpenUpdate(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar), outl: c_ptr(c_int), inp: c_ptr(c_uchar), inl: c_int): c_int

.. function:: proc EVP_OpenFinal(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar), outl: c_ptr(c_int)): c_int

.. type:: type EVP_MD

.. type:: type EVP_MD_CTX

.. type:: type CHPL_EVP_MD_CTX

.. type:: type ENGINE

.. type:: type EVP_MD_PTR = c_ptr(EVP_MD)

.. type:: type EVP_MD_CTX_PTR = c_ptr(EVP_MD_CTX)

.. type:: type ENGINE_PTR = c_ptr(ENGINE)

.. function:: proc OpenSSL_add_all_digests()

.. function:: proc EVP_get_digestbyname(name: c_string): CONST_EVP_MD_PTR

.. function:: proc CHPL_EVP_MD_CTX_new(): CHPL_EVP_MD_CTX

.. function:: proc CHPL_EVP_MD_CTX_free(ref c: CHPL_EVP_MD_CTX)

.. function:: proc CHPL_EVP_MD_CTX_ptr(ref c: CHPL_EVP_MD_CTX): EVP_MD_CTX_PTR

.. function:: proc EVP_DigestInit_ex(ctx: EVP_MD_CTX_PTR, types: CONST_EVP_MD_PTR, impl: ENGINE_PTR): c_int

.. function:: proc EVP_DigestUpdate(ctx: EVP_MD_CTX_PTR, const d: c_void_ptr, cnt: size_t): c_int

.. function:: proc EVP_DigestFinal_ex(ctx: EVP_MD_CTX_PTR, md: c_ptr(c_uchar), ref s: c_uint): c_int

.. type:: type EVP_CIPHER

.. type:: type EVP_CIPHER_CTX

.. type:: type CHPL_EVP_CIPHER_CTX

.. type:: type EVP_CIPHER_PTR = c_ptr(EVP_CIPHER)

.. type:: type EVP_CIPHER_CTX_PTR = c_ptr(EVP_CIPHER_CTX)

.. function:: proc RAND_bytes(buf: c_ptr(c_uchar), num: c_int): c_int

.. function:: proc EVP_sha256(): CONST_EVP_MD_PTR

.. function:: proc PKCS5_PBKDF2_HMAC(pass: c_string, passlen: c_int, const salt: c_ptr(c_uchar), saltlen: c_int, iterCount: c_int, digest: CONST_EVP_MD_PTR, keylen: c_int, outx: c_ptr(c_uchar)): c_int

.. function:: proc CHPL_EVP_CIPHER_CTX_new(): CHPL_EVP_CIPHER_CTX

.. function:: proc CHPL_EVP_CIPHER_CTX_free(ref c: CHPL_EVP_CIPHER_CTX)

.. function:: proc CHPL_EVP_CIPHER_CTX_ptr(ref c: CHPL_EVP_CIPHER_CTX): EVP_CIPHER_CTX_PTR

.. function:: proc EVP_EncryptInit_ex(ctx: EVP_CIPHER_CTX_PTR, cipher: CONST_EVP_CIPHER_PTR, impl: ENGINE_PTR, const key: c_ptr(c_uchar), const iv: c_ptr(c_uchar)): c_int

.. function:: proc EVP_EncryptUpdate(ctx: EVP_CIPHER_CTX_PTR, outm: c_ptr(c_uchar), outl: c_ptr(c_int), const ins: c_ptr(c_uchar), inl: c_int): c_int

.. function:: proc EVP_EncryptFinal_ex(ctx: EVP_CIPHER_CTX_PTR, outm: c_ptr(c_uchar), outl: c_ptr(c_int)): c_int

.. function:: proc EVP_DecryptInit_ex(ctx: EVP_CIPHER_CTX_PTR, cipher: CONST_EVP_CIPHER_PTR, impl: ENGINE_PTR, const key: c_ptr(c_uchar), const iv: c_ptr(c_uchar)): c_int

.. function:: proc EVP_DecryptUpdate(ctx: EVP_CIPHER_CTX_PTR, outm: c_ptr(c_uchar), outl: c_ptr(c_int), const ins: c_ptr(c_uchar), inl: c_int): c_int

.. function:: proc EVP_DecryptFinal_ex(ctx: EVP_CIPHER_CTX_PTR, outm: c_ptr(c_uchar), outl: c_ptr(c_int)): c_int

.. function:: proc EVP_aes_128_cbc(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_aes_128_ecb(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_aes_128_cfb(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_aes_128_ofb(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_aes_192_cbc(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_aes_192_ecb(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_aes_192_cfb(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_aes_192_ofb(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_aes_256_cbc(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_aes_256_ecb(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_aes_256_cfb(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_aes_256_ofb(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_bf_cbc(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_bf_ecb(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_bf_cfb(): CONST_EVP_CIPHER_PTR

.. function:: proc EVP_bf_ofb(): CONST_EVP_CIPHER_PTR

.. function:: proc RAND_seed(const buf: c_void_ptr, num: c_int)

