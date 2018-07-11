proc main(){
  use Crypto;

  /* Array to buffer */
  var arr: [0..4] uint(8) = [1, 2, 3, 4, 5]; // fails with error: type mismatch in assignment from int(64) to uint(8)
  var c = new unmanaged CryptoBuffer(arr);
  writeln(c.toHex());
  writeln(c.toHexString());
  writeln(c.getBuffData());
  writeln(c.getBuffSize());
  delete c;
}
