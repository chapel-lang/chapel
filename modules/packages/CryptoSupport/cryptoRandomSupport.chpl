module cryptoRandomSupport {
  require "openssl/rand.h";
  require "CryptoSupport/CryptoUtils.chpl";
  require "CryptoSupport/primitives/utilPrimitives.chpl";

  use utilPrimitives;

  pragma "no doc"
  proc createRandomBuffer(buffLen: int) {
    var buff: [0..(buffLen - 1)] uint(8);
    var retErrCode: c_int;
    retErrCode = utilPrimitives.RAND_bytes(c_ptrTo(buff): c_ptr(c_uchar), buffLen: c_int);
    if (!retErrCode) {
      halt("The random buffer generator has failed to initialize a buffer.");
    }
    return buff;
  }

}
