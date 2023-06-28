class C { var field: int; }

var globalOwn = new owned C(10);
var globalBorrow = globalOwn.borrow();

record myRecord {
  var b: borrowed C;
  proc init(in myInstance) lifetime return globalBorrow {
    b = globalBorrow;
  }
}


proc main() {
  // works
  var cc = new C(20);
  var x = new myRecord(cc);
  writeln(x);

  // leads to error today if no lifetime annotation is used
  var y = new myRecord(new C(30));
  writeln(y);
}
