
/*****
      ex_method.chpl -
      Examples of methods.  See text for details.

      c 2015-2018 Primordial Machine Vision Systems
*****/

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
  this = new unmanaged num(5);
}

writeln("\ndemonstrate ref intent");
var oldn = n.borrow();
writef("original instance %? (same as old? %s)\n", n, (n == oldn));
n.changeInst();
writef("new instance %? (same as old? %s)\n", n, (n == oldn));
