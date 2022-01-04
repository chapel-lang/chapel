module MyEquals {
  proc myEquals(x, y) {
    return x == y;
  }
}


module MyType {
  class C {
    var x: int;
  }

  operator C.==(a: C, b: C) {
    return a.x == b.x;
  }
}

module Main {
  proc main() {
    use MyType, MyEquals;

    var myC1 = new C(1);
    var myC1a = new C(1);
    var myC2 = new C(2);

    writeln(myC1 == myC1a); // expect true
    writeln(myC1 == myC2);  // expect false
    writeln(myEquals(myC1, myC1a)); // expect true
    writeln(myEquals(myC1, myC2));  // expect false
  }
}
