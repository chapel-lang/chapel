// This test shows what appears to be a spurious lifetime
// checker error. Since the initializer is consuming 'new C(30)'
// through the 'in' intent, it seems like the lifetime of the actual
// should not matter.

class C { var field: int; }

var globalOwn = new owned C(10);
var globalBorrow = globalOwn.borrow();

record myRecord {
  var b: borrowed C;
  proc init(in myInstance) /*lifetime return globalBorrow */ {
    b = globalBorrow;
  }
}

proc main() {
  // works
  var cc = new C(20);
  var x = new myRecord(cc);
  writeln(x);

  // fails with Option 1 unless lifetime clause is added,
  // but used to work
  var y = new myRecord(new C(30));
  writeln(y);

  // fails unless lifetime clause is added
  var z;
  {
    z = new myRecord(new C(30));
  }
  writeln(z);
}
