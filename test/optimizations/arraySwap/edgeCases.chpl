use BlockDist;

proc test(d1, d2) {
  var a1: [d1] int;
  var a2: [d2] int;

  a1 = 10;
  a2 = 20;

  a1 <=> a2;

  writeln(a1);
  writeln(a2);
}

var d1 = {0..99} dmapped Block({1..100});
var d2 = {1..100} dmapped Block({1..100});

test(d1, d2);

