use IO;

var fw = openTempFile().writer();
if fw.writeBytes(63, 2) {
    writeln("nope");
}
