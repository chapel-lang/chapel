use IO;

var f = open("fileLines.chpl", iomode.r);

for line in f.lines() {
    write(line);
}
