var mystring1: string = "événement";
var ulen1 = mystring1.ulength;

writeln("Byte length = ", mystring1.length);
writeln("Codepoint length = ", ulen1);

for i in 1..ulen1 {
  writeln("Codepoint index ", i, " = '",
          codePointToString(mystring1[i: codePointIndex]), "'");
}
