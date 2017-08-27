proc main(){
  use Crypto;

  var r = new CryptoRandom();
  var rBuff = r.createRandomBuffer(0); // halts with invald buffer length
  writeln(rBuff.toHex());
}
