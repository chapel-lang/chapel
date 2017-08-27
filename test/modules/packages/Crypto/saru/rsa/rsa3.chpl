proc main(){
  use Crypto;

  var r = new RSAKey(2048);

  var rx: [0..0] RSAKey; // with 0 starting domain domain
  rx[0] = r;

  var rsa = new RSA();
  var msg = new CryptoBuffer("hello");
  writeln("Message: " + msg.toHexString());
  var ep = rsa.encrypt(msg, rx);
  writeln("IV: " + ep.getIV().toHexString());
  writeln("EncMessage: " + ep.getEncMessage().toHexString());
  writeln("EncKey: " + ep.getEncKeyByIndex(0).toHexString()); //should pass because domain is now from 0
  var ms = rsa.decrypt(ep, r);
  writeln("Decrypted Message: " + ms.toHexString());
}
