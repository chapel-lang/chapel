var mystring1 = "hello";

for (c, i) in zip(mystring1.bytes(), 0..) {
  const str = codepointToString(c);
  write(c);
  var c2 = mystring1.byte(i);
  if c2 != c then
    write("#", c2);
  write("\t", str);
  for (bval, j) in zip(str.bytes(), 0..) {
    write(" ", bval);
    var bval2 = str.byte(j);
    if bval2 != bval then
      write("#", bval2);
  }
  writeln();
}

var mystring2 = "événement";

for (c, i) in zip(mystring2.codepoints(), 0..) {
  const str = codepointToString(c);
  write(c);
  var c2 = mystring2.codepoint(i);
  if c2 != c then
    write("#", c2);
  write("\t", str);
  for (bval, j) in zip(str.bytes(), 0..) {
    write(" ", bval);
    var bval2 = str.byte(j);
    if bval2 != bval then
      write("#", bval2);
  }
  writeln();
}
