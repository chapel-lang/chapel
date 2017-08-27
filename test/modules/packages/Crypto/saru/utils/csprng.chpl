proc main(){
  use Crypto;

  var r = new CryptoRandom();
  var rBuff = r.createRandomBuffer(20);
  writeln(rBuff.toHex());
}
