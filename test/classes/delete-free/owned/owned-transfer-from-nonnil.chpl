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
  x; // end of statement mentioning x
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

// ok: x not used after transfer
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

// ok: x not used after transfer
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

// OK: copy is elided, x is dead
proc test6() {
  var x = new R(1);
  var y = x; // ownership transfer to y
  // end of statement mentioning x
}
test6();

// OK: copy is elided, x is dead
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
  writeln(x, y);
}
test8();

// expect error: R cannot store nil after x transferred out of
proc test9() {
  var x = new R(1);
  var y = x; // ownership transfer to y
  writeln(x, y); // x is now storing nil
  // end of statement mentioning x
}
test9();

// expect error: R.field cannot store nil after transferred out of
proc test10() {
  var x = new R(1);
  var y = x.field; // ownership transfer to y
  writeln(x, y); // x is now storing nil
  // end of statement mentioning x
}
test10();

record R1 {
  var field: owned MyClass;
  proc init(x:int) {
    this.field = new owned MyClass(x);
  }
  proc init=(ref rhs: R1) {
    this.field = rhs.field;
  }
}
proc =(ref lhs: R1, ref rhs: R1) {
  lhs.field = rhs.field;
}

// expect error: R1 cannot store nil after x transferred out of
// error reported in = for R1
proc test11() {
  var x = new R1(1);
  var y = new R1(2);
  y = x;
  writeln(x, y);
}
test11();

// expect error: R1 cannot store nil after x transferred out of
// error reported in init= for R1
proc test12() {
  var x = new R1(1);
  var y = x; // ownership transfer to y
  writeln(x, y); // x is now storing nil
  // end of statement mentioning x
}
test12();

record R2 {
  var field: owned MyClass;
  proc init(x:int) {
    this.field = new owned MyClass(x);
  }
  proc init=(const ref rhs: R2) {
    this.field = new owned MyClass(rhs.field.x);
  }
}
proc =(ref lhs: R2, const ref rhs: R2) {
  lhs.field = new owned MyClass(rhs.field.x);
}

// OK
proc test13() {
  var x = new R2(1);
  var y = new R2(2);
  y = x;
  writeln(x, y);
}
test13();

// OK
proc test14() {
  var x = new R2(1);
  var y = x; // ownership transfer to y
  writeln(x, y); // x is now storing nil
  // end of statement mentioning x
}
test14();
