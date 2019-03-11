// string.uchars(), string.ulength, codePointIndex, and codePointToString()
// are deprecated

var str = "deprecate me";

for i in str.uchars() do
  writeln(i);

writeln(str.ulength);

writeln(str[1: codePointIndex]);

writeln(codePointToString(str.codepoint(1)));
