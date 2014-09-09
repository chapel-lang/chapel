config const n = 11;
var s0 = "kiss kiss";

var l0: atomic int;
var l1: atomic int;

writeln(s0);
begin with (ref s0) {
  l0.waitFor(1);
  s0 = "bang bang";
  l1.write(1);
}

on Locales[numLocales-1] {
  l0.write(1);
  l1.waitFor(1);
  writeln(s0);
}

