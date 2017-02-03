
class CC { proc deinit() { writeln("CC.deinit()"); } }
record RR { proc deinit() { writeln("RR.deinit()"); } }

var cc = new CC();
cc.deinit();
delete cc;

var rr: RR;
rr.deinit();
