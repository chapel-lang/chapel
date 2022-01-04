module MyEquals {
  proc myEquals(x, y) {
    return x == y;
  }
}


module MyType {
  class C {
    var x: int;
  }

  operator C.==(a: int, b: C) {
    return a == b.x;
  }
}

module Main {
  proc main() {
    use MyType, MyEquals;

    var myC1 = new C(1);
    var myC1a = new C(1);
    var myC2 = new C(2);

    writeln(1 == myC1a); // expect true
    writeln(1 == myC2);  // expect false
    writeln(myEquals(1, myC1a)); // expect true
    writeln(myEquals(1, myC2));  // expect false
  }
}
