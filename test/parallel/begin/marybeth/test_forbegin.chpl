use Time;

var j: sync int;
var done: single bool;

var a,b: [1..3] int;
a = 0;

for i in 1..3 {
  j = i;
  var k = i;
  begin {
    a(i) = j;
    b(i) = k;
    if (i == 3) then done = true;
  }
}

if (done) {
writeln(a);
writeln(b);
}
