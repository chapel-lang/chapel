// To verify deprecation message for openreader
// Was originally test/library/standard/IO/channel/lines.chpl

var c = openreader('helper.txt');

for line in c.lines() {
  write(line);
}

