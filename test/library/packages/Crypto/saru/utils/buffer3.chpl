proc main() {
  use Crypto;
  var a = new unmanaged CryptoBuffer(""); // doesn't do anything
  writeln(a.toHex());
  delete a;
}
