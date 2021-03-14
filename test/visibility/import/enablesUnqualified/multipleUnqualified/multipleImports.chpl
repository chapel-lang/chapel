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
  import M.{a, foo, bar};
  import M.{b};

  proc main() {
    writeln(a);
    writeln(b);
    foo();
    bar(a);
  }
}
