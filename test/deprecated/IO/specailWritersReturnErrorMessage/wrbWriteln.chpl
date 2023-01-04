use IO;

var fw = openTempFile().writer();
if fw.writeln("yep") {
    writeln("nope");
}
