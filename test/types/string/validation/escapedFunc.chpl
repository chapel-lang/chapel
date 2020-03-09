var str = b"bAb\xffbAb".decode(policy=decodePolicy.escape);

// iterate over the string using indexing
var idx = 1;
while idx <= str.size {
  writeln("Char: ", str[idx]);
  // we get all replacement characters here. we should be getting the actual
  // escape codepoint
  writef("codepoint: %xu\n", str.codepoint[idx]);
  writef("repr: %ht\n", str[idx].encode(encodePolicy.unescape));
  idx += 1;

}

writeln("default iterations");
for s in str {
  writef("codepoint: %xu\n", s.toCodepoint());
  writef("repr: %ht\n", s.encode(encodePolicy.unescape));
}
writeln();

writeln("codepoint iterations");
for cp in str.codepoints() {
  writef("codepoint: %xu\n", cp);
}
writeln();

var strLower = str.toLower();
writeln("codepoint iterations for lower");
for cp in strLower.codepoints() {
  writef("codepoint: %xu\n", cp);
}
writeln();


writeln("find/count");
var strGood = "A";
var strBad = b"\xff".decode(policy=decodePolicy.escape);

writeln("Should be 2: ", str.count(strGood));
writeln("Should be 1: ", str.count(strBad));

writeln("Should be 2: ", str.find(strGood));
writeln("Should be 4: ", str.find(strBad));

writeln("Should be 8: ", str.rfind(strGood));
writeln("Should be 4: ", str.rfind(strBad));
writeln();

writeln("split");
for s in str.split(strBad) {
  writef("repr: %ht\n", s.encode(encodePolicy.unescape));
}
writeln();

for s in str.split(strGood) {
  writef("repr: %ht\n", s.encode(encodePolicy.unescape));
}
writeln();

