use IO except openreader; // To verify deprecation message for openreader
// Was originally test/library/standard/IO/channel/lines.chpl

config param locking=true,
             ioKind=iokind.dynamic;

var c = openreader('helper.txt', kind=ioKind, locking=locking);

for line in c.lines() {
  write(line);
}

