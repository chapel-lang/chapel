// string.uchars(), string.ulength, and codePointToString are deprecated
for i in "deprecate me".uchars() do
  writeln(i);

writeln("deprecate me".ulength);

writeln(codePointToString("deprecate me".codepoint(1)));
