proc main(){
  use Crypto;

  var SHA = new owned Hash(Digest.SHA1);
  var SHA2 = new owned Hash(Digest.SHA256);
  var s = "The quick brown fox jumps over the lazy dog";
  writeln(s);

  var buf = new owned CryptoBuffer(s);
  var digest = SHA.getDigest(buf);
  writeln(SHA.getDigestName() , " = " , digest.toHex());

  buf = new owned CryptoBuffer(s);
  digest = SHA2.getDigest(buf);
  writeln(SHA2.getDigestName() , " = " , digest.toHex());
}
