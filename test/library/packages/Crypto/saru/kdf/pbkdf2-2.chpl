proc main() {
  use Crypto;

  var hash = new Hash("SHA256");
  var k = new KDF(0, 1000, hash); // should halt
  var buf = new CryptoBuffer("random_salt");
  var key = k.passKDF("random_key", buf);
  writeln("Generated Key: ", key.toHex());

  delete key;
  delete buf;
  delete k;
  delete hash;
}
