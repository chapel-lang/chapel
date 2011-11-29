module M1 {
  var a = 1;
  class C {
    var myvar = 2;
    proc foo() {
      return a+myvar;
    }
  }
}

module M2 {
  proc main {
    var c = bar();
    writeln(c.myvar);
  }
  proc bar() {
    use M1;
    return new C();
  }
}
