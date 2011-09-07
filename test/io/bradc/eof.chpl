var infile = open("eof.in", "r").reader();

/*
while !infile.eof {
  var s = infile.read(string);
  writeln(s);
}*/

var s:string;
while infile.read(s) {
  writeln(s);
}

