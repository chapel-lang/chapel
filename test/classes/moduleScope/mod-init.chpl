proc main() {
  var a   = new A(10);

  writeln();

  var m1a = new M1.A(20);
  var m2a = new M2.A(30);

  writeln();

  var m3a = new M3.A(40);
  var m4a = new M4.A(50);

  delete m2a;
  delete m1a;
  delete a;
}

class A {
  proc init(a : int) {
    writeln("A.init(", a, ")");
  }
}

// A module with a class with an initializer
module M1 {
  class A {
    proc init(a : int) {
      writeln("M1.A.init(", a, ")");
    }
  }
}


// A module with a class with a  constructor
module M2 {
  class A {
    proc A(a : int) {
      writeln("M2.A.A(", a, ")");
    }
  }
}


// A module with a record with an initializer
module M3 {
  record A {
    proc init(a : int) {
      writeln("M3.A.init(", a, ")");
    }
  }
}


// A module with a record with a  constructor
module M4 {
  record A {
    proc A(a : int) {
      writeln("M2.A.A(", a, ")");
    }
  }
}
