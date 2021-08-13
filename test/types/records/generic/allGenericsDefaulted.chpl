config param errorCase = 0;

record R {
  type t = int;
  param rank: int = 2;
  var x: rank*t;
}

proc foo(r: R) {
  writeln("In foo, r is: ", r);
}

proc bar(r: R(?)) {
  writeln("In bar, r is: ", r);
} 

proc baz(r: R(int, ?rank)) {
  writeln("In baz, r is: ", r);
}

proc boo(r: R(?t, 2)) {
  writeln("In boo, r is: ", r);
}

var ri2: R;
var rr2: R(real, 2);
var ri3: R(int, 3);

foo(ri2);
if errorCase == 1 then
  foo(rr2);
if errorCase == 2 then
  foo(ri3);
writeln();

bar(ri2);
bar(rr2);
bar(ri3);
writeln();

baz(ri2);
if errorCase == 3 then
  baz(rr2);
baz(ri3);
writeln();

boo(ri2);
boo(rr2);
if errorCase == 4 then
  boo(ri3);
writeln();


if errorCase == 5 then
  var ri4: R = new R(real, 3);
