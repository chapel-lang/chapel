module M {
  class C {
    proc foo() {
      writeln("In C.foo()!");
    }
  }
}

module N {
  private use M;

  proc bar() {
    return new C();
  }
}

module O {
  private use N;

  proc main() {
    var x = bar();
    writeln(x.type:string);
    x.foo();
  }
}
