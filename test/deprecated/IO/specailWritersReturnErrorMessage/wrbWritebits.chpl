use IO;

var fw = opentmp().writer();
if fw.writebits(7, 3) {
    writeln("nope");
}
