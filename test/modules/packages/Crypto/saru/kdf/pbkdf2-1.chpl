proc main(){
  use Crypto;

  var k = new KDF(32, 1000, new Hash("SHA256"));
  var key = k.PBKDF2_HMAC("random_key", new CryptoBuffer("random_salt"));
  writeln("Generated Key: ", key.toHex());
}
