var myBytes = b"abc\xffa";
var myString = myBytes.decode(errors=decodePolicy.escape);
writeln("Escaped string numBytes: ", myString.numBytes);
writeln("Escaped string bytes: ");
for b in myString.bytes() {
  writeln(b);
}
writeln();
writeln("Unescaped string bytes: ");
for b in myString.encode().bytes() {
  writeln(b);
}
writeln();

writeln("Base bytes: ");
for b in myBytes.bytes() {
  writeln(b);
}
writeln();

writeln(myBytes == myString.encode());

