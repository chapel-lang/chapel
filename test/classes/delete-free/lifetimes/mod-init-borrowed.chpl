pragma "safe"
module M0 {
  proc main() {
    var a   = new borrowed A(10);

    writeln();

    var m1a = new borrowed M1.A(20);
    var m2a = new borrowed M2.A(30);

    writeln();

    var m3a = new M3.A(40);
    var m4a = new M4.A(50);
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


// A module with a class with a  constructor
pragma "safe"
module M2 {
  class A {
    proc A(a : int) {
      writeln("M2.A.A(", a, ")");
    }
  }
}


// A module with a record with an initializer
pragma "safe"
module M3 {
  record A {
    proc init(a : int) {
      writeln("M3.A.init(", a, ")");
    }
  }
}


// A module with a record with a  constructor
pragma "safe"
module M4 {
  record A {
    proc A(a : int) {
      writeln("M2.A.A(", a, ")");
    }
  }
}
