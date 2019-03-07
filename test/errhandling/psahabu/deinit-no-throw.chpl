// class case handled by object.deinit() being unable to throw

record ThrowingDeinit {
  var i: int;

  proc init() {
    writeln("init");
    i = 0;
  }

  proc doSomething() {
    writeln("hello world");
  }

  proc deinit() throws {
    writeln("deinit");
    throw new owned Error();
  }
}

var t = new ThrowingDeinit();
t.doSomething();
