module aesSupport {
  require "openssl/evp.h";
  require "CryptoSupport/CryptoUtils.chpl";
  require "CryptoSupport/primitives/symmetricPrimitives.chpl";

  use CryptoUtils;
  use symmetricPrimitives;

  pragma "no doc"
  proc aesEncrypt(plaintext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer, const cipher: symmetricPrimitives.EVP_CIPHER_PTR) {

    /* Initialize the context */
    var ctx: symmetricPrimitives.EVP_CIPHER_CTX;
    symmetricPrimitives.EVP_CIPHER_CTX_init(ctx);

    /* Get buffer contents */
    var keyData = key.getBuffData();
    var ivData = IV.getBuffData();
    var plaintextData = plaintext.getBuffData();
    var plaintextLen = plaintext.getBuffSize();

    /* Allocating space for obtaining the ciphertext */
    var ciphertextLen = plaintextLen + 16; // 16 is the MAX_BLOCK_SIZE for AES
    var cipherDomain: domain(1) = {0..ciphertextLen};
    var updatedCipherLen: c_int = 0;
    var ciphertext: [cipherDomain] uint(8);

    symmetricPrimitives.EVP_EncryptInit_ex(ctx,
                       cipher,
                       c_nil: symmetricPrimitives.ENGINE_PTR,
                       c_ptrTo(keyData): c_ptr(c_uchar),
                       c_ptrTo(ivData): c_ptr(c_uchar));
    symmetricPrimitives.EVP_EncryptUpdate(ctx,
                      c_ptrTo(ciphertext): c_ptr(c_uchar),
                      c_ptrTo(ciphertextLen): c_ptr(c_int),
                      c_ptrTo(plaintextData): c_ptr(c_uchar),
                      plaintextLen: c_int);
    symmetricPrimitives.EVP_EncryptFinal_ex(ctx,
                        c_ptrTo(ciphertext): c_ptr(c_uchar),
                        c_ptrTo(updatedCipherLen): c_ptr(c_int));

    cipherDomain = {0..((ciphertextLen + updatedCipherLen) - 1)};
    return ciphertext;
  }

  pragma "no doc"
  proc aesDecrypt(ciphertext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer, cipher: symmetricPrimitives.EVP_CIPHER_PTR) {

    /* Initialize the context */
    var ctx: symmetricPrimitives.EVP_CIPHER_CTX;
    symmetricPrimitives.EVP_CIPHER_CTX_init(ctx);

    /* Get buffer contents */
    var keyData = key.getBuffData();
    var ivData = IV.getBuffData();
    var ciphertextData = ciphertext.getBuffData();
    var ciphertextLen = ciphertext.getBuffSize();

    /* Allocating space for obtaining the plaintext */
    var plaintextLen = ciphertextLen;
    var updatedPlainLen: c_int = 0;
    var plainDomain: domain(1) = {0..plaintextLen};
    var plaintext: [plainDomain] uint(8);

    symmetricPrimitives.EVP_DecryptInit_ex(ctx,
                       cipher,
                       c_nil: symmetricPrimitives.ENGINE_PTR,
                       c_ptrTo(keyData): c_ptr(c_uchar),
                       c_ptrTo(ivData): c_ptr(c_uchar));
    symmetricPrimitives.EVP_DecryptUpdate(ctx,
                      c_ptrTo(plaintext): c_ptr(c_uchar),
                      c_ptrTo(plaintextLen): c_ptr(c_int),
                      c_ptrTo(ciphertextData): c_ptr(c_uchar),
                      ciphertextLen: c_int);
    symmetricPrimitives.EVP_DecryptFinal_ex(ctx,
                        c_ptrTo(plaintext): c_ptr(c_uchar),
                        c_ptrTo(updatedPlainLen): c_ptr(c_int));

   plainDomain = {0..((plaintextLen + updatedPlainLen) - 1)};
   return plaintext;
  }

}
