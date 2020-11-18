module M {
  class Foo { }
}
module M2 {
  use M;
  proc Foo.secondary() { writeln("in secondary method"); }
}
module M3 {
  use M only;
  use M2 only;

  proc main() {
    var x = new borrowed M.Foo();
    x.secondary(); // Shouldn't be accessible because we didn't include M2
  }
}
