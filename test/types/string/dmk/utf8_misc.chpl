var mystring1: string = "hello";
var blen1 = mystring1.numBytes;

writeln("Byte length = ", mystring1.numBytes);

for i in 0..#blen1 {
  writeln("Byte index ", i, " = '", mystring1[i: byteIndex], "'");
}

var mystring2: string = "événement";
var cplen2 = mystring2.numCodepoints;

writeln("Codepoint length = ", cplen2);

for i in 0..#cplen2 {
  writeln("Codepoint index ", i, " = '", mystring2[i: codepointIndex], "'");
}
