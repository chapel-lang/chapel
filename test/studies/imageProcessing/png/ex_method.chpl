
class num {
  var v : int;

  proc type dump(msg : string) {
    writeln(msg);
  }
}

writeln("\ndemonstrate static method");
num.dump("calling without an instance");

var n = new num(3);
/* This does not work. */
/*
n.dump("or via an instance");
*/

proc ref num.changeInst() {
  this = new num(5);
}

writeln("\ndemonstrate ref intent");
var oldn = n;
writef("original instance %t (same as old? %s)\n", n, (n == oldn));
n.changeInst();
writef("new instance %t (same as old? %s)\n", n, (n == oldn));







