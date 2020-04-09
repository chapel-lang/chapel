use IO;

var infile = open("eof.in", iomode.r).reader();

var s:string;
while infile.read(s) {
  writeln(s);
}

