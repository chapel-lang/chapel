
var j = 0;
var a: [1..3] sync int;

// with ref-clause => old semantics
for i in 1..3 {
  j = i;
  begin with (ref j) {
    if (i < 3) then while a(i+1).readXX() == 0 do;
    a(i).writeEF(i + j);
  }
}

while a(1).readXX() == 0 do;

writeln(a.readFE());

// unmodified code => new semantics
a.reset();
for i in 1..3 {
  j = i;
  begin {
    if (i < 3) then while a(i+1).readXX() == 0 do;
    a(i).writeEF(i + j);
  }
}

while a(1).readXX() == 0 do;

writeln(a.readFE());
