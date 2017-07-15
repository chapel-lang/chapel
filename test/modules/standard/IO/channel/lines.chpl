use IO;

var c = open('lines.good', iomode.r);

for line in c.lines() {
  write(line);
}

