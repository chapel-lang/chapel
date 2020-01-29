class MyClass { var x: int; }

proc passWithInIntent(in arg) { }

// ok: x is dead when transferred
proc test1() {
  var x = new owned MyClass(1);
  var y = x; // ownership transfer to y
  // end of statement mentioning x
}
test1();

// ok: x is dead when transferred
proc test1b() {
  var x = new owned MyClass(1);
  {
    var y = x; // ownership transfer to y
  }
  // end of statement mentioning x
}
test1b();

// error: x used after transfer
proc test1c() {
  var x = new owned MyClass(1);
  var y = x; // ownership transfer to y
  writeln(x);
  // end of statement mentioning x
}
test1c();

// error: x is end-of-block
proc test1d() {
  var x = new owned MyClass(1);
  ref rx = x;
  var y = x; // ownership transfer to y
  // end of statement mentioning x
}
test1d();

config const something = true;

// ok: x is dead after transfer
proc test2() {
  var x = new owned MyClass(1);
  if something {
    passWithInIntent(x); // ownership transfer to formal argument
  }
}
test2();

// ok: x is dead after transfer
proc test2b() {
  var x = new owned MyClass(1);
  {
    if something {
      {
        passWithInIntent(x); // ownership transfer to formal argument
      }
    }
  }
}
test2b();

// error: x used after transfer
proc test2c() {
  var x = new owned MyClass(1);
  if something {
    passWithInIntent(x); // ownership transfer to formal argument
    writeln(x);
  }
}
test2c();

// error: x used after transfer
proc test2d() {
  var x = new owned MyClass(1);
  if something {
    passWithInIntent(x); // ownership transfer to formal argument
  }
  writeln(x);
}
test2d();

// error: x is end-of-block
proc test2e() {
  var x = new owned MyClass(1);
  ref rx = x;
  if something {
    passWithInIntent(x); // ownership transfer to formal argument
  }
}
test2e();

proc passInAndRef(in a, ref b) {
  writeln(a.x);
  writeln(b.x);
}

proc refIdentity(ref a) ref { return a; }

// error: x is used as argument after transfer
proc test3() {
  var x = new owned MyClass(1);
  passInAndRef(x, x);
}
test3();

// error: x is used as argument after transfer
proc test3a() {
  var x = new owned MyClass(1);
  passInAndRef(refIdentity(x), refIdentity(x));
}
test3a();

// ok: x is dead when transferred
proc test4() {
  var x = new owned MyClass(1);
  var y: owned MyClass?;
  y = x; // ownership transfer to y
  // end of statement mentioning x
}
test4();

// ok: x is dead when transferred
proc test4b() {
  var x = new owned MyClass(1);
  {
    var y: owned MyClass?;
    y = x; // ownership transfer to y
  }
  // end of statement mentioning x
}
test4b();

// error: x used after transfer
proc test4c() {
  var x = new owned MyClass(1);
  var y: owned MyClass?;
  y = x; // ownership transfer to y
  writeln(x); // end of statement mentioning x
}
test4c();

// error: x is end-of-block
proc test4d() {
  var x = new owned MyClass(1);
  ref rx = x;
  var y: owned MyClass?;
  y = x; // ownership transfer to y
  // end of statement mentioning x
}
test4d();

// does ownership transfer in returning
// expecting an error for instantiation with owned non-nilable
proc acceptRefButReturn(ref arg) { return arg; }

// error in acceptRefButReturn
proc test5() {
  var x = new owned MyClass(1);
  var y = acceptRefButReturn(x);
}
test5();

record R {
  var field: owned MyClass;
  proc init(x:int) {
    this.field = new owned MyClass(x);
  }
}

// expect error: R cannot store nil after x transferred out of
proc test6() {
  var x = new R(1);
  var y = x; // ownership transfer to y
  // end of statement mentioning x
}
test6();

// expect error: R cannot store nil after x transferred out of
proc test7() {
  var x = new R(1);
  passWithInIntent(x);
}
test7();

// expect error: R cannot store nil after x transferred out of
proc test8() {
  var x = new R(1);
  var y = new R(2);
  y = x;
}
test8();

// TODO: add some loop examples
