var infile = new file("eof.in");
infile.open();

while !infile.eof {
  var s = infile.read(string);
  writeln(s);
}
