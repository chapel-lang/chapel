class C {
  var D: domain(1);
  var A: [D] int;
}

var D = {1..4};

var c = new unmanaged C(D);
writeln(c);
c.D = {1..8};
writeln(c);
D = {1..8};
writeln(c);

delete c;
