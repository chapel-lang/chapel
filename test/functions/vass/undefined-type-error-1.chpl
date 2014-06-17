class CC { var dd:DD; }
class DD {}
class EE {}

proc CC.funC(arg) return dd.funD(arg, 0);
proc DD.funD(arg: dummy, extras): int  return 0;
proc EE.dummy type return int;

var cc = new CC();
writeln(cc.funC(0));
