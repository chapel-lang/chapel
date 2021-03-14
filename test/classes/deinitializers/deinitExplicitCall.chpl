
class CC { proc deinit() { writeln("CC.deinit()"); } }
record RR { proc deinit() { writeln("RR.deinit()"); } }

var cc = new CC();
cc.deinit();
delete cc;

var rr: RR;
rr.deinit();

deinit();
deinit(cc);

proc RR.something() {
  deinit();
  deinit(cc);
}
rr.something();
