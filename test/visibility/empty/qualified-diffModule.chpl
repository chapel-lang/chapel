module M {
  class Foo { }
}
module M2 {
  use M;
  proc Foo.secondary() { writeln("in secondary method"); }
}
module M3 {
  use M only;

  proc main() {
    var x = new M.Foo();
    x.borrow().secondary(); // Shouldn't be accessible: we didn't include M2
  }
}
