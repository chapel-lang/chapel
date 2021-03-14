config const n = 11;
var s0 = "kiss kiss";

var l0: atomic int;
var l1: atomic int;

begin {
  l0.write(1);
  l1.waitFor(1);
  writeln(s0);
}

writeln(s0);
l0.waitFor(1);
s0 = "bang bang";
l1.write(1);
