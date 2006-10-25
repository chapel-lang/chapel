config var n = 4;

var D = [1..n];

var W: [i in D] real = i;

writeln("W is: ", W);

foo(W);

def foo(W: [?WD]) {
  var Perm: [i in WD] int = i;
  writeln("Perm is: ", Perm);
}
