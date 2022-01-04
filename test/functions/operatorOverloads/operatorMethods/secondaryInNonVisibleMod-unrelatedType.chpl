module MyEquals {
  proc myEquals(x, y) {
    return x == y;
  }
}


module MyType {
  class C {
    var x: int;
  }

  operator C.==(a: int, b: int) {
    // I don't actually expect to see this message.  It is to distinguish from
    // the default operator and ensure we don't see it.
    writeln("In C.==(int, int)");
    return a == b;
  }
}

module Main {
  proc main() {
    use MyType, MyEquals;

    var myC1 = new C(1);
    var myC1a = new C(1);
    var myC2 = new C(2);

    writeln(1 == 1); // expect true
    writeln(1 == 2);  // expect false
    writeln(myEquals(1, 1)); // expect true
    writeln(myEquals(1, 2));  // expect false
  }
}
