module M1 {
  var a = 1;
  class C {
    var b = 2;
    proc foo() {
      return a+b;
    }
  }
}

module M2 {
  proc main {
    var c = bar();
    writeln(c.foo());
  }
  proc bar() {
    use M1;
    return new C();
  }
}
