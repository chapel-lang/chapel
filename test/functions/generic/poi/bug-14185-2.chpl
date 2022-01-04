module LibraryOne {
  use GenericLibrary;
  proc foo(i: int) {
    writeln("LibraryOne.foo");
  }
  callFoo(1);
}
module GenericLibrary {
  proc callFoo(param x) {
    foo(x);
  }
}
module LibraryTwo {
  use GenericLibrary;
  proc foo(i: int) {
    writeln("LibraryTwo.foo");
  }
  callFoo(1);
}
module Application {
  use LibraryOne;
  use LibraryTwo;
  proc main() { }
}
