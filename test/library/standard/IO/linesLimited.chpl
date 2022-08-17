use IO;

var filename = "linesLimited.txt";
var f = open(filename, iomode.r);

for line in f.lines() {
  write(line);
}

for line in f.lines(region=27..) {
  write(line);
}

for line in f.lines(region=..53) {
  write(line);
}

for line in f.lines(region=27..53) {
  write(line);
}

f.close();
