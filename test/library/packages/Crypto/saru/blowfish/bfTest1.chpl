proc main() {
  use Crypto;

  var bf = new Blowfish("cbc");

  var msg = new CryptoBuffer("hello world");

  // static values assumed for testing purposes
  var iv = new CryptoBuffer("12345678"); // should be exactly 8 bytes
  var key = new CryptoBuffer("long keys are the best");

  writeln("MSG: ", msg.toHex());

  var ct = bf.encrypt(msg, key, iv);
  writeln("CT: ", ct.toHex());

  var pt = bf.decrypt(ct, key, iv);
  writeln("PT: ", pt.toHex());

  delete pt;
  delete ct;
  delete key;
  delete iv;
  delete msg;
  delete bf;
}
