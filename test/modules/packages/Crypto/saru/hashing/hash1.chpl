proc main(){
  use Crypto;

  var SHA = new Hash("SHA1");
  var SHA2 = new Hash("SHA256");
  var s = "The quick brown fox jumps over the lazy dog";
  writeln(s);

  var digest = SHA.getDigest(new CryptoBuffer(s));
  writeln(SHA.getDigestName() + " = " + digest.toHexString());
  digest = SHA2.getDigest(new CryptoBuffer(s));
  writeln(SHA2.getDigestName() + " = " + digest.toHexString());
}
