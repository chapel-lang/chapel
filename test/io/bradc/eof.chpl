var infile = open("eof.in", mode.r).reader();

var s:string;
while infile.read(s) {
  writeln(s);
}

