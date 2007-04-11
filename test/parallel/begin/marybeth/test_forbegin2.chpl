use Time;

var j, match: int;
var a,b:[1..3] int;
var done = false;
var count: sync int = 0;
config const deterministic = true;

for i in 1..3 {
  j = i;
  var k = i;
  begin {
    if (j == i) then match += 1;
    a(i) = i;
    b(i) = k;
    var tmp = readFE(count);
    writeXF(count,tmp+1);
  }
}

while !done {
  if (readXX(count) == 3) {
    if !(deterministic) then
      writeln("Number of matches = ", match);
    writeln(a);
    writeln(b);
    done = true;
  }
}

