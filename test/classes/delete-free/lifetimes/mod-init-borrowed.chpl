pragma "safe"
module M0 {
  proc main() {
    use M1 only;
    use M2 only;

    var a   = new borrowed A(10);

    writeln();

    var m1a = new borrowed M1.A(20);

    writeln();

    var m2a = new M2.A(40);
  }

  class A {
    proc init(a : int) {
      writeln("A.init(", a, ")");
    }
  }
}

// A module with a class with an initializer
pragma "safe"
module M1 {
  class A {
    proc init(a : int) {
      writeln("M1.A.init(", a, ")");
    }
  }
}


// A module with a record with an initializer
pragma "safe"
module M2 {
  record A {
    proc init(a : int) {
      writeln("M2.A.init(", a, ")");
    }
  }
}
