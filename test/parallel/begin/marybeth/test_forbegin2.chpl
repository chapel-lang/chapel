use Time;

var j, match: int;
var done: single bool;
config const deterministic = true;

for i in 1..3 {
  j = i;
  var k = i;
  begin {
    if (j == i) then match += 1;
    writeln("i = ",i);
    writeln("k = ",k);
    if (i == 3) then done = true;
  }
}
if !(deterministic) then
  if (done) then
    writeln("Number of matches = ", match);


