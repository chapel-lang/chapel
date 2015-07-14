record R {
  proc type foo() {
    writeln("In R.foo()");
  }

  proc type bar() {
    return 42;
  }
}

R.foo();
writeln(R.bar());
