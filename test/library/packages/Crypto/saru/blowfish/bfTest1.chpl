proc main() {
  use Crypto;

  var bf = new owned Blowfish(CryptoChainMode.cbc);

  var msg = new owned CryptoBuffer("hello world");

  // static values assumed for testing purposes
  var iv = new owned CryptoBuffer("12345678"); // should be exactly 8 bytes
  var key = new owned CryptoBuffer("long keys are the best");

  writeln("MSG: ", msg.toHex());

  var ct = bf.encrypt(msg, key, iv);
  writeln("CT: ", ct.toHex());

  var pt = bf.decrypt(ct, key, iv);
  writeln("PT: ", pt.toHex());
}
