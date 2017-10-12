proc main(){
  use Crypto;

  var SHA = new Hash("SHA1024"); //should halt
  var s = "The quick brown fox jumps over the lazy dog";
  writeln(s);

  var buf = new CryptoBuffer(s);
  var digest = SHA.getDigest(buf);
  writeln(SHA.getDigestName() + " = " + digest.toHexString());

  delete digest;
  delete buf;
  delete SHA;
}
