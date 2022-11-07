use IO;

var fw = opentmp().writer();
if fw.writeBytes(63, 2) {
    writeln("nope");
}
