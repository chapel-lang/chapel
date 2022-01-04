module MyEquals {
  proc myEquals(x, y) {
    return x == y;
  }
}


module MyType {
  class C {
    var x: int;
  }

  operator C.==(a: C, b: int) {
    return a.x == b;
  }
}

module Main {
  proc main() {
    use MyType, MyEquals;

    var myC1 = new C(1);
    var myC1a = new C(1);
    var myC2 = new C(2);

    writeln(myC1 == 1); // expect true
    writeln(myC1 == 2);  // expect false
    writeln(myEquals(myC1, 1)); // expect true
    writeln(myEquals(myC1, 2));  // expect false
  }
}
