use IO;

config param locking=true;

var c = openReader('lines.good', locking=locking);

for line in c.lines() {
  write(line);
}

