module M1 {

  var x: int = 1;

  module M2 {

    var x: int = 2;

    module M3 {
      import M1.M2.x;
      proc foo() {
        writeln(x);
      }

    }

  }

}

module M {
  proc main() {
    use M1.M2.M3;
    foo();
  }
}
