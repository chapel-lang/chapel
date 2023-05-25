use IO;

var fw = openTempFile().writer();
if fw.writef("%.5", 3.1415) {
    writeln("nope");
}
