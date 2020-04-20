/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

/*  A cryptographic library based on `OpenSSL <https://www.openssl.org/>`_, targeted at
    flexible encryption purposes.

    The Crypto module focuses on providing various cryptographic utilities such as

    - Symmetric Ciphers

      + :chpl:class:`AES`
      + :chpl:class:`Blowfish`

    - Asymmetric Ciphers

      + :chpl:class:`RSA`

    - Hashing Functions

      + :chpl:class:`Hash`

    - Key Derivation Functions

      + :chpl:class:`KDF`

    - Cryptographically-Secure Pseudo-Random Number Generators

      + :chpl:class:`CryptoRandom`

    - Miscellaneous Utilities.

      + :chpl:class:`CryptoBuffer`
      + :chpl:class:`RSAKey`
      + :chpl:class:`Envelope`

    .. note::

      This module is currently under development and will expand significantly
      in upcoming releases.

    Compiling with Crypto
    ----------------------

    In order to compile a Chapel program that uses this module, the
    Crypto and C_OpenSSL (C wrappers to OpenSSL) libraries must be installed on the system.
    The paths to both the ``openssl/<module>.h`` header files and OpenSSL library
    must be passed to the ``-I`` and ``-L`` compiler arguments. The library name,
    typically ``crypto``, must be passed to the ``-l`` argument as well.

    The compilation command should look something like this:

    .. code-block:: sh

        chpl -I$PATH_TO_OPENSSL_DIR \
             -L$PATH_TO_OPENSSL_LIBS -lcrypto source.chpl

    This would also work by setting the ``LDFLAGS`` and ``CPPFLAGS`` environment
    variables to the expected paths on certain systems.


    Chapel Crypto API
    ------------------
*/

module Crypto {

  use C_OpenSSL;
  use SysError;

  private use IO;
  private use SysCTypes;

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

  /* The `CryptoBuffer` class is a wrapper around the internal representation
     of how the values in this library are stored. Every sequence of bytes going
     into a Crypto utility or coming out of it, is a `CryptoBuffer`.

     A `CryptoBuffer` can enclose a `string` or a `[] uint(8)` passed to its
     initializer and provides helper functions to access those values.

  */
  class CryptoBuffer {
    pragma "no doc"
    var _len: int = 0;
    pragma "no doc"
    var buffDomain: domain(1);
    pragma "no doc"
    var buff: [buffDomain] uint(8);

    /* The `CryptoBuffer` class initializer that initializes the buffer
       when a `string` is supplied to it.

       :arg s: `string` input for buffer conversion.
       :type s: `string`

       :return: An object of class `CryptoBuffer`.
       :rtype: `CryptoBuffer`

    */
    proc init(s: string) {
      this.complete();
      this._len = s.numBytes;
      if (this._len == 0) {
        halt("Enter a string with length greater than 0 in order to create a buffer");
      }
      this.buffDomain = {0..<this._len};
      for i in this.buffDomain do {
        this.buff[i] = s.byte(i);
      }
    }

    /* The `CryptoBuffer` class initializer that initializes the buffer
       when a `[] uint(8)` is supplied to it.

       :arg s: `[] uint(8)` input for buffer conversion.
       :type s: `[] uint(8)`

       :return: An object of class `CryptoBuffer`.
       :rtype: `CryptoBuffer`

    */
    proc init(s: [] uint(8)) {
      this.complete();
      this._len = s.size;
      if (this._len == 0) {
        halt("Enter an array with size greater than 0 in order to create a buffer");
      }
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
    proc toHex() throws {
      var buffHex: [this.buffDomain] string;
      for i in this.buffDomain do {
        buffHex[i] = try "%02xu".format(this.buff[i]);
      }
      return buffHex;
    }

    /* Returns the hexadecimal string representation of the entire internal buffer.

       :return: hex string representation of the internal buffer.
       :rtype: `string`

    */
    proc toHexString() throws {
      var buffHexString: string;
      for i in this.buffDomain do {
        buffHexString += try "%02xu".format(this.buff[i]);
      }
      return buffHexString;
    }
  }

  /* `RSAKey` class encloses the `EVP_PKEY` object provided by the
     OpenSSL primitives. The `EVP_PKEY` object can contain the public key,
     private key or both of them. Hence, the contents of an object of the
     class `RSAKey` may be decided by the user.

     Calling the `RSAKey` initializer without using any key import or export
     functions may result in generation of a single object that contains both the
     keys (public and private).

     In order to separate out the key objects, keys can be imported
     from a `.pem` file. (TODO)

  */
  class RSAKey {
    pragma "no doc"
    var keyLen: int;
    pragma "no doc"
    var keyObj: EVP_PKEY_PTR;

    /* The `RSAKey` class initializer that initializes the `EVP_PKEY` object
       of OpenSSL and basically, initializes a set of public and private keys.

       It checks for valid RSA key lengths and generates a public key and private
       key pair accordingly.

       :arg keyLen: RSA Key length in bits.
       :type keyLen: `int`

       :return: An object of class `RSAKey` representing the key pair.
       :rtype: `RSAKey`

    */
    proc init(keyLen: int) {
      this.complete();
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

  /* The `Envelope` class wraps all the data returned by the encrypt function
     of the `RSA` class along with some utility data. An `RSA` encrypt function
     returns an array of `RSA` encrypted symmetric keys and a single `AES`
     encrypted message. The `Envelope` also encloses the initialization vector
     used during encryption such that it can be utilized during the decryption
     phase as well.

  */
  class Envelope {
    pragma "no doc"
    var keyDomain: domain(1);
    pragma "no doc"
    var keys: [keyDomain] owned CryptoBuffer;
    pragma "no doc"
    var iv: owned CryptoBuffer;
    pragma "no doc"
    var value: owned CryptoBuffer;

    /* The `Envelope` class initializer that encapsulates the IV, AES encrypted
       ciphertext buffer and an array of encrypted key buffers.

       :arg iv: Initialization Vector.
       :type iv: `owned CryptoBuffer`

       :arg encSymmKey: Array of encrypted symmetric (AES) keys.
       :type encSymmKey: `[] owned CryptoBuffer`

       :arg encSymmValue: AES-encrypted ciphertext buffer.
       :type encSymmValue: `owned CryptoBuffer`

       :return: An object of class `Envelope`.
       :rtype: `Envelope`

    */
    proc init(iv: owned CryptoBuffer, encSymmKey: [] owned CryptoBuffer, encSymmValue: owned CryptoBuffer) {
      this.complete();
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
      return this.value.borrow();
    }

    /* This function returns the IV generated by the `encrypt` routine and
       encapsulated in the `Envelope`. This is used for both encryption and
       decryption.

       :return: Initialization Vector.
       :rtype: `CryptoBuffer`

    */
    proc getIV() {
      return this.iv.borrow();
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
      return this.keys[i].borrow();
    }

    /* This function returns the entire array of symmetric key buffers.

       :return: Entire array of key buffers.
       :rtype: `[] CryptoBuffer`

    */
    proc getEncKeys() {
      return this.keys.borrow();
    }
  }

  pragma "no doc"
  proc digestPrimitives(digestName: string, hashLen: int, inputBuffer: CryptoBuffer) {

    OpenSSL_add_all_digests();

    var ctx = CHPL_EVP_MD_CTX_new();

    var hash: [0..#hashLen] uint(8); ;
    var retHashLen: c_uint = 0;

    var md: CONST_EVP_MD_PTR;
    md = EVP_get_digestbyname(digestName.c_str());

    EVP_DigestInit_ex(CHPL_EVP_MD_CTX_ptr(ctx), md, c_nil: ENGINE_PTR);
    EVP_DigestUpdate(CHPL_EVP_MD_CTX_ptr(ctx), c_ptrTo(inputBuffer.buff): c_void_ptr, inputBuffer._len: size_t);
    EVP_DigestFinal_ex(CHPL_EVP_MD_CTX_ptr(ctx), c_ptrTo(hash): c_ptr(c_uchar), retHashLen);

    CHPL_EVP_MD_CTX_free(ctx);
    return hash;
  }

  /* The `Digest` enum represents all the hashing functions provided by the
     OpenSSL primitives. Value from this enum is passed to the `Hash` class
     initializer in order to select the type of hashing function to be used.

  */
  enum Digest {
    MD5, SHA1, SHA224, SHA256, SHA384, SHA512, RIPEMD160
  }
  /* The `Hash` class represents all the hashing functions provided by the
     OpenSSL primitives. It supports all the prominent and most commonly used
     deterministic hashing functions.

  */
  class Hash {
    pragma "no doc"
    var hashLen: int;
    pragma "no doc"
    var digestName: string;
    pragma "no doc"
    var hashDomain: domain(1);
    pragma "no doc"
    var hashSpace: [hashDomain] uint(8);

    /* The `Hash` class initializer that initializes the hashing function
       to be used. This initializer sets the byte length of the
       respective hash and allocates a domain for memory allocation
       for hashing. It currently supports the following hashing functions -
       ``MD5``, ``SHA1``, ``SHA224``, ``SHA256``, ``SHA384``, ``SHA512`` and
       ``RIPEMD160`` consumed via an enum, `Digest`.

       :arg digestName: Hashing function to be used.
       :type digestName: `Digest`

       :return: An object of class `Hash`.
       :rtype: `Hash`

       Initialization example,

       .. code-block:: chapel

          var h = new Hash(Digest.SHA256);

    */
    proc init(digestName: Digest) {
      this.complete();
      select digestName {
        when Digest.MD5        do this.hashLen = 16;
        when Digest.SHA1       do this.hashLen = 20;
        when Digest.SHA224     do this.hashLen = 28;
        when Digest.SHA256     do this.hashLen = 32;
        when Digest.SHA384     do this.hashLen = 48;
        when Digest.SHA512     do this.hashLen = 64;
        when Digest.RIPEMD160  do this.hashLen = 20;
        otherwise do halt("A digest with the name \'" + digestName: string + "\' doesn't exist.");
      }
      this.digestName = digestName: string;
      this.hashDomain = {0..#this.hashLen};
    }

    /* Returns the name of the hashing function used. For instance,
      instantiating an object with ``var hash = new Hash("SHA256")``,
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

       :return: An `owned CryptoBuffer` representing the hash digest.
       :rtype: `owned CryptoBuffer`

    */
    proc getDigest(inputBuffer: CryptoBuffer): owned CryptoBuffer {
      this.hashSpace = digestPrimitives(this.digestName, this.hashLen, inputBuffer);
      var hashBuffer = new owned CryptoBuffer(this.hashSpace);
      return hashBuffer;
    }
  }

  pragma "no doc"
  proc aesEncrypt(plaintext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer, cipher: CONST_EVP_CIPHER_PTR) {

    var ctx = CHPL_EVP_CIPHER_CTX_new();

    var keyData = key.getBuffData();
    var ivData = IV.getBuffData();
    var plaintextData = plaintext.getBuffData();
    var plaintextLen = plaintext.getBuffSize();

    var ciphertextLen = plaintextLen + 16; // 16 is the MAX_BLOCK_SIZE for AES
    var cipherDomain: domain(1) = {0..ciphertextLen};
    var updatedCipherLen: c_int = 0;
    var ciphertext: [cipherDomain] uint(8);

    EVP_EncryptInit_ex(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                       cipher,
                       c_nil: ENGINE_PTR,
                       c_ptrTo(keyData): c_ptr(c_uchar),
                       c_ptrTo(ivData): c_ptr(c_uchar));
    EVP_EncryptUpdate(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                      c_ptrTo(ciphertext): c_ptr(c_uchar),
                      c_ptrTo(ciphertextLen): c_ptr(c_int),
                      c_ptrTo(plaintextData): c_ptr(c_uchar),
                      plaintextLen: c_int);
    EVP_EncryptFinal_ex(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                        c_ptrTo(ciphertext): c_ptr(c_uchar),
                        c_ptrTo(updatedCipherLen): c_ptr(c_int));

    cipherDomain = {0..#(ciphertextLen + updatedCipherLen)};

    CHPL_EVP_CIPHER_CTX_free(ctx);
    return ciphertext;
  }

  pragma "no doc"
  proc aesDecrypt(ciphertext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer, cipher: CONST_EVP_CIPHER_PTR) {

    var ctx = CHPL_EVP_CIPHER_CTX_new();

    var keyData = key.getBuffData();
    var ivData = IV.getBuffData();
    var ciphertextData = ciphertext.getBuffData();
    var ciphertextLen = ciphertext.getBuffSize();

    var plaintextLen = ciphertextLen;
    var updatedPlainLen: c_int = 0;
    var plainDomain: domain(1) = {0..plaintextLen};
    var plaintext: [plainDomain] uint(8);

    EVP_DecryptInit_ex(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                       cipher,
                       c_nil: ENGINE_PTR,
                       c_ptrTo(keyData): c_ptr(c_uchar),
                       c_ptrTo(ivData): c_ptr(c_uchar));
    EVP_DecryptUpdate(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                      c_ptrTo(plaintext): c_ptr(c_uchar),
                      c_ptrTo(plaintextLen): c_ptr(c_int),
                      c_ptrTo(ciphertextData): c_ptr(c_uchar),
                      ciphertextLen: c_int);
    EVP_DecryptFinal_ex(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                        c_ptrTo(plaintext): c_ptr(c_uchar),
                        c_ptrTo(updatedPlainLen): c_ptr(c_int));

   plainDomain = {0..#(plaintextLen + updatedPlainLen)};

   CHPL_EVP_CIPHER_CTX_free(ctx);
   return plaintext;
  }

  /* The `CryptoChainMode` enum represents all cipher chaining modes
     that can be used by a symmetric cipher. It is used by the `AES` and
     `Blowfish` class initializers to select a chaining mode.

  */
  enum CryptoChainMode {
    cbc, ecb, cfb, ofb
  }
  /* The `AES` class represents the symmetric encryption algorithm, AES.
     The Advanced Encryption Standard (AES), also known by its original name Rijndael
     is a specification for the encryption of electronic data established by the
     U.S. National Institute of Standards and Technology (NIST) in 2001.

     It is the most widely used symmetric cipher and is also used to encrypt
     messages in public-key cryptography such as within `RSA`.

     Currently, the `AES` class allows symmetric encryption using only the CBC or
     Cipher Block Chaining mode in 128, 192, and 256 key size variants.

     After thorough testing, ECB, OCB and other chaining mode variants will also
     be added to this library(TODO).

  */
  class AES {
    pragma "no doc"
    var cipher: CONST_EVP_CIPHER_PTR;
    pragma "no doc"
    var byteLen: int;

    /* The `AES` class initializer that initializes the AES encryption
       algorithm with the right key length and chaining mode.

       :arg bits: Number of bits representing the variant of AES based on
                  key-size. (128, 192 or 256)
       :type bits: `int`

       :arg mode: Chaining mode to be used.
       :type mode: `CryptoChainMode`

       :return: An object of class `AES`.
       :rtype: `AES`

       Initialization example,

       .. code-block:: chapel

          var aes = new AES(256, CryptoChainMode.cbc);

    */
    proc init(bits: int, mode: CryptoChainMode) {
      var tmpCipher: CONST_EVP_CIPHER_PTR;
      if (bits == 128 && mode == CryptoChainMode.cbc) {
        tmpCipher = EVP_aes_128_cbc();
      } else if (bits == 192 && mode == CryptoChainMode.cbc) {
        tmpCipher = EVP_aes_192_cbc();
      } else if (bits == 256 && mode == CryptoChainMode.cbc) {
        tmpCipher = EVP_aes_256_cbc();
      } else {
        halt("The desired variant of AES does not exist.");
      }
      this.cipher = tmpCipher;
      this.byteLen = bits/8;
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

       :return: An `owned CryptoBuffer` representing the ciphertext.
       :rtype: `owned CryptoBuffer`

    */
    proc encrypt(plaintext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer): owned CryptoBuffer {
      var encryptedPlaintext = aesEncrypt(plaintext, key, IV, this.cipher);
      var encryptedPlaintextBuff = new owned CryptoBuffer(encryptedPlaintext);
      return encryptedPlaintextBuff;
    }

    /* This is the 'AES' decrypt routine that decrypts the user supplied ciphertext
       buffer using the same key and IV used for encryption.

       The `decrypt` takes in the ciphertext buffer, key buffer and IV buffer as the
       arguments and returns a buffer of the decrypted plaintext.

       :arg ciphertext: A `CryptoBuffer` representing the ciphertext to be decrypted.
       :type ciphertext: `CryptoBuffer`

       :arg key: A `CryptoBuffer` representing the key to be used for decryption
                 (same as the one used in encryption).
       :type key: `CryptoBuffer`

       :arg IV: A `CryptoBuffer` representing the initialization vector to be used
                for decryption (same as the one used in encryption).
       :type IV: `CryptoBuffer`

       :return: An `owned CryptoBuffer` representing the obtained plaintext.
       :rtype: `owned CryptoBuffer`

    */
    proc decrypt(ciphertext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer): owned CryptoBuffer {
      var decryptedCiphertext = aesDecrypt(ciphertext, key, IV, this.cipher);
      var decryptedCiphertextBuff = new owned CryptoBuffer(decryptedCiphertext);
      return decryptedCiphertextBuff;
    }
  }

pragma "no doc"
proc bfEncrypt(plaintext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer, cipher: CONST_EVP_CIPHER_PTR) {

    var ctx = CHPL_EVP_CIPHER_CTX_new();

    var keyData = key.getBuffData();
    var ivData = IV.getBuffData();
    var plaintextData = plaintext.getBuffData();
    var plaintextLen = plaintext.getBuffSize();

    var ciphertextLen = plaintextLen + 8;
    var cipherDomain: domain(1) = {0..#ciphertextLen};
    var updatedCipherLen: c_int = 0;
    var ciphertext: [cipherDomain] uint(8);

    EVP_EncryptInit_ex(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                       cipher,
                       c_nil: ENGINE_PTR,
                       c_ptrTo(keyData): c_ptr(c_uchar),
                       c_ptrTo(ivData): c_ptr(c_uchar));

    EVP_EncryptUpdate(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                      c_ptrTo(ciphertext): c_ptr(c_uchar),
                      c_ptrTo(ciphertextLen): c_ptr(c_int),
                      c_ptrTo(plaintextData): c_ptr(c_uchar),
                      plaintextLen: c_int);

    EVP_EncryptFinal_ex(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                        c_ptrTo(ciphertext[ciphertextLen..]): c_ptr(c_uchar),
                        c_ptrTo(updatedCipherLen): c_ptr(c_int));
    cipherDomain = {0..#(ciphertextLen + updatedCipherLen)};

    CHPL_EVP_CIPHER_CTX_free(ctx);
    return ciphertext;
  }

  pragma "no doc"
  proc bfDecrypt(ciphertext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer, cipher: CONST_EVP_CIPHER_PTR) {

    var ctx = CHPL_EVP_CIPHER_CTX_new();

    var keyData = key.getBuffData();
    var ivData = IV.getBuffData();
    var ciphertextData = ciphertext.getBuffData();
    var ciphertextLen = ciphertext.getBuffSize();

    var plaintextLen = ciphertextLen;
    var updatedPlainLen: c_int = 0;
    var plainDomain: domain(1) = {0..plaintextLen};
    var plaintext: [plainDomain] uint(8);

    EVP_DecryptInit_ex(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                       cipher,
                       c_nil: ENGINE_PTR,
                       c_ptrTo(keyData): c_ptr(c_uchar),
                       c_ptrTo(ivData): c_ptr(c_uchar));
    EVP_DecryptUpdate(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                      c_ptrTo(plaintext): c_ptr(c_uchar),
                      c_ptrTo(plaintextLen): c_ptr(c_int),
                      c_ptrTo(ciphertextData): c_ptr(c_uchar),
                      ciphertextLen: c_int);
    EVP_DecryptFinal_ex(CHPL_EVP_CIPHER_CTX_ptr(ctx),
                        c_ptrTo(plaintext[plaintextLen..]): c_ptr(c_uchar),
                        c_ptrTo(updatedPlainLen): c_ptr(c_int));

    plainDomain = {0..#(plaintextLen + updatedPlainLen)};

    CHPL_EVP_CIPHER_CTX_free(ctx);
    return plaintext;
  }

  /* The `Blowfish` class represents a symmetric-key block cipher called Blowfish, designed
     in 1993 by Bruce Schneier. Considering current scenario, the Advanced Encryption
     Standard(AES) cipher is used more in practice. Since, Blowfish is unpatented and placed
     in the public domain, it receives a decent amount of attention from the community.

     .. note::

       Since the key in Blowfish can be of various sizes, use a :chpl:class:`KDF` routine
       to generate a secure-key with the recommended byte-size as 16 bytes. Also, the
       initialization vector in Blowfish cipher should strictly be 8 bytes long.


     The `Blowfish` class supports 4 `chaining modes <https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation>`_:

     - ``cbc`` or `Cipher Block Chaining`

     - ``ecb`` or `Electronic Codebook`

     - ``cfb`` or `Cipher Feedback`

     - ``ofb`` or `Output Feedback`

  */
  class Blowfish {
    pragma "no doc"
    var cipher: CONST_EVP_CIPHER_PTR;
    pragma "no doc"
    var byteLen: int;

    /* The `Blowfish` class initializer that initializes the Blowfish encryption
       algorithm with the right key length and chaining mode.

       :arg mode: Name of the chaining mode to be used.
       :type mode: `CryptoChainMode`

       :return: An object of class `Blowfish`.
       :rtype: `Blowfish`

       Initialization example,

       .. code-block:: chapel

          var bf = new Blowfish(CryptoChainMode.cbc);

    */
    proc init(mode: CryptoChainMode) {
      var tmpCipher: CONST_EVP_CIPHER_PTR;
      select mode {
        when CryptoChainMode.cbc  do tmpCipher = EVP_bf_cbc();
        when CryptoChainMode.ecb  do tmpCipher = EVP_bf_ecb();
        when CryptoChainMode.cfb  do tmpCipher = EVP_bf_cfb();
        when CryptoChainMode.ofb  do tmpCipher = EVP_bf_ofb();
        otherwise do halt("The desired variant of Blowfish cipher does not exist.");
      }
      this.cipher = tmpCipher;
    }

    /* This is the 'Blowfish' encrypt routine that encrypts the user supplied message buffer
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

       :return: An `owned CryptoBuffer` representing the ciphertext.
       :rtype: `owned CryptoBuffer`

    */
    proc encrypt(plaintext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer): owned CryptoBuffer throws {
      var ivLen = IV.getBuffSize();
      var keyLen = key.getBuffSize();
      if (ivLen != 8) {
        throw new owned IllegalArgumentError("IV", "Blowfish cipher expects a size of 8 bytes.");
      }

      if (keyLen < 10) {
        throw new owned IllegalArgumentError("key", "Blowfish cipher expects a size greater than 10 bytes.");
      }
      var encryptedPlaintext = bfEncrypt(plaintext, key, IV, this.cipher);
      var encryptedPlaintextBuff = new owned CryptoBuffer(encryptedPlaintext);
      return encryptedPlaintextBuff;
    }

    /* This is the 'Blowfish' decrypt routine that decrypts the user supplied ciphertext
       buffer using the same key and IV used for encryption.

       The `decrypt` takes in the ciphertext buffer, key buffer and IV buffer as the
       arguments and returns a buffer of the decrypted plaintext.

       :arg ciphertext: A `CryptoBuffer` representing the ciphertext to be decrypted.
       :type ciphertext: `CryptoBuffer`

       :arg key: A `CryptoBuffer` representing the key to be used for decryption
                 (same as the one used in encryption).
       :type key: `CryptoBuffer`

       :arg IV: A `CryptoBuffer` representing the initialization vector to be used
                for decryption (same as the one used in encryption).
       :type IV: `CryptoBuffer`

       :return: An `owned CryptoBuffer` representing the obtained plaintext.
       :rtype: `owned CryptoBuffer`

    */
    proc decrypt(ciphertext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer): owned CryptoBuffer {
      var decryptedCiphertext = bfDecrypt(ciphertext, key, IV, this.cipher);
      var decryptedCiphertextBuff = new owned CryptoBuffer(decryptedCiphertext);
      return decryptedCiphertextBuff;
    }
  }

  pragma "no doc"
  proc createRandomBuffer(buffLen: int) throws {
    var buff: [0..#buffLen] uint(8);
    var retErrCode: c_int;
    retErrCode = RAND_bytes(c_ptrTo(buff): c_ptr(c_uchar), buffLen: c_int);
    if (!retErrCode) {
      throw SystemError.fromSyserr(retErrCode);
    }
    return buff;
  }

  /* The `CryptoRandom` class represents a CSPRNG provided by OpenSSL that
     automatically does the seeding part before returning a random sequence
     of bytes of the type `CryptoBuffer`.

     Given the length of the buffer required by the user, it generates a random
     buffer of the same size.

  */
  class CryptoRandom {
    /* This function represents a CSPRNG that generates and allocates the desired
       number of random values as specified by the argument. Halts for number of
       bytes less than 1 (invalid). For instance,

       .. code-block:: chapel

          var a = (new CryptoRandom()).getRandomBuffer(5)

       would give us a `CryptoBuffer` of size `5` and pre-initialized with values.

       :arg buffLen: Number of random values to be generated in the buffer.
       :type buffLen: `int`

       :return: An `owned CryptoBuffer` representing the generated values.
       :rtype: `owned CryptoBuffer`

    */
    proc getRandomBuffer(buffLen: int): owned CryptoBuffer throws {
      if (buffLen < 1) {
        throw new owned IllegalArgumentError("buffLen", "Invalid random buffer length specified.");
      }
      var randomizedBuff = try createRandomBuffer(buffLen);
      var randomizedCryptoBuff = new owned CryptoBuffer(randomizedBuff);
      return randomizedCryptoBuff;
    }
  }

  pragma "no doc"
  proc PBKDF2(userKey: string, saltBuff: CryptoBuffer, byteLen: int, iterCount: int, digestName: string) {

    OpenSSL_add_all_digests();

    var key: [0..#byteLen] uint(8);
    var salt = saltBuff.getBuffData();
    var saltLen = saltBuff.getBuffSize();
    var userKeyLen = userKey.numBytes;

    var md: CONST_EVP_MD_PTR;
    md = EVP_get_digestbyname(digestName.c_str());

    PKCS5_PBKDF2_HMAC(userKey.c_str(), userKeyLen: c_int,
                      c_ptrTo(salt): c_ptr(c_uchar),
                      saltLen: c_int, iterCount: c_int, md,
                      byteLen: c_int, c_ptrTo(key): c_ptr(c_uchar));

    return key;
  }

  /* The `KDF` class contains the most widely used key derivation functions.
     Currently it supports a single password based key derivation function which
     is the most secure one as of writing this library. More KDFs will be added
     to this in the future.

  */
  class KDF {
    pragma "no doc"
    var byteLen: int;
    pragma "no doc"
    var iterCount: int;
    pragma "no doc"
    var hashName: string;

    /* The `KDF` class initializer that initializes the common data used by most
       of the Key Derivation Functions.

       :arg byteLen: Size of the expected key in bytes / key length.
       :type byteLen: `int`

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
    }

    /* This function represents Password-Based KDF 2. It generates a secure-key
       buffer based on the salt and also on the metadata provided in the `KDF`
       initializer.

       :arg userKey: User-specified `string` representation of the key.
       :type userKey: `string`

       :arg saltBuff: A `CryptoBuffer` representing the user generated salt.
       :type saltBuff: `CryptoBuffer`

       :return: An `owned CryptoBuffer` representing the generated key.
       :rtype: `owned CryptoBuffer`

    */
    proc passKDF(userKey: string, saltBuff: CryptoBuffer): owned CryptoBuffer {
      var key = PBKDF2(userKey, saltBuff, this.byteLen, this.iterCount, this.hashName);
      var keyBuff = new owned CryptoBuffer(key);
      return keyBuff;
    }
  }

    pragma "no doc"
    proc rsaEncrypt(keys: [] RSAKey, plaintext: CryptoBuffer, ref iv: [] uint(8), ref encSymmKeys: [] CryptoBuffer) {

      var ctx: EVP_CIPHER_CTX;
      EVP_CIPHER_CTX_init(ctx); // TODO

      var numKeys = keys.size;
      for i in keys.domain do {
        var keySize = EVP_PKEY_size(keys[i].getKeyPair());
        var dummyMalloc: [1..((keySize): int(64))] uint(8);
        encSymmKeys[i] = new unmanaged CryptoBuffer(dummyMalloc);
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
    proc rsaDecrypt(key: RSAKey, iv: [] uint(8), ciphertext: [] uint(8), encKeys: [] CryptoBuffer) throws {

      var ctx: EVP_CIPHER_CTX;
      EVP_CIPHER_CTX_init(ctx); // TODO

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
        throw new owned IllegalArgumentError("key", "The RSAKey is an invalid match.");
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

    /* RSA is one of the first practical public-key cryptosystems and is widely
       used for secure data transmission. A user of the `RSA` first generates the
       public and private keys using the `RSAKey` class. The `RSAKey` object is
       then passed to the `RSA` functions for further encryption and decryption
       purposes.

       The `encrypt` function of this class takes in an array of `RSAKey` objects
       and uses all of them to perform the encryption.

       Similarly, decryption can be done individually performed (without passing an array
       as an argument) by passing a single `RSAKey` object every time the decryption takes place.

    */
  class RSA {

    /* This is the 'RSA' encrypt routine that encrypts the plaintext buffer. This
       uses the `AES` encryption algorithm to encrypt the plaintext and auto-generates
       a secure IV and key. Furthermore, the key is encrypted using `RSA` based on the
       'RSAKey' objects supplied in the arguments.

       The function returns an `Envelope` object that encloses the auto-generated
       IV, AES encrypted ciphertext and an array of `RSA` encrypted key buffers.
       The number of encrypted keys is equal to the number of `RSAKey` objects
       in the array. This kind of use case is useful specifically in developing
       one-to-many systems such as GPG.

       For instance, consider a server-client model where the server performs
       encryption using the public keys of all the clients and returns an `Envelope`
       to each client. Now, the decryption can only be performed by each client
       using only their own private key.

       :arg plaintext: A `CryptoBuffer` representing the plaintext to be
                       encrypted.
       :type plaintext: `CryptoBuffer`

       :arg keys: An array of `RSAKey` objects. This depends on the number of
                  different RSA encryptions that are to be performed on the same
                  plaintext.
       :type keys: `[] RSAKey`

       :return: An `owned Envelope` object which comprises of the IV buffer,
                array of RSA encrypted keys and AES encrypted ciphertext.
       :rtype: `owned Envelope`

    */
    proc encrypt(plaintext: CryptoBuffer, keys: [] RSAKey): owned Envelope {
      var ivLen = EVP_CIPHER_iv_length(EVP_aes_256_cbc()) - 1;
      var iv: [0..(ivLen: int(64))] uint(8);

      var encSymmKeys: [keys.domain] CryptoBuffer;

      var ciphertext = rsaEncrypt(keys, plaintext, iv, encSymmKeys);

      var envp = new owned Envelope(new owned CryptoBuffer(iv), encSymmKeys, new owned CryptoBuffer(ciphertext));
      return envp;
    }

    /* This is the 'RSA' decrypt routine that decrypts the ciphertext buffer. This
       uses `AES` decryption to decrypt the ciphertext.

       It accepts the entire `Envelope` object but only a single `RSAKey` object
       for unilateral decryption.

       :arg envp: A `CryptoBuffer` representing the plaintext to be
                       encrypted.
       :type envp: `Envelope`

       :arg key: An array of `RSAKey` objects. This depends on the number of
                  different RSA encryptions that are to be performed on the same
                  plaintext.
       :type key: `RSAKey`

       :return: An `owned CryptoBuffer` representing the obtained plaintext.
       :rtype: `owned CryptoBuffer`

    */
    proc decrypt(envp: Envelope, key: RSAKey): owned CryptoBuffer throws {
      var iv = envp.getIV().getBuffData();
      var ciphertext = envp.getEncMessage().getBuffData();
      var encKeys = envp.getEncKeys();

      var plaintext = try rsaDecrypt(key, iv, ciphertext, encKeys);
      var plaintextBuff = new owned CryptoBuffer(plaintext);
      return plaintextBuff;
    }
  }

  /*
      Support for low-level native C_OpenSSL bindings.
      This submodule wraps the C_OpenSSL implementation, providing access to
      most of the C_OpenSSL calls.

      Refer to the `C_OpenSSL documentation <https://www.openssl.org/docs/manmaster/man3/>`_
      of the reference version for the usage of this module.

    */
  module C_OpenSSL {
    // The RSA module include needs special treatment since it uses
    // I as an argument even though Chapel will have already included
    // complex.h which defines I. See issue #6824.
    require "CryptoHandlers/openssl_c_support.h";

    require "openssl/pem.h", "openssl/bn.h", "openssl/bio.h", "openssl/evp.h",
            "openssl/aes.h", "openssl/rand.h", "openssl/sha.h", "-lcrypto", "-lssl";

    use SysCTypes;

    extern type EVP_PKEY_CTX;
    extern type EVP_PKEY;
    extern var EVP_PKEY_RSA: c_int;

    extern type EVP_PKEY_CTX_PTR = c_ptr(EVP_PKEY_CTX);
    extern type EVP_PKEY_PTR = c_ptr(EVP_PKEY);
    extern type CONST_EVP_MD_PTR;
    extern type CONST_EVP_CIPHER_PTR;

    extern proc EVP_CIPHER_iv_length(e: CONST_EVP_CIPHER_PTR): c_int;
    extern proc EVP_PKEY_size(pkey: EVP_PKEY_PTR): c_int;
    extern proc EVP_PKEY_CTX_new_id(id: c_int, e: ENGINE_PTR): EVP_PKEY_CTX_PTR;
    extern proc EVP_PKEY_keygen_init(ctx: EVP_PKEY_CTX_PTR): c_int;
    extern proc EVP_PKEY_CTX_set_rsa_keygen_bits(ctx: EVP_PKEY_CTX_PTR, mbits: c_int): c_int;
    extern proc EVP_PKEY_keygen(ctx: EVP_PKEY_CTX_PTR, ref ppkey: EVP_PKEY_PTR): c_int;
    extern proc EVP_PKEY_CTX_free(ctx: EVP_PKEY_CTX_PTR);

    extern proc EVP_SealInit(ref ctx: EVP_CIPHER_CTX, types: CONST_EVP_CIPHER_PTR,
                             ek: c_ptr(c_ptr(c_uchar)), ekl: c_ptr(c_int),
                             iv: c_ptr(c_uchar), pubk: c_ptr(EVP_PKEY_PTR), npubk: c_int): c_int;
    extern proc EVP_SealUpdate(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar),
                               outl: c_ptr(c_int), inp: c_ptr(c_uchar), inl: c_int): c_int;
    extern proc EVP_SealFinal(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar), outl: c_ptr(c_int)): c_int;

    extern proc EVP_OpenInit(ref ctx: EVP_CIPHER_CTX, types: CONST_EVP_CIPHER_PTR,
                             ek: c_ptr(c_uchar), ekl: c_int, iv: c_ptr(c_uchar), priv: EVP_PKEY_PTR): c_int;
    extern proc EVP_OpenUpdate(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar),
                               outl: c_ptr(c_int), inp: c_ptr(c_uchar), inl: c_int): c_int;
    extern proc EVP_OpenFinal(ref ctx: EVP_CIPHER_CTX, outm: c_ptr(c_uchar), outl: c_ptr(c_int)): c_int;

    extern type EVP_MD;
    extern type EVP_MD_CTX;
    extern type CHPL_EVP_MD_CTX;
    extern type ENGINE;

    extern type EVP_MD_PTR = c_ptr(EVP_MD);
    extern type EVP_MD_CTX_PTR = c_ptr(EVP_MD_CTX);
    extern type ENGINE_PTR = c_ptr(ENGINE);

    extern proc OpenSSL_add_all_digests();
    extern proc EVP_get_digestbyname(name: c_string): CONST_EVP_MD_PTR;

    extern proc CHPL_EVP_MD_CTX_new(): CHPL_EVP_MD_CTX;
    extern proc CHPL_EVP_MD_CTX_free(ref c: CHPL_EVP_MD_CTX);
    extern proc CHPL_EVP_MD_CTX_ptr(ref c: CHPL_EVP_MD_CTX):EVP_MD_CTX_PTR;
    extern proc EVP_DigestInit_ex(ctx: EVP_MD_CTX_PTR, types: CONST_EVP_MD_PTR, impl: ENGINE_PTR): c_int;
    extern proc EVP_DigestUpdate(ctx: EVP_MD_CTX_PTR, const d: c_void_ptr, cnt: size_t): c_int;
    extern proc EVP_DigestFinal_ex(ctx: EVP_MD_CTX_PTR, md: c_ptr(c_uchar), ref s: c_uint): c_int;

    extern type EVP_CIPHER;
    extern type EVP_CIPHER_CTX;
    extern type CHPL_EVP_CIPHER_CTX;

    extern type EVP_CIPHER_PTR = c_ptr(EVP_CIPHER);
    extern type EVP_CIPHER_CTX_PTR = c_ptr(EVP_CIPHER_CTX);

    extern proc RAND_bytes(buf: c_ptr(c_uchar), num: c_int) : c_int;

    extern proc EVP_sha256(): CONST_EVP_MD_PTR;

    extern proc PKCS5_PBKDF2_HMAC(pass: c_string,
                                  passlen: c_int,
                                  const salt: c_ptr(c_uchar),
                                  saltlen: c_int,
                                  iterCount: c_int,
                                  digest: CONST_EVP_MD_PTR,
                                  keylen: c_int,
                                  outx: c_ptr(c_uchar)): c_int;


    extern proc CHPL_EVP_CIPHER_CTX_new(): CHPL_EVP_CIPHER_CTX;
    extern proc CHPL_EVP_CIPHER_CTX_free(ref c: CHPL_EVP_CIPHER_CTX);
    extern proc CHPL_EVP_CIPHER_CTX_ptr(ref c: CHPL_EVP_CIPHER_CTX):EVP_CIPHER_CTX_PTR;
    extern proc EVP_EncryptInit_ex(ctx: EVP_CIPHER_CTX_PTR,
                                  cipher: CONST_EVP_CIPHER_PTR,
                                  impl: ENGINE_PTR,
                                  const key: c_ptr(c_uchar),
                                  const iv: c_ptr(c_uchar)): c_int;
    extern proc EVP_EncryptUpdate(ctx: EVP_CIPHER_CTX_PTR,
                                  outm: c_ptr(c_uchar),
                                  outl: c_ptr(c_int),
                                  const ins: c_ptr(c_uchar),
                                  inl: c_int): c_int;
    extern proc EVP_EncryptFinal_ex(ctx: EVP_CIPHER_CTX_PTR,
                                    outm: c_ptr(c_uchar),
                                    outl: c_ptr(c_int)): c_int;
    extern proc EVP_DecryptInit_ex(ctx: EVP_CIPHER_CTX_PTR,
                                  cipher: CONST_EVP_CIPHER_PTR,
                                  impl: ENGINE_PTR,
                                  const key: c_ptr(c_uchar),
                                  const iv: c_ptr(c_uchar)): c_int;
    extern proc EVP_DecryptUpdate(ctx: EVP_CIPHER_CTX_PTR,
                                  outm: c_ptr(c_uchar),
                                  outl: c_ptr(c_int),
                                  const ins: c_ptr(c_uchar),
                                  inl: c_int): c_int;
    extern proc EVP_DecryptFinal_ex(ctx: EVP_CIPHER_CTX_PTR,
                                    outm: c_ptr(c_uchar),
                                    outl: c_ptr(c_int)): c_int;

    extern proc EVP_aes_128_cbc(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_aes_128_ecb(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_aes_128_cfb(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_aes_128_ofb(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_aes_192_cbc(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_aes_192_ecb(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_aes_192_cfb(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_aes_192_ofb(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_aes_256_cbc(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_aes_256_ecb(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_aes_256_cfb(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_aes_256_ofb(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_bf_cbc(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_bf_ecb(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_bf_cfb(): CONST_EVP_CIPHER_PTR;
    extern proc EVP_bf_ofb(): CONST_EVP_CIPHER_PTR;

    extern proc RAND_seed(const buf: c_void_ptr, num: c_int);
  }
}
