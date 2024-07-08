module M1 {
  class C { var x: int; }
  var ownGlobal = new owned C(1);
  var global: borrowed C = ownGlobal.borrow();
}

module M2 {
  use M1;
  proc main() {
    writeln(global);
  }
}
