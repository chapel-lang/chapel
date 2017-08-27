proc main(){
  use Crypto;

  var SHA = new Hash("SHA1024"); //should halt
  var s = "The quick brown fox jumps over the lazy dog";
  writeln(s);

  var digest = SHA.getDigest(new CryptoBuffer(s));
  writeln(SHA.getDigestName() + " = " + digest.toHexString());
}
