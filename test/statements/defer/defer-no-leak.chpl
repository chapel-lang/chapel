class C {}

proc foo() {
  defer {
    var c = new owned C();
    writeln(c);
  }
}

foo();
