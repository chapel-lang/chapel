use IO;

var f = open("fileLines.chpl", ioMode.r);

for line in f.lines() {
    write(line);
}
