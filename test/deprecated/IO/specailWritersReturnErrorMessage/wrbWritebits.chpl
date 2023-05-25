use IO;

var fw = openTempFile().writer();
if fw.writeBits(7, 3) {
    writeln("nope");
}
