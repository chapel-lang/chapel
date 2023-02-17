use IO;

var infile = open("eof.in", ioMode.r).reader();

while !infile.eof {
  var s = infile.read(string);
  writeln(s);
}
