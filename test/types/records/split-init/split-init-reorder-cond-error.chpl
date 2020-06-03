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
  if cond {
    b = new R("b");
    a = new R("a");
  } else {
    a = new R("a");
    b = new R("b");
  }
  f(a, b);
  writeln("end");
}
test1();

proc test1eob() {
  writeln("test1eob");
  var a;
  var b;
  if cond {
    b = new R("b");
    a = new R("a");
  } else {
    a = new R("a");
    b = new R("b");
  }

  ref ra = a;
  ref rb = b;
}
test1eob();

proc test2() {
  writeln("test2");
  var a;
  var b;
  var c;
  var d;
  {
    if cond {
      a = new R("a");
      b = new R("b");
      c = new R("c");
      d = new R("d");
    } else {
      {
        a = new R("a");
        c = new R("c");
        b = new R("b");
        d = new R("d");
      }
    }
  }
  f(a, b);
  writeln("end");
}
test2();

proc test2eob() {
  writeln("test2eob");
  var a;
  var b;
  var c;
  var d;
  {
    if cond {
      a = new R("a");
      b = new R("b");
      c = new R("c");
      d = new R("d");
    } else {
      {
        a = new R("a");
        c = new R("c");
        b = new R("b");
        d = new R("d");
      }
    }
  }
  ref ra = a;
  ref rb = b;
}
test2eob();
