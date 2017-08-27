proc main(){
  use Crypto;

  var r = new RSAKey(2048);

  var rsa = new RSA();
  var msg = new CryptoBuffer("hello");
  writeln("Message: " + msg.toHexString());
  var ep = rsa.encrypt(msg, [r]);
  writeln("IV: " + ep.getIV().toHexString());
  writeln("EncMessage: " + ep.getEncMessage().toHexString());
  writeln("EncKey: " + ep.getEncKeyByIndex(0).toHexString()); //should fail due to out of bounds index
  var ms = rsa.decrypt(ep, r);
  writeln("Decrypted Message: " + ms.toHexString());
}
