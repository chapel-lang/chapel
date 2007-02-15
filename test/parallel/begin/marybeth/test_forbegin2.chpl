use Time;

var j, match: int;
var a,b:[1..3] int;
var done: single bool;
config const deterministic = true;

for i in 1..3 {
  j = i;
  var k = i;
  begin {
    if (j == i) then match += 1;
    a(i) = i;
    b(i) = k;
    if (i == 3) then done = true;
  }
}
if (done) {
  if !(deterministic) then
    writeln("Number of matches = ", match);
  writeln(a);
  writeln(b);
}


