config param case = 1, aligned=false;

proc testit(r) {
  if !aligned then writeln(r.low, "..", r.high); else writeln(r.alignedLow, "..", r.alignedHigh);
}

var r = 1..;
var r2 = ..10;
var r3 = 1.. by 2;
var r4 = ..10 by -2;

if case == 1 then
  testit(r);
if case == 2 then
  testit(r2);
if case == 3 then
  testit(r3);
if case == 4 then
  testit(r4);
