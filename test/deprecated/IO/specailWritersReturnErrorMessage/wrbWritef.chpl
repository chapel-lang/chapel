use IO;

var fw = opentmp().writer();
if fw.writef("%.5", 3.1415) {
    writeln("nope");
}
