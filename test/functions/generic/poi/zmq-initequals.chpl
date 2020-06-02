/* This is a simplification of this test:
     test/library/packages/ZMQ/pair.chpl
*/

module Application {
  use MyZMQ;
  record Foo {
  }
  proc Foo.init=(rhs: Foo) { // user-defined or compiler-generated
    compilerError("success");
  }
  proc main  {
    recv(Foo);
  }
}

module MyZMQ {
  proc recv(type T) {
    var data, ret: T;
    ret = data;       // need functions visible from the caller scope
  }
}
