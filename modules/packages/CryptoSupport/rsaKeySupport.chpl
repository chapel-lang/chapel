module rsaKeySupport {
  require "CryptoSupport/handlers/rsa_complex_bypass_handler.h";
  require "openssl/evp.h";
  require "CryptoSupport/CryptoUtils.chpl";
  require "CryptoSupport/primitives/asymmetricPrimitives.chpl";

  use CryptoUtils;
  use asymmetricPrimitives;

  pragma "no doc"
  proc generateKeys(bits: int) {
   var localKeyPair: asymmetricPrimitives.EVP_PKEY_PTR;
   var keyCtx = asymmetricPrimitives.EVP_PKEY_CTX_new_id(asymmetricPrimitives.EVP_PKEY_RSA: c_int,
                                                         c_nil: asymmetricPrimitives.ENGINE_PTR);
   asymmetricPrimitives.EVP_PKEY_keygen_init(keyCtx);
   asymmetricPrimitives.EVP_PKEY_CTX_set_rsa_keygen_bits(keyCtx, bits: c_int);
   asymmetricPrimitives.EVP_PKEY_keygen(keyCtx, localKeyPair);

   asymmetricPrimitives.EVP_PKEY_CTX_free(keyCtx);
   return localKeyPair;
  }
}
