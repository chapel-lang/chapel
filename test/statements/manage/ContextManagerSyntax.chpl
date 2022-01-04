//
// TODO: Check to make sure 'const' storage kind is immutable.
//

use TrackingRecord;

proc r.enterThis() {
  writeln('entering');
}

proc r.leaveThis(in err: owned Error?) {
  writeln('leaving');
  if err then try! { throw err; }
}

record res {
  proc doSomething() {
    writeln('doing something with resource');
  }
}

var globalRes = new res();

record man {
  var x = new r();

  proc enterThis() ref {
    writeln('proc man.enterThis() ref: res');
    return globalRes;
  }

  proc enterThis() const ref {
    writeln('proc man.enterThis() const ref: res');
    return globalRes;
  }

  proc enterThis() {
    writeln('proc man.enterThis(): res');
    return globalRes;
  }

  proc leaveThis(in err: owned Error?) {
    writeln('leaving');
    if err then try! { throw err; }
  }
}

proc doSomething() {
  writeln('doing something');
}

proc test1() {
  writeln('T1');
  // Calls the value overload of 'enterThis'.
  manage new man() do doSomething();
}
test1(); writeln();

proc test2() {
  writeln('T2');
  // Calls the value overload of 'enterThis'.
  manage new man() {
    doSomething();
  }
}
test2(); writeln();

// Make sure 'manager' stays alive till end of block.
proc test3() {
  writeln('T3');
  var myManager = new man();
  manage myManager do doSomething();
}
test3(); writeln();

// Make sure 'manager' stays alive till end of block, nested.
proc test4() {
  writeln('T4');
  var myManager = new man();
  manage myManager, new man() {
    doSomething();
  }
}
test4(); writeln();

// Make sure both managers stay alive till end of block.
proc test5() {
  writeln('T5');
  var myManager1 = new man();
  var myManager2 = new man();
  manage myManager1, myManager2 do
    doSomething();
}
test5(); writeln();

// TODO: This may be dropped if we decide we don't want explicit storage.
proc test6() {
  writeln('T6: resource is explicitly var');
  manage new man() as var myResource do
    myResource.doSomething();
}
test6(); writeln();

// TODO: This may be dropped if we decide we don't want explicit storage.
proc test7() {
  writeln('T7: resource is explicitly ref');
  manage new man() as ref myResource do
    myResource.doSomething();
}
test7(); writeln();

// TODO: This may be dropped if we decide we don't want explicit storage.
proc test8() {
  writeln('T8: resource is explicitly const ref');
  manage new man() as const ref myResource do
    myResource.doSomething();
}
test8(); writeln();

// TODO: This may be dropped if we decide we don't want explicit storage.
// Make sure manager survives to end of block.
proc test9() {
  writeln('T9: resource is explicitly var');
  var myManager = new man();
  manage myManager as var myResource do
    myResource.doSomething();
}
test9(); writeln();

// TODO: This may be dropped if we decide we don't want explicit storage.
// Make sure manager survives to end of block.
proc test10() {
  writeln('T10: resource is explicitly ref');
  var myManager = new man();
  manage myManager as ref myResource do
    myResource.doSomething();
}
test10(); writeln();

// TODO: This may be dropped if we decide we don't want explicit storage.
// Make sure manager survives to end of block.
proc test11() {
  writeln('T11: resource is explicitly const ref');
  var myManager = new man();
  manage myManager as const ref myResource do
    myResource.doSomething();
}
test11(); writeln();

// TODO: This may be dropped if we decide we don't want explicit storage.
proc test12() {
  writeln('T12: nested managers, mixed resource storage types');
  manage new man() as var res1, new man() as ref res2,
         new man() as const ref res3 {
    res1.doSomething();
    res2.doSomething();
    res3.doSomething();
  }
}
test12(); writeln();

// TODO: This may be dropped if we decide we don't want explicit storage.
// All managers should live till end of block.
proc test13() {
  writeln('T13: nested managers, mixed resource storage types');
  var myManager1 = new man();
  var myManager2 = new man();
  var myManager3 = new man();
  manage myManager1 as var res1, myManager2 as ref res2,
         myManager3 as const ref res3 {
    res1.doSomething();
    res2.doSomething();
    res3.doSomething();
  }
}
test13(); writeln();

// TODO: This may be dropped if we decide we don't want explicit storage.
proc test14() {
  writeln('T14: same manager nested, mixed resource storage types');
  var myManager = new man();
  manage myManager as var res1, myManager as ref res2,
         myManager as const ref res3 {
    res1.doSomething();
    res2.doSomething();
    res3.doSomething();
  }
}
test14(); writeln();

// TODO: This will currently prefer const ref > value > ref. We need to
// decide what the order should be (and the current order is almost certainly
// bugged and should prefer value first).
proc test15() {
  writeln('T15: storage kind of resource omitted');
  manage new man() as myResource do
    myResource.doSomething();
}
test15(); writeln();
