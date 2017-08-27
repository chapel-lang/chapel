proc main(){
  use Crypto;

  var r = new RSAKey(1024);

  var rsa = new RSA();
  var msg = new CryptoBuffer("hello");
  writeln("Message: " + msg.toHexString());
  var ep = rsa.encrypt(msg, [r]);
  writeln("IV: " + ep.getIV().toHexString());
  writeln("EncMessage: " + ep.getEncMessage().toHexString());
  writeln("EncKey: " + ep.getEncKeyByIndex(1).toHexString());
  var ms = rsa.decrypt(ep, r);
  writeln("Decrypted Message: " + ms.toHexString());
}
