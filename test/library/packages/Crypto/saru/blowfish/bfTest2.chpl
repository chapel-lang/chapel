// test using KDF

proc main(){
  use Crypto;

  var a = new owned Blowfish(CryptoChainMode.cbc);

  /* Key Generation phase starts */
  var salt = new owned CryptoBuffer("random_salt");
  var hash = new owned Hash(Digest.SHA256);
  var k = new owned KDF(16, 1000, hash);
  var key = k.passKDF("random_key", salt);
  writeln("Generated Key: ", key.toHex());
  /* Key Generation phase ends */

  /* IV is manipulated to return the same encryption on every run (for testing purposes) */
  var iv = new owned CryptoBuffer("iv123456");
  writeln("Generated IV: ", iv.toHex());

  /* The message to be encrypted */
  var msg = new owned CryptoBuffer("test string");
  writeln("Original Message: ", msg.toHex());

  /* Encrypt the message using the key and IV */
  var ct = a.encrypt(msg, key, iv);
  writeln("Obtained Ciphertext: ", ct.toHex());

  /* Decrypt the message using the key and IV */
  var orig = a.decrypt(ct, key, iv);
  writeln("Obtained Plaintext: ", orig.toHex());
}
