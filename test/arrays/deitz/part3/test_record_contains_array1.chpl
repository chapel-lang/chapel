record R {
  var D: domain(1);
  var A: [D] int;
}

var D = {1..4};

var r = new R(D);
writeln(r);
r.D = {1..8};
writeln(r);
D = {1..8};
writeln(r);
