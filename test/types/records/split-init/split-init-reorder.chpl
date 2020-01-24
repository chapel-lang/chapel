// Note that this test is sensitive to the fact that array initialization
// currently uses default-initialization and then assignment in many cases. As
// that is improved, the .good file should be updated.

config const cond = false;
config const otherCond = true;
var print = true;

class C { }
record R {
  var s: string;
  var ptr: owned C = new owned C();
  proc init() {
    if print then writeln("init (default)");
  }
  proc init(s:string) {
    this.s = s;
    if print then writeln("init ", s);
  }
  proc init=(other: R) {
    this.s = other.s;
    if print then writeln("init= ", other.s);
  }
  proc deinit() {
    if print then writeln("deinit ", s);
  }
}
proc =(ref lhs:R, rhs:R) {
  if print then writeln("= ", lhs.s, " ", rhs.s);
  lhs.s = rhs.s;
}

proc f(a, b) { }

proc test1() {
  writeln("test1");
  var a;
  var b;
  b = new R("b");
  a = new R("a");
  f(a, b);
  writeln("end");
}
test1();

proc test1eob() {
  writeln("test1eob");
  var a;
  var b;
  b = new R("b");
  a = new R("a");
  ref ra = a;
  ref rb = b;
}
test1eob();

proc test2() {
  writeln("test2");
  var a;
  var b;
  {
    b = new R("b");
    a = new R("a");
  }
  f(a, b);
  writeln("end");
}
test2();

proc test2eob() {
  writeln("test2eob");
  var a;
  var b;
  {
    b = new R("b");
    a = new R("a");
  }
  ref ra = a;
  ref rb = b;
}
test2eob();

proc test3() {
  writeln("test3");
  var a;
  var b;
  {
    b = new R("b");
    {
      a = new R("a");
    }
  }
  f(a, b);
  writeln("end");
}
test3();

proc test3eob() {
  writeln("test3eob");
  var a;
  var b;
  {
    b = new R("b");
    {
      a = new R("a");
    }
  }
  ref ra = a;
  ref rb = b;
}
test3eob();

proc test4() {
  writeln("test4");
  var a;
  var b;
  {
    {
      b = new R("b");
    }
    a = new R("a");
  }
  f(a, b);
  writeln("end");
}
test4();

proc test4eob() {
  writeln("test4eob");
  var a;
  var b;
  {
    {
      b = new R("b");
    }
    a = new R("a");
  }
  ref ra = a;
  ref rb = b;
}
test4eob();

proc test5() {
  writeln("test5");
  var a;
  var b;
  if cond {
    b = new R("b0");
    a = new R("a0");
  } else {
    b = new R("b");
    a = new R("a");
  }
  f(a, b);
  writeln("end");
}
test5();

proc test5eob() {
  writeln("test5eob");
  var a;
  var b;
  if cond {
    b = new R("b0");
    a = new R("a0");
  } else {
    b = new R("b");
    a = new R("a");
  }
  ref ra = a;
  ref rb = b;
}
test5eob();

proc test6() {
  writeln("test6");
  var a;
  var b;
  {
    if cond {
      b = new R("b0");
      a = new R("a0");
    } else {
      b = new R("b");
      a = new R("a");
    }
  }
  f(a, b);
  writeln("end");
}
test6();

proc test6eob() {
  writeln("test6eob");
  var a;
  var b;
  {
    if cond {
      b = new R("b0");
      a = new R("a0");
    } else {
      b = new R("b");
      a = new R("a");
    }
  }
  ref ra = a;
  ref rb = b;
}
test6eob();

proc test7() {
  writeln("test7");
  var a;
  var b;
  if otherCond then
    return;
  b = new R("b");
  a = new R("a");
  f(a, b);
  writeln("end");
}
test7();

proc test7eob() {
  writeln("test7eob");
  var a;
  var b;
  if otherCond then
    return;
  b = new R("b");
  a = new R("a");
  ref ra = a;
  ref rb = b;
}
test7eob();

proc test8() {
  writeln("test8");
  var a;
  var b;
  b = new R("b");
  if otherCond then
    return;
  a = new R("a");
  f(a, b);
  writeln("end");
}
test8();

proc test8eob() {
  writeln("test8eob");
  var a;
  var b;
  b = new R("b");
  if otherCond then
    return;
  a = new R("a");
  ref ra = a;
  ref rb = b;
}
test8eob();

proc test9() {
  writeln("test9");
  var a;
  var b;
  b = new R("b");
  a = new R("a");
  if otherCond then
    return;
  f(a, b);
  writeln("end");
}
test9();

proc test9eob() {
  writeln("test9eob");
  var a;
  var b;
  b = new R("b");
  a = new R("a");
  if otherCond then
    return;
  ref ra = a;
  ref rb = b;
}
test9eob();
