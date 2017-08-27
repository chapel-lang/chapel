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
  require "openssl/evp.h", "-lcrypto";
  require "openssl/aes.h", "openssl/rand.h";

  require "CryptoSupport/hashSupport.chpl";
  require "CryptoSupport/aesSupport.chpl";
  require "CryptoSupport/kdfSupport.chpl";
  require "CryptoSupport/CryptoUtils.chpl";
  require "CryptoSupport/cryptoRandomSupport.chpl";
  require "CryptoSupport/rsaSupport.chpl";

  use kdfSupport;
  use aesSupport;
  use hashSupport;
  use CryptoUtils;
  use cryptoRandomSupport;
  use symmetricPrimitives;
  use asymmetricPrimitives;
  use rsaSupport;


  class Hash {
    var hashLen: int;
    var digestName: string;
    var hashDomain: domain(1);
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
    proc Hash(digestName: string) {
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
      this.hashDomain = {0..this.hashLen-1};
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
      this.hashSpace = hashSupport.digestPrimitives(this.digestName, this.hashLen, inputBuffer);
      var hashBuffer = new CryptoBuffer(this.hashSpace);
      return hashBuffer;
    }
  }



  class AES {
    const cipher: symmetricPrimitives.EVP_CIPHER_PTR;
    var bitLen: int;

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
    proc AES(bits: int, mode: string) {
      if (bits == 128 && mode == "cbc") {
        this.cipher = symmetricPrimitives.EVP_aes_128_cbc();
      } else if (bits == 192 && mode == "cbc") {
        this.cipher = symmetricPrimitives.EVP_aes_192_cbc();
      } else if (bits == 256 && mode == "cbc") {
        this.cipher = symmetricPrimitives.EVP_aes_256_cbc();
      } else {
        halt("The desired variant of AES does not exist.");
      }
      this.bitLen = bits/8;
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
        return this.bitLen;
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
      var encryptedPlaintext = aesSupport.aesEncrypt(plaintext, key, IV, this.cipher);
      var encryptedPlaintextBuff = new CryptoBuffer(encryptedPlaintext);
      return encryptedPlaintextBuff;
    }

    /* This is the 'AES' decrypt routine that decrypts the user supplied ciphertext
       buffer using the same key and IV used for encryption.
       The `decrypt` takes in the ciphertext buffer, key buffer and IV buffer as the
       arguments and returns a buffer of the decrypted plaintext.

       :arg ciphertext: A `CryptoBuffer` representing the ciphertext to be decrypted.
       :type ciphertext: `CryptoBuffer`

       :arg key: A `CryptoBuffer` representing the key to be used for decryption
                 (same as the one used in ecryption).
       :type key: `CryptoBuffer`

       :arg IV: A `CryptoBuffer` representing the initialization vector to be used
                for decryption (same as the one used in ecryption).
       :type IV: `CryptoBuffer`

       :return: A `CryptoBuffer` representing the obtained plaintext.
       :rtype: `CryptoBuffer`
    */
    proc decrypt(ciphertext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer): CryptoBuffer {
      var decryptedCiphertext = aesSupport.aesDecrypt(ciphertext, key, IV, this.cipher);
      var decryptedCiphertextBuff = new CryptoBuffer(decryptedCiphertext);
      return decryptedCiphertextBuff;
    }
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
      var randomizedBuff = cryptoRandomSupport.createRandomBuffer(buffLen);
      var randomizedCryptoBuff = new CryptoBuffer(randomizedBuff);
      return randomizedCryptoBuff;
    }
  }

  class KDF {
    var bitLen: int;
    var iterCount: int;
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
    proc KDF(bitLen: int, iterCount: int, digest: Hash) {
      if (bitLen < 1) {
        halt("Invalid key size.");
      }
      this.bitLen = bitLen;
      this.iterCount = iterCount;
      this.hashName = digest.getDigestName();
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
    proc PBKDF2_HMAC(userKey: string, saltBuff: CryptoBuffer): CryptoBuffer {
      var key = kdfSupport.PBKDF2_HMAC(userKey, saltBuff, this.bitLen, this.iterCount, this.hashName);
      var keyBuff = new CryptoBuffer(key);
      return keyBuff;
    }
  }

  class RSA {
    proc RSA() {}

    /* This is the 'RSA' encrypt routine that encrypts the plaintext buffer. This
       uses the `AES` encryption algorithm to encrypt the plaintext and auto-generates
       a secure IV and key. Furthermore, the key is encrypted using `RSA` based on the
       'RSAKey' objects supplied in the arguments.

       The function returns en `Envelope` object that encloses the auto-generated
       IV, AES encrypted ciphertext and an array of `RSA` encrypted key buffers.
       The numer of encrypted keys is equal to the number of `RSAKey` objects
       in the array. This kind of use case, is useful specifically in developing
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

       :return: An `Envelope` object which comprises of the IV buffer, array of
                RSA encryted keys and AES encrypted ciphertext.
       :rtype: `Envelope`
    */
    proc encrypt(plaintext: CryptoBuffer, keys: [] RSAKey): Envelope {
      var ivLen = asymmetricPrimitives.EVP_CIPHER_iv_length(asymmetricPrimitives.EVP_aes_256_cbc());
      var iv: [0..((ivLen - 1): int(64))] uint(8);

      var encSymmKeys: [keys.domain] CryptoBuffer;

      var ciphertext = rsaSupport.rsaEncrypt(keys, plaintext, iv, encSymmKeys);

      var envp = new Envelope(new CryptoBuffer(iv), encSymmKeys, new CryptoBuffer(ciphertext));
      return envp;
    }

    /* This is the 'RSA' decrypt routine that decrypts the ciphertext buffer. This
       uses `AES` decryption to decrypt the ciphertext.
       It's accepts the entire `Envelope` object but only a single `RSAKey` object
       for unilateral decryption.

       :arg envp: A `CryptoBuffer` representing the plaintext to be
                       encrypted.
       :type envp: `Envelope`

       :arg key: An array of `RSAKey` objects. This depends on the number of
                  different RSA encryptions that are to be performed on the same
                  plaintext.
       :type key: `RSAKey`

       :return: An `Envelope` object which comprises of the IV buffer, array of
                RSA encryted keys and AES encrypted ciphertext.
       :rtype: `CryptoBuffer`
    */
    proc decrypt(envp: Envelope, key: RSAKey): CryptoBuffer {
      var iv = envp.getIV().getBuffData();
      var ciphertext = envp.getEncMessage().getBuffData();
      var encKeys = envp.getEncKeys();

      var plaintext = rsaSupport.rsaDecrypt(key, iv, ciphertext, encKeys);
      var plaintextBuff = new CryptoBuffer(plaintext);
      return plaintextBuff;
    }
  }
}
