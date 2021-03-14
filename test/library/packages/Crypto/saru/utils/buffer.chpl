proc main(){
  use Crypto;

  /* String to buffer */
  var b = new unmanaged CryptoBuffer("foobar");
  writeln(b.toHex());
  writeln(b.toHexString());
  writeln(b.getBuffData());
  writeln(b.getBuffSize());

  delete b;

  /* Array to buffer */
  var arr: [0..4] uint(8) = [1: uint(8), 2: uint(8), 3: uint(8), 4: uint(8), 5: uint(8)];
  var c = new unmanaged CryptoBuffer(arr);
  writeln(c.toHex());
  writeln(c.toHexString());
  writeln(c.getBuffData());
  writeln(c.getBuffSize());

  delete c;
}
