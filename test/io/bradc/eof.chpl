use IO;

var infile = open("eof.in", ioMode.r).reader();

var s:string;
while infile.read(s) {
  writeln(s);
}

