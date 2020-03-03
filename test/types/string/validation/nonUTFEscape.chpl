var myBytes = b"abc\xffa";

writeln("Base bytes: ");
for b in myBytes.bytes() {
  writeln(b);
}
writeln();

var myString = myBytes.decode(errors=decodePolicy.escape);
writeln("Escaped string bytes: ");
for b in myString.bytes() {
  writef("Decimal: %u, Hexadecimal: %xu\n", b, b);
}
writeln();

writeln("Direct copy string bytes: ");
var directBytes = myString.encode(errors=encodePolicy.pass);
for b in directBytes.bytes() {
  writeln(b);
}
writeln();

writeln("Unescaped string bytes: ");
var unescapedBytes = myString.encode(errors=encodePolicy.unescape);
for b in unescapedBytes.bytes() {
  writeln(b);
}
writeln();

writeln("Should be false: ", myBytes == directBytes);
writeln("Should be true: ", myBytes == unescapedBytes);

