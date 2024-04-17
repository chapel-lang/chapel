module Library {
  proc callFoo(x) {
    x.foo;
  }
}
module Application {
  use Library;
  proc int.foo { writeln("In int.foo"); return 1; }
  proc main() {
    callFoo(1);
  }
}
