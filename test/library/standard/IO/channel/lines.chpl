use IO;

config param locking=true,
             ioKind=iokind.dynamic;

var c = openReader('lines.good', kind=ioKind, locking=locking);

for line in c.lines() {
  write(line);
}

