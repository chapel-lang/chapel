use IO;

var fw = openTempFile().writer();
if fw.writebits(7, 3) {
    writeln("nope");
}
