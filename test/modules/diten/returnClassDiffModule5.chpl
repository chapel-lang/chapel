pragma "error mode fatal"
module M1 {
  var a = 1;
  class C {
    var b = 2;
    proc foo() {
      return a+b;
    }
    proc writeThis(f) {
      f.writeln("How does this get found?");
      f.write("{a = ", a, ", b = ", b, "}");
    }
  }
}

module M2 {
  proc main {
    var c = bar();
    writeln(baz(c));
    writeln(c);
    delete c;
  }
  proc bar() {
    use M1;
    return new C();
  }
  proc baz(obj:object) {
    use M1;
    return (obj:C).foo();
  }
}
