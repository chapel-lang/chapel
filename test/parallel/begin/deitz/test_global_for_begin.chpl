use Time;

var j = 0;
var a: [1..3] int;

// with ref-clause => old semantics
for i in 1..3 {
  j = i;
  begin with (ref i, ref j) {
    if (i < 3) then while a(i+1) == 0 do sleep(1);
    a(i) = i + j;
  }
}

while a(1) == 0 do sleep(1);

writeln(a);

// unmodified code => new semantics
a = 0;
for i in 1..3 {
  j = i;
  begin {
    if (i < 3) then while a(i+1) == 0 do sleep(1);
    a(i) = i + j;
  }
}

while a(1) == 0 do sleep(1);

writeln(a);
