module callInGeneric {
  proc foo() {
    writeln('foo');
  }

  proc test(param x=1) {
    import this.Other.blah;
    if blah then
      foo();
  }
  test();

  module Other {
    var blah = true;
  }
}
