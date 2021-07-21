module Library {
  class C {
    proc method() { }
  }
  class D: C { }
  class E: C { }
  var myD = new D();
  myD.method();
}
module OKOther {
  use Library;
  // these methods are not defined on C/D in Library
  // and so they can't affect code in there
  proc C.otherOperation() { writeln("in C.otherOperation"); }
  override proc D.otherOperation() { writeln("in D.otherOperation"); }

  proc main() {
    var d: C = new D();
    var e: C = new E();
    d.otherOperation();
    e.otherOperation();
  }
}
