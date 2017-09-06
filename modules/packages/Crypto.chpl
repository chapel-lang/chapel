/*
 * Copyright 2004-2017 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*  A cryptographic library based on OpenSSl, targeted at flexible encryption
    purposes.

    The Crypto module focuses on providing various cryptographic utilities such as

    - Symmetric Ciphers
    - Asymmetric Ciphers
    - Hashing Functions
    - Key Derivation Functions
    - Cryptographically-Secure Pseudo-Random Number Generators
    - Miscellaneous Utilities such as CryptoBuffer, Envelopes, etc.

    .. note::

      This module is currently under development and will expand significantly
      in upcoming releases.

*/

module Crypto {

  use C_OpenSSL;

  pragma "no doc"
  proc generateKeys(bits: int) {
   var localKeyPair: EVP_PKEY_PTR;
   var keyCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA: c_int,
                                    c_nil: ENGINE_PTR);
   EVP_PKEY_keygen_init(keyCtx);
   EVP_PKEY_CTX_set_rsa_keygen_bits(keyCtx, bits: c_int);
   EVP_PKEY_keygen(keyCtx, localKeyPair);

   EVP_PKEY_CTX_free(keyCtx);
   return localKeyPair;
  }

  class CryptoBuffer {
    pragma "no doc"
    var _len: int = 0;
    pragma "no doc"
    var buffDomain: domain(1);
    pragma "no doc"
    var buff: [buffDomain] uint(8);

    /* The `CryptoBuffer` class constructor that initializes the buffer
       when a `string` is supplied to it.

       :arg s: `string` input for buffer conversion.
       :type s: `string`

       :return: An object of class `CryptoBuffer`.
       :rtype: `CryptoBuffer`

    */
    proc init(s: string) {
      this._len = s.length;
      this.buffDomain = {1..this._len};
      for i in this.buffDomain do {
        this.buff[i] = ascii(s[i]);
      }
    }

    /* The `CryptoBuffer` class constructor that initializes the buffer
       when a `[] uint(8)` is supplied to it.

       :arg s: `[] uint(8)` input for buffer conversion.
       :type s: `[] uint(8)`

       :return: An object of class `CryptoBuffer`.
       :rtype: `CryptoBuffer`

    */
    proc init(s: [] uint(8)) {
      this._len = s.size;
      this.buffDomain = s.domain;
      for i in this.buffDomain do {
        this.buff[i] = s[i];
      }
    }

    /* Returns the entire internal buffer with each byte in ASCII.

       :return: the internal buffer representation.
       :rtype: `[] uint(8)`

    */
    proc getBuffData() {
      return this.buff;
    }

    /* Returns the pointer to the entire internal buffer with
       each byte in ASCII.

       :return: pointer to the internal buffer representation.
       :rtype: `c_ptr([] uint(8))`

    */
    proc getBuffPtr() {
      return c_ptrTo(this.buff);
    }

    /* Returns the length of the entire internal buffer.

       :return: length of the internal buffer representation.
       :rtype: `int`

    */
    proc getBuffSize(): int {
      return this._len;
    }

    /* Returns the hexadecimal array representation of the entire internal buffer.

       :return: hex array representation of the internal buffer.
       :rtype: `[] string`

    */
    proc toHex() {
      var buffHex: [this.buffDomain] string;
      for i in this.buffDomain do {
        buffHex[i] = "%xu".format(this.buff[i]);
      }
      return buffHex;
    }

    /* Returns the hexadecimal string representation of the entire internal buffer.

       :return: hex string representation of the internal buffer.
       :rtype: `string`

    */
    proc toHexString() {
      var buffHexString: string;
      for i in this.buffDomain do {
        buffHexString += "%xu".format(this.buff[i]);
      }
      return buffHexString;
    }
  }

  class RSAKey {
    pragma "no doc"
    var keyLen: int;
    pragma "no doc"
    var keyObj: EVP_PKEY_PTR;

    /* The `RSAKey` class constructor that initializes the `EVP_PKEY` object
       of OpenSSL and basically, initializes a set of public and private keys.

       It checks for valid RSA key lengths and generates a public key and private
       key pair accordingly.

       :arg keyLen: RSA Key length in bits.
       :type keyLen: `int`

       :return: An object of class `RSAKey` representing the key pair.
       :rtype: `RSAKey`

    */
    proc init(keyLen: int) {
      if (keyLen != 1024 && keyLen != 2048 && keyLen != 4096) {
        halt("RSAKey: Invalid key length.");
      }
      this.keyLen = keyLen;
      this.keyObj = generateKeys(this.keyLen);
    }

    pragma "no doc"
    proc getKeyPair() {
      return this.keyObj;
    }

    // TODO: Key access functions to be added
  }

  class Envelope {
    pragma "no doc"
    var keyDomain: domain(1);
    pragma "no doc"
    var keys: [keyDomain] CryptoBuffer;
    pragma "no doc"
    var iv: CryptoBuffer;
    pragma "no doc"
    var value: CryptoBuffer;

    /* The `Envelope` class constructor that encapsulates the IV, AES encrypted
       ciphertext buffer and an array of encrypted key buffers.

       :arg iv: Initialization Vector.
       :type iv: `CryptoBuffer`

       :arg encSymmKey: Array of encrypted symmetric (AES) keys.
       :type encSymmKey: `[] CryptoBuffer`

       :arg encSymmValue: AES-encrypted ciphertext buffer.
       :type encSymmValue: `CryptoBuffer`

       :return: An object of class `Envelope`.
       :rtype: `Envelope`

    */
    proc init(iv: CryptoBuffer, encSymmKey: [] CryptoBuffer, encSymmValue: CryptoBuffer) {
      this.keyDomain = encSymmKey.domain;
      for i in this.keyDomain do {
        this.keys[i] = encSymmKey[i];
      }
      this.iv = iv;
      this.value = encSymmValue;
    }

    /* This function returns the encrypted version of the plaintext
       supplied by the user.

       :return: A 'CryptoBuffer' representing the ciphertext.
       :rtype: `CryptoBuffer`

    */
    proc getEncMessage() {
      return this.value;
    }

    /* This function returns the IV generated by the `encrypt` routine and
       encapsulated in the `Envelope`. This is used for both encryption and
       dencryption.

       :return: Initialization Vector.
       :rtype: `CryptoBuffer`

    */
    proc getIV() {
      return this.iv;
    }

    /* This function returns a particular symmetric key buffer based
       on the index supplied as the argument.

       .. note::

         The supplied index should be in the domain of the RSA-encrypted keys array.

       :arg i: An index of the symmetric key buffer array.
       :type i: `int`

       :return: A specific key buffer based on the index.
       :rtype: `CryptoBuffer`

    */
    proc getEncKeyByIndex(i: int) {
      return this.keys[i];
    }

    /* This function returns the entire array of symmetric key buffers.

       :return: Entire array of key buffers.
       :rtype: `[] CryptoBuffer`

    */
    proc getEncKeys() {
      return this.keys;
    }
  }

  pragma "no doc"
  proc digestPrimitives(digestName: string, hashLen: int, inputBuffer: CryptoBuffer) {

    OpenSSL_add_all_digests();

    var ctx: EVP_MD_CTX;

    var hash: [0..#hashLen] uint(8); ;
    var retHashLen: c_uint = 0;

    const md = EVP_get_digestbyname(digestName.c_str());

    EVP_MD_CTX_init(ctx);
    EVP_DigestInit_ex(ctx, md, c_nil: ENGINE_PTR);
    EVP_DigestUpdate(ctx, c_ptrTo(inputBuffer.buff): c_void_ptr, inputBuffer._len: size_t);
    EVP_DigestFinal_ex(ctx, c_ptrTo(hash): c_ptr(c_uchar), retHashLen);

    return hash;
  }

  class Hash {
    pragma "no doc"
    var hashLen: int;
    pragma "no doc"
    var digestName: string;
    pragma "no doc"
    var hashDomain: domain(1);
    pragma "no doc"
    var hashSpace: [hashDomain] uint(8);

    /* The `Hash` class constructor that initializes the hashing function
       to be used. This constructor sets the byte length of the
       respective hash and allocates a domain for memory allocation
       for hashing.

       :arg digestName: Name of the hashing function to be used.
       :type digestName: `string`

       :return: An object of class `Hash`.
       :rtype: `Hash`

    */
    proc init(digestName: string) {
      select digestName {
        when "MD5"        do this.hashLen = 16;
        when "SHA1"       do this.hashLen = 20;
        when "SHA224"     do this.hashLen = 28;
        when "SHA256"     do this.hashLen = 32;
        when "SHA384"     do this.hashLen = 48;
        when "SHA512"     do this.hashLen = 64;
        when "RIPEMD160"  do this.hashLen = 20;
        otherwise do halt("A digest with the name \'" + digestName + "\' doesn't exist.");
      }
      this.digestName = digestName;
      this.hashDomain = {0..#this.hashLen};
    }

    /* Returns the name of the hashing function used. For instance,
      instantiating an object with `var hash = new Hash("SHA256")`,
      would make this function return the string `SHA256`.

      :return: Name of the hashing function.
      :rtype: `string`

    */
    proc getDigestName(): string {
      return this.digestName;
    }

    /* Given a CryptoBuffer (buffer) as input, this function returns it's
       hash digest. The returned hash digest is also a buffer that can be
       accessed by using buffer utility functions.

       :arg inputBuffer: A `CryptoBuffer` representing the input to be hashed.
       :type inputBuffer: `CryptoBuffer`

       :return: A `CryptoBuffer` representing the hash digest.
       :rtype: `CryptoBuffer`

    */
    proc getDigest(inputBuffer: CryptoBuffer): CryptoBuffer {
      this.hashSpace = digestPrimitives(this.digestName, this.hashLen, inputBuffer);
      var hashBuffer = new CryptoBuffer(this.hashSpace);
      return hashBuffer;
    }
  }

  pragma "no doc"
  proc aesEncrypt(plaintext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer, const cipher: EVP_CIPHER_PTR) {

    var ctx: EVP_CIPHER_CTX;
    EVP_CIPHER_CTX_init(ctx);

    var keyData = key.getBuffData();
    var ivData = IV.getBuffData();
    var plaintextData = plaintext.getBuffData();
    var plaintextLen = plaintext.getBuffSize();

    var ciphertextLen = plaintextLen + 16; // 16 is the MAX_BLOCK_SIZE for AES
    var cipherDomain: domain(1) = {0..ciphertextLen};
    var updatedCipherLen: c_int = 0;
    var ciphertext: [cipherDomain] uint(8);

    EVP_EncryptInit_ex(ctx,
                       cipher,
                       c_nil: ENGINE_PTR,
                       c_ptrTo(keyData): c_ptr(c_uchar),
                       c_ptrTo(ivData): c_ptr(c_uchar));
    EVP_EncryptUpdate(ctx,
                      c_ptrTo(ciphertext): c_ptr(c_uchar),
                      c_ptrTo(ciphertextLen): c_ptr(c_int),
                      c_ptrTo(plaintextData): c_ptr(c_uchar),
                      plaintextLen: c_int);
    EVP_EncryptFinal_ex(ctx,
                        c_ptrTo(ciphertext): c_ptr(c_uchar),
                        c_ptrTo(updatedCipherLen): c_ptr(c_int));

    cipherDomain = {0..#(ciphertextLen + updatedCipherLen)};
    return ciphertext;
  }

  pragma "no doc"
  proc aesDecrypt(ciphertext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer, cipher: EVP_CIPHER_PTR) {

    var ctx: EVP_CIPHER_CTX;
    EVP_CIPHER_CTX_init(ctx);

    var keyData = key.getBuffData();
    var ivData = IV.getBuffData();
    var ciphertextData = ciphertext.getBuffData();
    var ciphertextLen = ciphertext.getBuffSize();

    var plaintextLen = ciphertextLen;
    var updatedPlainLen: c_int = 0;
    var plainDomain: domain(1) = {0..plaintextLen};
    var plaintext: [plainDomain] uint(8);

    EVP_DecryptInit_ex(ctx,
                       cipher,
                       c_nil: ENGINE_PTR,
                       c_ptrTo(keyData): c_ptr(c_uchar),
                       c_ptrTo(ivData): c_ptr(c_uchar));
    EVP_DecryptUpdate(ctx,
                      c_ptrTo(plaintext): c_ptr(c_uchar),
                      c_ptrTo(plaintextLen): c_ptr(c_int),
                      c_ptrTo(ciphertextData): c_ptr(c_uchar),
                      ciphertextLen: c_int);
    EVP_DecryptFinal_ex(ctx,
                        c_ptrTo(plaintext): c_ptr(c_uchar),
                        c_ptrTo(updatedPlainLen): c_ptr(c_int));

   plainDomain = {0..#(plaintextLen + updatedPlainLen)};
   return plaintext;
  }

  class AES {
    pragma "no doc"
    const cipher: EVP_CIPHER_PTR;
    pragma "no doc"
    var byteLen: int;

    /* The `AES` class constructor that initializes the AES encryption
       algorithm with the right key length and chaining mode.

       :arg bits: Number of bits representing the variant of AES based on
                  key-size. (128, 192 or 256)
       :type bits: `int`

       :arg mode: Name of the chaining mode to be used.
       :type mode: `string`

       :return: An object of class `AES`.
       :rtype: `AES`

    */
    proc init(bits: int, mode: string) {
      var tmpCipher: EVP_CIPHER_PTR = nil;
      if (bits == 128 && mode == "cbc") {
        tmpCipher = EVP_aes_128_cbc();
      } else if (bits == 192 && mode == "cbc") {
        tmpCipher = EVP_aes_192_cbc();
      } else if (bits == 256 && mode == "cbc") {
        tmpCipher = EVP_aes_256_cbc();
      } else {
        halt("The desired variant of AES does not exist.");
      }
      this.cipher = tmpCipher;
      this.byteLen = bits/8;
      super.init();
    }

    /* This function returns the size in bytes of the key-length/variant of
       AES which is to be used. For instance,

       - AES128 returns 16
       - AES192 returns 24
       - AES256 returns 32

       :return: Key length of AES in bytes.
       :rtype: `int`

    */
    proc getByteSize(): int {
        return this.byteLen;
    }

    /* This is the 'AES' encrypt routine that encrypts the user supplied message buffer
       using the key and IV.

       The `encrypt` takes in the plaintext buffer, key buffer and IV buffer as the
       arguments and returns a buffer of the ciphertext.

       :arg plaintext: A `CryptoBuffer` representing the plaintext to be encrypted.
       :type plaintext: `CryptoBuffer`

       :arg key: A `CryptoBuffer` representing the key to be used for encryption.
       :type key: `CryptoBuffer`

       :arg IV: A `CryptoBuffer` representing the initialization vector to be used
                for encryption.
       :type IV: `CryptoBuffer`

       :return: A `CryptoBuffer` representing the ciphertext.
       :rtype: `CryptoBuffer`

    */
    proc encrypt(plaintext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer): CryptoBuffer {
      var encryptedPlaintext = aesEncrypt(plaintext, key, IV, this.cipher);
      var encryptedPlaintextBuff = new CryptoBuffer(encryptedPlaintext);
      return encryptedPlaintextBuff;
    }

    /* This is the 'AES' decrypt routine that decrypts the user supplied ciphertext
       buffer using the same key and IV used for encryption.

       The `decrypt` takes in the ciphertext buffer, key buffer and IV buffer as the
       arguments and returns a buffer of the decrypted plaintext.

       :arg ciphertext: A `CryptoBuffer` representing the ciphertext to be decrypted.
       :type ciphertext: `CryptoBuffer`

       :arg key: A `CryptoBuffer` representing the key to be used for dencryption
                 (same as the one used in encryption).
       :type key: `CryptoBuffer`

       :arg IV: A `CryptoBuffer` representing the initialization vector to be used
                for dencryption (same as the one used in encryption).
       :type IV: `CryptoBuffer`

       :return: A `CryptoBuffer` representing the obtained plaintext.
       :rtype: `CryptoBuffer`

    */
    proc decrypt(ciphertext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer): CryptoBuffer {
      var decryptedCiphertext = aesDecrypt(ciphertext, key, IV, this.cipher);
      var decryptedCiphertextBuff = new CryptoBuffer(decryptedCiphertext);
      return decryptedCiphertextBuff;
    }
  }

  pragma "no doc"
  proc createRandomBuffer(buffLen: int) {
    var buff: [0..#buffLen] uint(8);
    var retErrCode: c_int;
    retErrCode = RAND_bytes(c_ptrTo(buff): c_ptr(c_uchar), buffLen: c_int);
    if (!retErrCode) {
      halt("The random buffer generator has failed to initialize a buffer.");
    }
    return buff;
  }

  class CryptoRandom {
    /* This function represents a CSPRNG that generates and allocates the desired
       number of random values as specified by the argument. Halts for number of
       bytes less than 1 (invalid). For instance,

                    `var a = (new CryptoRandom()).createRandomBuffer(5)`

       would give us a `CryptoBuffer` of size `5` and pre-initialized with values.

       :arg buffLen: Number of random values to be generated in the buffer.
       :type buffLen: `int`

       :return: A `CryptoBuffer` representing the generated values.
       :rtype: `CryptoBuffer`

    */
    proc createRandomBuffer(buffLen: int): CryptoBuffer {
      if (buffLen < 1) {
        halt("Invalid random buffer length specified.");
      }
      var randomizedBuff = createRandomBuffer(buffLen);
      var randomizedCryptoBuff = new CryptoBuffer(randomizedBuff);
      return randomizedCryptoBuff;
    }
  }

  pragma "no doc"
  proc PBKDF2_HMAC_PRIM(userKey: string, saltBuff: CryptoBuffer, byteLen: int, iterCount: int, digestName: string) {

    OpenSSL_add_all_digests();

    var key: [0..#byteLen] uint(8);
    var salt = saltBuff.getBuffData();
    var userKeyLen = userKey.length;

    const md = EVP_get_digestbyname(digestName.c_str());

    PKCS5_PBKDF2_HMAC(userKey.c_str(),
                      userKeyLen: c_int,
                      c_ptrTo(salt): c_ptr(c_uchar),
                      byteLen: c_int,
                      iterCount: c_int,
                      md,
                      byteLen: c_int,
                      c_ptrTo(key): c_ptr(c_uchar));

    return key;
  }

  class KDF {
    pragma "no doc"
    var byteLen: int;
    pragma "no doc"
    var iterCount: int;
    pragma "no doc"
    var hashName: string;

    /* The `KDF` class constructor that initializes the common data used by most
       of the Key Derivation Functions.

       :arg bitLen: Size of the expected key in bytes / key length.
       :type bitLen: `int`

       :arg iterCount: The iteration count used by OpenSSL to repeat the `Update`
                       primitive. Min. recommended value = 1000. Greater the
                       `iterCount`, greater the security of the key.
       :type iterCount: `int`

       :arg digest: An object of class 'Hash'. This decides the Hashing function
                    to be used for HMAC based KDFs.
       :type digest: `Hash`

       :return: An object of class `KDF`.
       :rtype: `KDF`

    */
    proc init(byteLen: int, iterCount: int, digest: Hash) {
      if (byteLen < 1) {
        halt("Invalid key size specified.");
      }
      this.byteLen = byteLen;
      this.iterCount = iterCount;
      this.hashName = digest.getDigestName();
      super.init();
    }

    /* This function represents Password-Based KDF 2. It generates a secure-key
       buffer based on the salt and also on the metadata provided in the `KDF`
       constructor.

       :arg userKey: User-specified `string` representation of the key.
       :type userKey: `string`

       :arg saltBuff: A `CryptoBuffer` representing the user generated salt.
       :type saltBuff: `CryptoBuffer`

       :return: A `CryptoBuffer` representing the generated key.
       :rtype: `CryptoBuffer`

    */
    proc passKDF(userKey: string, saltBuff: CryptoBuffer): CryptoBuffer {
      var key = PBKDF2_HMAC_PRIM(userKey, saltBuff, this.byteLen, this.iterCount, this.hashName);
      var keyBuff = new CryptoBuffer(key);
      return keyBuff;
    }
  }

    pragma "no doc"
    proc rsaEncrypt(keys: [] RSAKey, plaintext: CryptoBuffer, ref iv: [] uint(8), ref encSymmKeys: [] CryptoBuffer) {

      var ctx: EVP_CIPHER_CTX;
      EVP_CIPHER_CTX_init(ctx);

      var numKeys = keys.size;
      for i in keys.domain do {
        var keySize = EVP_PKEY_size(keys[i].getKeyPair());
        var dummyMalloc: [1..((keySize): int(64))] uint(8);
        encSymmKeys[i] = new CryptoBuffer(dummyMalloc);
      }

      var encSymmKeysPtr: [keys.domain] c_ptr(uint(8));
      var encryptedSymKeyLen: c_int = 0;
      for i in keys.domain do {
        encSymmKeysPtr[i] = encSymmKeys[i].getBuffPtr();
      }

      var keyObjs: [keys.domain] EVP_PKEY_PTR;
      for i in keys.domain do {
        keyObjs[i] = keys[i].getKeyPair();
      }

      var plaintextBuff = plaintext.getBuffData();
      var plaintextBuffLen = plaintext.getBuffSize();
      var ciphertextLen = plaintextBuffLen + 16;
      var cipherDomain: domain(1) = {0..#ciphertextLen};
      var updatedCipherLen: c_int = 0;
      var ciphertext: [cipherDomain] uint(8);

      EVP_SealInit(ctx, EVP_aes_256_cbc(),
                  c_ptrTo(encSymmKeysPtr),
                  c_ptrTo(encryptedSymKeyLen): c_ptr(c_int),
                  c_ptrTo(iv): c_ptr(c_uchar),
                  c_ptrTo(keyObjs), numKeys: c_int);
      EVP_SealUpdate(ctx,
                    c_ptrTo(ciphertext): c_ptr(c_uchar),
                    c_ptrTo(ciphertextLen): c_ptr(c_int),
                    c_ptrTo(plaintextBuff): c_ptr(c_uchar),
                    plaintextBuffLen: c_int);
      EVP_SealFinal(ctx,
                   c_ptrTo(ciphertext): c_ptr(c_uchar),
                   c_ptrTo(updatedCipherLen): c_ptr(c_int));

      cipherDomain = {0..#(ciphertextLen + updatedCipherLen)};
      return ciphertext;
    }

    pragma "no doc"
    proc rsaDecrypt(key: RSAKey, iv: [] uint(8), ciphertext: [] uint(8), encKeys: [] CryptoBuffer) {

      var ctx: EVP_CIPHER_CTX;
      EVP_CIPHER_CTX_init(ctx);

      var numEncKeys = encKeys.size;
      var openErrCode = 0;

      for i in encKeys.domain do {
        openErrCode = EVP_OpenInit(ctx,
                                  EVP_aes_256_cbc(),
                                  encKeys[i].getBuffPtr(),
                                  encKeys[i].getBuffSize(): c_int,
                                  c_ptrTo(iv): c_ptr(c_uchar),
                                  key.getKeyPair());
        if (openErrCode) {
          break;
        }
      }

      if (!openErrCode) {
        halt("The RSAKey is an invalid match");
      }

      var plaintextLen = ciphertext.size;
      var updatedPlainLen: c_int = 0;
      var plaintextDomain: domain(1) = {0..(plaintextLen)};
      var plaintext: [plaintextDomain] uint(8);

      EVP_OpenUpdate(ctx,
                    c_ptrTo(plaintext): c_ptr(c_uchar),
                    c_ptrTo(plaintextLen): c_ptr(c_int),
                    c_ptrTo(ciphertext): c_ptr(c_uchar),
                    ciphertext.size: c_int);
      EVP_OpenFinal(ctx,
                   c_ptrTo(plaintext): c_ptr(c_uchar),
                   c_ptrTo(updatedPlainLen): c_ptr(c_int));

      plaintextDomain = {0..#(plaintextLen + updatedPlainLen)};
      return plaintext;
    }

  class RSA {

    /* This is the 'RSA' encrypt routine that encrypts the plaintext buffer. This
       uses the `AES` encryption algorithm to encrypt the plaintext and auto-generates
       a secure IV and key. Furthermore, the key is encrypted using `RSA` based on the
       'RSAKey' objects supplied in the arguments.

       The function returns an `Envelope` object that encloses the auto-generated
       IV, AES encrypted ciphertext and an array of `RSA` encrypted key buffers.
       The number of encrypted keys is equal to the number of `RSAKey` objects
       in the array. This kind of use case, is useful specifically in developing
       one-to-many systems such as GPG.

       For instance, consider a server-client model where the server performs
       encryption using the public keys of all the clients and returns an `Envelope`
       to each client. Now, the dencryption can only be performed by each client
       using only their own private key.

       :arg plaintext: A `CryptoBuffer` representing the plaintext to be
                       encrypted.
       :type plaintext: `CryptoBuffer`

       :arg keys: An array of `RSAKey` objects. This depends on the number of
                  different RSA encryptions that are to be performed on the same
                  plaintext.
       :type keys: `[] RSAKey`

       :return: An `Envelope` object which comprises of the IV buffer, array of
                RSA encrypted keys and AES encrypted ciphertext.
       :rtype: `Envelope`

    */
    proc encrypt(plaintext: CryptoBuffer, keys: [] RSAKey): Envelope {
      var ivLen = EVP_CIPHER_iv_length(EVP_aes_256_cbc()) - 1;
      var iv: [0..(ivLen: int(64))] uint(8);

      var encSymmKeys: [keys.domain] CryptoBuffer;

      var ciphertext = rsaEncrypt(keys, plaintext, iv, encSymmKeys);

      var envp = new Envelope(new CryptoBuffer(iv), encSymmKeys, new CryptoBuffer(ciphertext));
      return envp;
    }

    /* This is the 'RSA' decrypt routine that decrypts the ciphertext buffer. This
       uses `AES` dencryption to decrypt the ciphertext.

       It's accepts the entire `Envelope` object but only a single `RSAKey` object
       for unilateral dencryption.

       :arg envp: A `CryptoBuffer` representing the plaintext to be
                       encrypted.
       :type envp: `Envelope`

       :arg key: An array of `RSAKey` objects. This depends on the number of
                  different RSA encryptions that are to be performed on the same
                  plaintext.
       :type key: `RSAKey`

       :return: An `Envelope` object which comprises of the IV buffer, array of
                RSA encrypted keys and AES encrypted ciphertext.
       :rtype: `CryptoBuffer`

    */
    proc decrypt(envp: Envelope, key: RSAKey): CryptoBuffer {
      var iv = envp.getIV().getBuffData();
      var ciphertext = envp.getEncMessage().getBuffData();
      var encKeys = envp.getEncKeys();

      var plaintext = rsaDecrypt(key, iv, ciphertext, encKeys);
      var plaintextBuff = new CryptoBuffer(plaintext);
      return plaintextBuff;
    }
  }
}


/*
    Support for low-level native C_OpenSSL bindings.
    This submodule wraps the C_OpenSSL implementation, providing access to
    most of the C_OpenSSL calls.

    Refer to the
    `C_OpenSSL documentation <https://www.openssl.org/docs/manmaster/man3/>`_
    of the reference version for the usage of this module.
    
  */
module C_OpenSSL {
  require "openssl/evp.h", "openssl/pem.h",
          "openssl/bn.h", "openssl/bio.h",
          "openssl/aes.h", "openssl/rand.h", "-lcrypto";
  //require "CryptoHandlers/rsa_complex_bypass_handler.h";

  extern type EVP_PKEY_CTX;
  extern type EVP_PKEY;
  extern var EVP_PKEY_RSA: c_int;

  extern type EVP_PKEY_CTX_PTR = c_ptr(EVP_PKEY_CTX);
  extern type EVP_PKEY_PTR = c_ptr(EVP_PKEY);

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

  extern type EVP_CIPHER;
  extern type EVP_CIPHER_CTX;

  extern type EVP_CIPHER_PTR = c_ptr(EVP_CIPHER);
  extern type EVP_CIPHER_CTX_PTR = c_ptr(EVP_CIPHER_CTX);

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

  extern proc PKCS5_PBKDF2_HMAC(pass: c_string,
                                passlen: c_int,
                                const salt: c_ptr(c_uchar),
                                saltlen: c_int,
                                iterCount: c_int,
                                const digest: EVP_MD_PTR,
                                keylen: c_int,
                                outx: c_ptr(c_uchar)): c_int;
  extern proc RAND_seed(const buf: c_void_ptr, num: c_int);
}
