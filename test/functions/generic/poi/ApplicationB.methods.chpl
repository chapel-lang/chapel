// This is a version of
//   test/functions/ferguson/hijacking/ApplicationB.chpl
// beefed up to also use methods.

module GenericLibrary {
  class MyClass {}
  var myC = new MyClass();
  proc note(param message) {
    compilerWarning(message, 2);
    writeln(message);
  }
  // these use foo() and bar() at POI
  proc callFoo1(arg) {
    arg.foo();
    bar();
  }
  proc MyClass.callFoo2(arg) {
    arg.foo();
    bar();
  }
}
module LibraryX {
  use GenericLibrary;
  proc MyClass.foo() { note("X.foo()"); }
  proc bar()         { note("X.bar()"); }
  proc runX() {
    callFoo1(myC);
    myC.callFoo2(myC);
  }
}
module LibraryY {
  use GenericLibrary;
  proc MyClass.foo() { note("Y.foo()"); }
  proc bar()         { note("Y.bar()"); }
  proc runY() {
    callFoo1(myC);
    myC.callFoo2(myC);
  }
}
module Application {
  proc main() {
    use LibraryX;
    runX();
    use LibraryY;
    runY();
  }
}
