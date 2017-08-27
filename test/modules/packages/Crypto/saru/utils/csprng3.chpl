proc main(){
  use Crypto;

  var r = new CryptoRandom();
  var rBuff = r.createRandomBuffer(-100); // halts with invald buffer length
  writeln(rBuff.toHex());
}
