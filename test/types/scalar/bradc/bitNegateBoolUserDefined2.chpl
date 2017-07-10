
module A {
  proc ~(a: bool) return !a;
}

module B {

  use A;

  proc main() {
    var t: bool = true;
    var f: bool = false;

    writeln("~true = ", ~t);
    writeln("~false = ", ~f);
  }
}
