// A simple reproducer in #21292

var ic, dc: atomic int;
var iv, dv: atomic int;

record R {
  var myval: int;
  proc init(x) { myval = x; iv.add(myval); ic.add(1); }
  proc deinit() { dv.add(myval); dc.add(1); }
}

config var skip = true;

forall i in 0..8 {

  var r0 = new R(1);

  if i == 0 then continue;

  var r1 = new R(10);

  if i == 1 then continue;

  var r2 = new R(100);

  if i == 2 then continue;

  var r3 = new R(1000);

  if i == 3 then continue;

  var r4 = new R(10000);
}

writeln(ic);
writeln(dc);

writeln(iv);
writeln(dv);
