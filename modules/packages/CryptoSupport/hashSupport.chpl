module hashSupport {
  require "openssl/evp.h";
  require "CryptoSupport/CryptoUtils.chpl";
  require "CryptoSupport/primitives/hashPrimitives.chpl";

  use CryptoUtils;
  use hashPrimitives;

  pragma "no doc"
  proc digestPrimitives(digestName: string, hashLen: int, inputBuffer: CryptoBuffer) {

    /* Loads the digest primitives into the table  */
    hashPrimitives.OpenSSL_add_all_digests();

    /* Create a context variable */
    var ctx: hashPrimitives.EVP_MD_CTX;

    /* Allocate space for hashed output */
    var hash: [0..hashLen-1] uint(8); ;
    var retHashLen: c_uint = 0;

    /* Get pointer to the desired digest structure */
    const md = hashPrimitives.EVP_get_digestbyname(digestName.c_str());

    /* OpenSSL primitive calls */
    hashPrimitives.EVP_MD_CTX_init(ctx);
    hashPrimitives.EVP_DigestInit_ex(ctx, md, c_nil: hashPrimitives.ENGINE_PTR);
    hashPrimitives.EVP_DigestUpdate(ctx, c_ptrTo(inputBuffer.buff): c_void_ptr, inputBuffer._len: size_t);
    hashPrimitives.EVP_DigestFinal_ex(ctx, c_ptrTo(hash): c_ptr(c_uchar), retHashLen);

    return hash;
  }
}
