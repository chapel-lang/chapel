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

// writeln('T1: inference with value/ref/const ref overloads');
record man1 {
  var x = new r();
  proc enterThis() ref: res {
    writeln('proc man1.enterThis() ref: res'); return globalRes;
  }
  proc enterThis() const ref: res {
    writeln('proc man1.enterThis() const ref: res'); return globalRes;
  }
  proc enterThis(): res {
    writeln('proc man1.enterThis(): res'); return globalRes;
  }
  proc leaveThis(in err: owned Error?) {
    if err then try! { throw err; } writeln('leaving');
  }
}

// writeln('T2: inference with ref/const ref overloads');
record man2 {
  var x = new r();
  proc enterThis() ref: res {
    writeln('proc man2.enterThis() ref: res'); return globalRes;
  }
  proc enterThis() const ref: res {
    writeln('proc man2.enterThis() const ref: res'); return globalRes;
  }
  proc leaveThis(in err: owned Error?) {
    if err then try! { throw err; } writeln('leaving');
  }
}

// writeln('T3: inference with value/ref overloads');
record man3 {
  var x = new r();
  proc enterThis(): res {
    writeln('proc man3.enterThis(): res'); return new res();
  }
  proc enterThis() ref {
    writeln('proc man3.enterThis() ref: res'); return globalRes;
  }
  proc leaveThis(in err: owned Error?) {
    if err then try! { throw err; } writeln('leaving');
  }
}

// writeln('T4: inference with value/const ref overloads');
record man4 {
  var x = new r();
  proc enterThis(): res {
    writeln('proc man4.enterThis(): res'); return new res();
  }
  proc enterThis() const ref: res {
    writeln('proc man4.enterThis() const ref: res'); return globalRes;
  }
  proc leaveThis(in err: owned Error?) {
    if err then try! { throw err; } writeln('leaving');
  }
}

// writeln('T5: only value overload');
record man5 {
  var x = new r();
  proc enterThis(): res {
    writeln('proc man5.enterThis(): res'); return new res();
  }
  proc leaveThis(in err: owned Error?) {
    if err then try! { throw err; } writeln('leaving');
  }
}

// writeln('T6: only ref overload');
record man6 {
  var x = new r();
  proc enterThis() ref: res {
    writeln('proc man6.enterThis() ref: res'); return globalRes;
  }
  proc leaveThis(in err: owned Error?) {
    if err then try! { throw err; } writeln('leaving');
  }
}

// writeln('T7: only const ref overload');
record man7 {
  var x = new r();
  proc enterThis() const ref: res {
    writeln('proc man7.enterThis() const ref: res'); return globalRes;
  }
  proc leaveThis(in err: owned Error?) {
    if err then try! { throw err; } writeln('leaving');
  }
}

proc test1a() {
  writeln('T1A: inference with ALL overloads and NO mutation');
  manage new man1() as myResource do
    myResource.doSomethingConst();
}
test1a(); writeln();

proc test1b() {
  writeln('T1B: inference with ALL overloads and mutation');
  manage new man1() as myResource do
    myResource.mutate();
}
test1b(); writeln();

proc test2a() {
  writeln('T2A: inference with REF/CONST-REF overloads and NO mutation');
  manage new man2() as myResource do
    myResource.doSomethingConst();
}
test2a(); writeln();

proc test2b() {
  writeln('T2B: inference with REF/CONST-REF overloads and mutation');
  manage new man2() as myResource do
    myResource.mutate();
}
test2b(); writeln();

proc test3a() {
  writeln('T3A: inference with VALUE/REF overloads and NO mutation');
  manage new man3() as myResource do
    myResource.doSomethingConst();
}
test3a(); writeln();

proc test3b() {
  writeln('T3B: inference with VALUE/REF overloads and mutation');
  manage new man3() as myResource do
    myResource.mutate();
}
test3b(); writeln();

proc test4() {
  writeln('T4: inference with VALUE/CONST-REF overloads and NO mutation');
  manage new man4() as myResource do
    myResource.doSomethingConst();
}
test4(); writeln();

// TODO: Should this prefer VALUE since the resource is being mutated?
/*
proc test4b() {
  writeln('T4B: inference with VALUE/CONST-REF overloads and mutation');
  manage new man4() as myResource do
    myResource.mutate();
}
test4b(); writeln();
*/

proc test5a() {
  writeln('T5A: only VALUE overload with NO mutation');
  manage new man5() as myResource do
    myResource.doSomethingConst();
}
test5a(); writeln();

proc test5b() {
  writeln('T5B: only VALUE overload with mutation');
  manage new man5() as myResource do
    myResource.mutate();
}
test5b(); writeln();

proc test6() {
  writeln('T6: only REF overload with mutation');
  manage new man6() as myResource do
    myResource.mutate();
}
test6(); writeln();

// Compiler error is in another test.
proc test7() {
  writeln('T7: only CONST-REF overload with NO mutation');
  manage new man7() as myResource do
    myResource.doSomethingConst();
}
test7(); writeln();

