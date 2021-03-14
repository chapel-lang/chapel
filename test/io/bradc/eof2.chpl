use IO;

var infile = open("eof.in", iomode.r).reader();

while !infile.eof {
  var s = infile.read(string);
  writeln(s);
}
