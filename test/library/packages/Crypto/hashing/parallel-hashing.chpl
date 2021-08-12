config const n = 1000;

// based on test from issue #11366

proc main() {
  use Crypto;

  var SHA2 = new owned Hash(Digest.SHA256);
  var s = "The quick brown fox jumps over the lazy dog";
  writeln(s);

  var buf = new owned CryptoBuffer(s);

  var digests:[1..n] owned CryptoBuffer?;
  forall i in 1..n {
    digests[i] = SHA2.getDigest(buf);
  }

  assert(n >= 1);
  var hash = digests[1]!.toHexString();
  for i in 1..n {
    assert(hash == digests[i]!.toHexString());
  }
  writeln(hash);
}
