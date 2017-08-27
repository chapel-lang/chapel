proc main() {
  use Crypto;

  var k = new KDF(0, 1000, new Hash("SHA256")); // should halt
  var key = k.PBKDF2_HMAC("random_key", new CryptoBuffer("random_salt"));
  writeln("Generated Key: ", key.toHex());
}
