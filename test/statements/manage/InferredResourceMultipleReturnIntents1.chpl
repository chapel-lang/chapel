use TrackingRecord;

proc r.enterContext() {
  writeln('entering');
}

proc r.exitContext(in err: owned Error?) {
  writeln('leaving');
  if err then try! { throw err; }
}

record res {
  proc doSomething() {
    writeln('doing something with resource');
  }
}

var globalRes = new res();

record man : contextManager {
  var x = new r();

  proc enterContext() ref {
    writeln('proc man.enterContext() ref: res');
    return globalRes;
  }

  proc enterContext() const ref {
    writeln('proc man.enterContext() const ref: res');
    return globalRes;
  }

  proc enterContext() {
    writeln('proc man.enterContext(): res');
    return globalRes;
  }

  proc exitContext(in err: owned Error?) {
    writeln('leaving');
    if err then try! { throw err; }
  }
}

// This will currently trigger a compiler error, because the compiler is
// unable to infer what the storage of 'myResource' should be.
proc test1() {
  writeln('T1: storage kind of resource omitted');
  manage new man() as myResource do
    myResource.doSomething();
}
test1(); writeln();

// Make sure notes don't trigger again.
proc test2() {
  writeln('T2: avoid duplicate compiler notes');
  manage new man() as myResource do
    myResource.doSomething();
}
test2(); writeln();

