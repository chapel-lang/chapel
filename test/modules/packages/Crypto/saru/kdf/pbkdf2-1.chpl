proc main(){
  use Crypto;

  var k = new KDF(32, 1000, new Hash("SHA256"));
  var key = k.passKDF("random_key", new CryptoBuffer("random_salt"));
  writeln("Generated Key: ", key.toHex());
}
