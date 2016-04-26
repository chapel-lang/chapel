module M1 {
  proc foo() {
    writeln("wait, I executed?");
  }

  proc bar() {
    writeln("I'm okay!");
  }
}

module M2 {
  use M1 except foo;

  proc main() {
    foo();
    bar();
  }

}
