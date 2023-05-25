use IO;

var fw = openTempFile().writer();
if fw.write("yep") {
    writeln("nope");
}
