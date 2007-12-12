use Time;

var j: sync int;
var count: sync int = 0;
var done = false;

var a,b: [1..3] int;
a = 0;

for i in 1..3 {
  j = i;
  var k = i;
  begin {
    a(i) = j;
    b(i) = k;
    count += 1;
  }
}

while !done {
  if (count.readXX() == 3) {
    writeln(a);
    writeln(b);
    done = true;
  }
}
