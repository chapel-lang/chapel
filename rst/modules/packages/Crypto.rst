.. default-domain:: chpl

.. module:: Crypto
   :synopsis: A cryptographic library based on `OpenSSL <https://www.openssl.org/>`_, targeted at

Crypto
======
**Usage**

.. code-block:: chapel

   use Crypto;

**Submodules**

.. toctree::
   :maxdepth: 1
   :glob:

   Crypto/*

A cryptographic library based on `OpenSSL <https://www.openssl.org/>`_, targeted at
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

.. class:: CryptoBuffer

   The `CryptoBuffer` class is a wrapper around the internal representation
   of how the values in this library are stored. Every sequence of bytes going
   into a Crypto utility or coming out of it, is a `CryptoBuffer`.
   
   A `CryptoBuffer` can enclose a `string` or a `[] uint(8)` passed to its
   initializer and provides helper functions to access those values.
   
   


   .. method:: proc init(s: string)

      The `CryptoBuffer` class initializer that initializes the buffer
      when a `string` is supplied to it.
      
      :arg s: `string` input for buffer conversion.
      :type s: `string`
      
      :return: An object of class `CryptoBuffer`.
      :rtype: `CryptoBuffer`
      
      

   .. method:: proc init(s: [] uint(8))

      The `CryptoBuffer` class initializer that initializes the buffer
      when a `[] uint(8)` is supplied to it.
      
      :arg s: `[] uint(8)` input for buffer conversion.
      :type s: `[] uint(8)`
      
      :return: An object of class `CryptoBuffer`.
      :rtype: `CryptoBuffer`
      
      

   .. method:: proc getBuffData()

      Returns the entire internal buffer with each byte in ASCII.
      
      :return: the internal buffer representation.
      :rtype: `[] uint(8)`
      
      

   .. method:: proc getBuffPtr()

      Returns the pointer to the entire internal buffer with
      each byte in ASCII.
      
      :return: pointer to the internal buffer representation.
      :rtype: `c_ptr([] uint(8))`
      
      

   .. method:: proc getBuffSize(): int

      Returns the length of the entire internal buffer.
      
      :return: length of the internal buffer representation.
      :rtype: `int`
      
      

   .. method:: proc toHex() throws

      Returns the hexadecimal array representation of the entire internal buffer.
      
      :return: hex array representation of the internal buffer.
      :rtype: `[] string`
      
      

   .. method:: proc toHexString() throws

      Returns the hexadecimal string representation of the entire internal buffer.
      
      :return: hex string representation of the internal buffer.
      :rtype: `string`
      
      

.. class:: RSAKey

   `RSAKey` class encloses the `EVP_PKEY` object provided by the
   OpenSSL primitives. The `EVP_PKEY` object can contain the public key,
   private key or both of them. Hence, the contents of an object of the
   class `RSAKey` may be decided by the user.
   
   Calling the `RSAKey` initializer without using any key import or export
   functions may result in generation of a single object that contains both the
   keys (public and private).
   
   In order to separate out the key objects, keys can be imported
   from a `.pem` file. (TODO)
   
   


   .. method:: proc init(keyLen: int)

      The `RSAKey` class initializer that initializes the `EVP_PKEY` object
      of OpenSSL and basically, initializes a set of public and private keys.
      
      It checks for valid RSA key lengths and generates a public key and private
      key pair accordingly.
      
      :arg keyLen: RSA Key length in bits.
      :type keyLen: `int`
      
      :return: An object of class `RSAKey` representing the key pair.
      :rtype: `RSAKey`
      
      

.. class:: Envelope

   The `Envelope` class wraps all the data returned by the encrypt function
   of the `RSA` class along with some utility data. An `RSA` encrypt function
   returns an array of `RSA` encrypted symmetric keys and a single `AES`
   encrypted message. The `Envelope` also encloses the initialization vector
   used during encryption such that it can be utilized during the decryption
   phase as well.
   
   


   .. method:: proc init(iv: owned CryptoBuffer, encSymmKey: [] owned CryptoBuffer, encSymmValue: owned CryptoBuffer)

      The `Envelope` class initializer that encapsulates the IV, AES encrypted
      ciphertext buffer and an array of encrypted key buffers.
      
      :arg iv: Initialization Vector.
      :type iv: `owned CryptoBuffer`
      
      :arg encSymmKey: Array of encrypted symmetric (AES) keys.
      :type encSymmKey: `[] owned CryptoBuffer`
      
      :arg encSymmValue: AES-encrypted ciphertext buffer.
      :type encSymmValue: `owned CryptoBuffer`
      
      :return: An object of class `Envelope`.
      :rtype: `Envelope`
      
      

   .. method:: proc getEncMessage()

      This function returns the encrypted version of the plaintext
      supplied by the user.
      
      :return: A 'CryptoBuffer' representing the ciphertext.
      :rtype: `CryptoBuffer`
      
      

   .. method:: proc getIV()

      This function returns the IV generated by the `encrypt` routine and
      encapsulated in the `Envelope`. This is used for both encryption and
      decryption.
      
      :return: Initialization Vector.
      :rtype: `CryptoBuffer`
      
      

   .. method:: proc getEncKeyByIndex(i: int)

      This function returns a particular symmetric key buffer based
      on the index supplied as the argument.
      
      .. note::
      
        The supplied index should be in the domain of the RSA-encrypted keys array.
      
      :arg i: An index of the symmetric key buffer array.
      :type i: `int`
      
      :return: A specific key buffer based on the index.
      :rtype: `CryptoBuffer`
      
      

   .. method:: proc getEncKeys()

      This function returns the entire array of symmetric key buffers.
      
      :return: Entire array of key buffers.
      :rtype: `[] CryptoBuffer`
      
      

.. enum:: enum Digest { MD5, SHA1, SHA224, SHA256, SHA384, SHA512, RIPEMD160 }

   The `Digest` enum represents all the hashing functions provided by the
   OpenSSL primitives. Value from this enum is passed to the `Hash` class
   initializer in order to select the type of hashing function to be used.
   
   


.. class:: Hash

   The `Hash` class represents all the hashing functions provided by the
   OpenSSL primitives. It supports all the prominent and most commonly used
   deterministic hashing functions.
   
   


   .. method:: proc init(digestName: Digest)

      The `Hash` class initializer that initializes the hashing function
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
      
      

   .. method:: proc getDigestName(): string

      Returns the name of the hashing function used. For instance,
      instantiating an object with ``var hash = new Hash("SHA256")``,
      would make this function return the string `SHA256`.
      
      :return: Name of the hashing function.
      :rtype: `string`
      
      

   .. method:: proc getDigest(inputBuffer: CryptoBuffer): owned CryptoBuffer

      Given a CryptoBuffer (buffer) as input, this function returns it's
      hash digest. The returned hash digest is also a buffer that can be
      accessed by using buffer utility functions.
      
      :arg inputBuffer: A `CryptoBuffer` representing the input to be hashed.
      :type inputBuffer: `CryptoBuffer`
      
      :return: An `owned CryptoBuffer` representing the hash digest.
      :rtype: `owned CryptoBuffer`
      
      

.. enum:: enum CryptoChainMode { cbc, ecb, cfb, ofb }

   The `CryptoChainMode` enum represents all cipher chaining modes
   that can be used by a symmetric cipher. It is used by the `AES` and
   `Blowfish` class initializers to select a chaining mode.
   
   


.. class:: AES

   The `AES` class represents the symmetric encryption algorithm, AES.
   The Advanced Encryption Standard (AES), also known by its original name Rijndael
   is a specification for the encryption of electronic data established by the
   U.S. National Institute of Standards and Technology (NIST) in 2001.
   
   It is the most widely used symmetric cipher and is also used to encrypt
   messages in public-key cryptography such as within `RSA`.
   
   Currently, the `AES` class allows symmetric encryption using only the CBC or
   Cipher Block Chaining mode in 128, 192, and 256 key size variants.
   
   After thorough testing, ECB, OCB and other chaining mode variants will also
   be added to this library(TODO).
   
   


   .. method:: proc init(bits: int, mode: CryptoChainMode)

      The `AES` class initializer that initializes the AES encryption
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
      
      

   .. method:: proc getByteSize(): int

      This function returns the size in bytes of the key-length/variant of
      AES which is to be used. For instance,
      
      - AES128 returns 16
      - AES192 returns 24
      - AES256 returns 32
      
      :return: Key length of AES in bytes.
      :rtype: `int`
      
      

   .. method:: proc encrypt(plaintext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer): owned CryptoBuffer

      This is the 'AES' encrypt routine that encrypts the user supplied message buffer
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
      
      

   .. method:: proc decrypt(ciphertext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer): owned CryptoBuffer

      This is the 'AES' decrypt routine that decrypts the user supplied ciphertext
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
      
      

.. class:: Blowfish

   The `Blowfish` class represents a symmetric-key block cipher called Blowfish, designed
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
   
   


   .. method:: proc init(mode: CryptoChainMode)

      The `Blowfish` class initializer that initializes the Blowfish encryption
      algorithm with the right key length and chaining mode.
      
      :arg mode: Name of the chaining mode to be used.
      :type mode: `CryptoChainMode`
      
      :return: An object of class `Blowfish`.
      :rtype: `Blowfish`
      
      Initialization example,
      
      .. code-block:: chapel
      
         var bf = new Blowfish(CryptoChainMode.cbc);
      
      

   .. method:: proc encrypt(plaintext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer): owned CryptoBuffer throws

      This is the 'Blowfish' encrypt routine that encrypts the user supplied message buffer
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
      
      

   .. method:: proc decrypt(ciphertext: CryptoBuffer, key: CryptoBuffer, IV: CryptoBuffer): owned CryptoBuffer

      This is the 'Blowfish' decrypt routine that decrypts the user supplied ciphertext
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
      
      

.. class:: CryptoRandom

   The `CryptoRandom` class represents a CSPRNG provided by OpenSSL that
   automatically does the seeding part before returning a random sequence
   of bytes of the type `CryptoBuffer`.
   
   Given the length of the buffer required by the user, it generates a random
   buffer of the same size.
   
   


   .. method:: proc getRandomBuffer(buffLen: int): owned CryptoBuffer throws

      This function represents a CSPRNG that generates and allocates the desired
      number of random values as specified by the argument. Halts for number of
      bytes less than 1 (invalid). For instance,
      
      .. code-block:: chapel
      
         var a = (new CryptoRandom()).getRandomBuffer(5)
      
      would give us a `CryptoBuffer` of size `5` and pre-initialized with values.
      
      :arg buffLen: Number of random values to be generated in the buffer.
      :type buffLen: `int`
      
      :return: An `owned CryptoBuffer` representing the generated values.
      :rtype: `owned CryptoBuffer`
      
      

.. class:: KDF

   The `KDF` class contains the most widely used key derivation functions.
   Currently it supports a single password based key derivation function which
   is the most secure one as of writing this library. More KDFs will be added
   to this in the future.
   
   


   .. method:: proc init(byteLen: int, iterCount: int, digest: Hash)

      The `KDF` class initializer that initializes the common data used by most
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
      
      

   .. method:: proc passKDF(userKey: string, saltBuff: CryptoBuffer): owned CryptoBuffer

      This function represents Password-Based KDF 2. It generates a secure-key
      buffer based on the salt and also on the metadata provided in the `KDF`
      initializer.
      
      :arg userKey: User-specified `string` representation of the key.
      :type userKey: `string`
      
      :arg saltBuff: A `CryptoBuffer` representing the user generated salt.
      :type saltBuff: `CryptoBuffer`
      
      :return: An `owned CryptoBuffer` representing the generated key.
      :rtype: `owned CryptoBuffer`
      
      

.. class:: RSA

   RSA is one of the first practical public-key cryptosystems and is widely
   used for secure data transmission. A user of the `RSA` first generates the
   public and private keys using the `RSAKey` class. The `RSAKey` object is
   then passed to the `RSA` functions for further encryption and decryption
   purposes.
   
   The `encrypt` function of this class takes in an array of `RSAKey` objects
   and uses all of them to perform the encryption.
   
   Similarly, decryption can be done individually performed (without passing an array
   as an argument) by passing a single `RSAKey` object every time the decryption takes place.
   
   


   .. method:: proc encrypt(plaintext: CryptoBuffer, keys: [] RSAKey): owned Envelope

      This is the 'RSA' encrypt routine that encrypts the plaintext buffer. This
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
      
      

   .. method:: proc decrypt(envp: Envelope, key: RSAKey): owned CryptoBuffer throws

      This is the 'RSA' decrypt routine that decrypts the ciphertext buffer. This
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
      
      

