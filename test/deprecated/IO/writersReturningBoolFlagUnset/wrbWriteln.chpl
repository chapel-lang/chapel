use IO;

var fw = opentmp().writer();
if fw.writeln("yep") {
    writeln("nope");
}
