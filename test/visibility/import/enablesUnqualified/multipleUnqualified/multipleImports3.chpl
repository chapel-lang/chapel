module M {
  var a: int;
  var b = true;
  proc foo() {
    writeln("In M.foo()");
  }

  proc bar(x) {
    writeln(x);
  }
}

module User {
  import M.{a, foo, bar, b};
  import M;

  proc main() {
    writeln(a);
    writeln(M.a);
    writeln(b);
    writeln(M.b);
    foo();
    M.foo();
    bar(a);
    M.bar(M.a);
  }
}
