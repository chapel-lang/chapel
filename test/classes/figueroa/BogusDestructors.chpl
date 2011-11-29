class C {
  proc ~C () {writeln("inside ~C");}
}

proc C.~C () {writeln("inside C.~C");}

record R {
  proc ~R () {writeln("inside ~R");}
}

var c: C;
var r: R;

delete c;
delete r;
