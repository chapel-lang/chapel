module A {
  var x: int;
}
module B {
  proc foo() {
    writeln("In B.foo()");
  }
}
module C {
  var z = 11;
}
module User {
  import A.x as y, B.{foo as bar}, C as D;

  proc main() {
    writeln(y);
    bar();
    writeln(D.z);
  }
}
