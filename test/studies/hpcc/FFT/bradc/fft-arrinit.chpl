config var n = 4;

var D = {1..n};

var W: [D] real = [i in D] i;

writeln("W is: ", W);

foo(W);

proc foo(W: [?WD]) {
  var Perm: [WD] int = [i in WD] i;
  writeln("Perm is: ", Perm);
}
