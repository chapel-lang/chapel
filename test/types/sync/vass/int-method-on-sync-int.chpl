// Upon calling an int method on a sync int variable, what should happen?
// * wait for the variable to become full, then empty it?
// * ignore the variable's full/empty state?
// * compile-time error?

proc int.methodOnInt() {
  writeln("in methodOnInt: ", this);
}

var i$: sync int = 55;

i$.methodOnInt();
