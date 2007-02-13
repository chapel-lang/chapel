use Time;

var j: sync int;
var done: single bool;

var a: [1..3] int;
a = 0;

for i in 1..3 {
  j = i;
  var k = i;
  begin {
    a(i) = j;
    writeln("i = ",i);
    writeln("k = ",k);
    if (i == 3) then done = true;
  }
}

if (done) {
writeln(a);
}
