module kdfSupport {
  require "openssl/evp.h";
  require "CryptoSupport/CryptoUtils.chpl";
  require "CryptoSupport/primitives/utilPrimitives.chpl";

  use CryptoUtils;
  use utilPrimitives;

  pragma "no doc"
  proc PBKDF2_HMAC(userKey: string, saltBuff: CryptoBuffer, bitLen: int, iterCount: int, digestName: string) {

    /* Loads all digests into the table*/
    utilPrimitives.OpenSSL_add_all_digests();

    var key: [0..(bitLen-1)] uint(8);
    var salt = saltBuff.getBuffData();
    var userKeyLen = userKey.length;

    /* Use the specified digest */
    const md = utilPrimitives.EVP_get_digestbyname(digestName.c_str());

    utilPrimitives.PKCS5_PBKDF2_HMAC(userKey.c_str(),
                      userKeyLen: c_int,
                      c_ptrTo(salt): c_ptr(c_uchar),
                      bitLen: c_int,
                      iterCount: c_int,
                      md,
                      bitLen: c_int,
                      c_ptrTo(key): c_ptr(c_uchar));

    return key;
  }

}
