var mystring1 = "événement";

for c in mystring1.uchars() {
  const str = codePointToString(c);
  write(c, "\t", str);
  for i in 0..#str.length do
    write(" ", str.buff[i]:int);
  writeln();
}
