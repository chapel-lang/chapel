use IO;

var infile = open("eof.in", ioMode.r).reader(locking=false);

var s:string;
while infile.read(s) {
  writeln(s);
}

