use IO;

var filename = "linesLimited.txt";
var f = open(filename, iomode.r);

for line in f.lines(region=-1..) {
  write(line);
}
f.close();
