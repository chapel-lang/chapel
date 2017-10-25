proc main() {
  use Crypto;

  var bf = new Blowfish("cbc");

  var msg = new CryptoBuffer("hello world");

  // static values assumed for testing purposes
  var iv = new CryptoBuffer("iv");
  var key = new CryptoBuffer("key");

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
