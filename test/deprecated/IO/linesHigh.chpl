use IO;

var filename = "linesHigh.txt";
var f = open(filename, ioMode.r);

for line in f.lines(region=..53) {
  write(line);
}

for line in f.lines(region=27..53) {
  write(line);
}

f.close();
