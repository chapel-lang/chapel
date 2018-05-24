proc main(){
  use Crypto;

  var SHA = new unmanaged Hash(Digest.SHA1);
  var SHA2 = new unmanaged Hash(Digest.SHA256);
  var s = "The quick brown fox jumps over the lazy dog";
  writeln(s);

  var buf = new unmanaged CryptoBuffer(s);
  var digest = SHA.getDigest(buf);
  writeln(SHA.getDigestName() , " = " , digest.toHex());

  delete digest;
  delete buf;

  buf = new unmanaged CryptoBuffer(s);
  digest = SHA2.getDigest(buf);
  writeln(SHA2.getDigestName() , " = " , digest.toHex());

  delete digest;
  delete buf;
  delete SHA2;
  delete SHA;
}
