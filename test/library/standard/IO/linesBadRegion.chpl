use IO;

var filename = "linesLimited.txt";
var f = open(filename, ioMode.r);

for line in f.lines(region=-1..) {
  write(line);
}
f.close();
