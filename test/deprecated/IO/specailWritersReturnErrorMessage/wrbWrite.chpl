use IO;

var fw = opentmp().writer();
if fw.write("yep") {
    writeln("nope");
}
