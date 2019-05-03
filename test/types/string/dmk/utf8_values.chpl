var mystring1 = "événement";

for (i, c) in zip(1.., mystring1.codepoints()) {
  const str = codepointToString(c);
  write(c);
  var c2 = mystring1.codepoint(i);
  if c2 != c then
    write("#", c2);
  write("\t", str);
  for (j, bval) in zip(1.., str.bytes()) {
    write(" ", bval);
    var bval2 = str.byte(j);
    if bval2 != bval then
      write("#", bval2);
  }
  writeln();
}
