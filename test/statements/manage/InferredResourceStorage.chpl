use TrackingRecord;

record res {
  proc const doSomethingConst() {
    writeln('const action with resource');
  }

  proc ref mutate() {
    writeln('mutating resource');
  }
}

var globalRes = new res();

record man1 : contextManager {
  var x = new r();
  proc enterContext(): res {
    writeln('proc man1.enterContext(): res'); return new res();
  }
  proc exitContext(in err: owned Error?) {
    if err then try! { throw err; } writeln('leaving');
  }
}

record man2 : contextManager {
  var x = new r();
  proc enterContext() ref: res {
    writeln('proc man2.enterContext() ref: res'); return globalRes;
  }
  proc exitContext(in err: owned Error?) {
    if err then try! { throw err; } writeln('leaving');
  }
}

record man3 : contextManager {
  var x = new r();
  proc enterContext() const ref: res {
    writeln('proc man3.enterContext() const ref: res'); return globalRes;
  }
  proc exitContext(in err: owned Error?) {
    if err then try! { throw err; } writeln('leaving');
  }
}

proc test1a() {
  writeln('T1A: only VALUE overload with NO mutation');
  manage new man1() as myResource do
    myResource.doSomethingConst();
}
test1a(); writeln();

proc test1b() {
  writeln('T1B: only VALUE overload with mutation');
  manage new man1() as myResource do
    myResource.mutate();
}
test1b(); writeln();

proc test2() {
  writeln('T2: only REF overload with mutation');
  manage new man2() as myResource do
    myResource.mutate();
}
test2(); writeln();

// Compiler error is in another test.
proc test3() {
  writeln('T3: only CONST-REF overload with NO mutation');
  manage new man3() as myResource do
    myResource.doSomethingConst();
}
test3(); writeln();

