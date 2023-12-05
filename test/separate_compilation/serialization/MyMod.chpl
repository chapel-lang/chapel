
module MyMod {
  proc foo() {
    writeln("MYPREFIX: MyMod MyMod MyMod");
  }

  // work around the fact that 'foo' will not be resolved unless called
  export proc keepFoo() {
    foo();
  }
}
