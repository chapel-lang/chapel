module Library {
  class C {
    proc method() { writeln("in Library's C.method"); }
  }
  class D: C { }
  class E: C { }
  var myD = new D();
  myD.method();
}
module Other {
  use Library;
  // shouldn't be allowed because
  // it would change myD.method() in Library
  override proc D.method() { writeln("in Other's D.method"); }

  proc main() {
    var d: C = new D();
    d.method();
  }
}
