config param printInitDeinit = true;

class C {
  var xx: int = 0;
}

record R {
  var x: int = 0;
  var ptr: shared C = new shared C(0);
  proc init() {
    this.x = 0;
    this.ptr = new shared C(0);
    if printInitDeinit then writeln("init (default)");
  }
  proc init(arg:int) {
    this.x = arg;
    this.ptr = new shared C(arg);
    if printInitDeinit then writeln("init ", arg, " ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    this.ptr = new shared C(other.ptr.xx);
    if printInitDeinit then writeln("init= ", other.x, " ", other.ptr.xx);
  }
  proc deinit() {
    if printInitDeinit then writeln("deinit ", x, " ", ptr.xx);
  }
  proc toString() {
    return "(" + this.x:string + " " + this.ptr.xx:string + ")";
  }
  proc ref set1() ref {
    this.x = 1;
    this.ptr.xx = 1;
    return this;
  }
}
proc =(ref lhs:R, rhs:R) {
  if printInitDeinit then writeln("lhs", lhs.toString(), " = rhs", rhs.toString());
  lhs.x = rhs.x;
  lhs.ptr = new shared C(rhs.ptr.xx);
}

proc set1f(ref arg: R) ref {
  arg.set1();
  return arg;
}

proc concatenate(a: R, b: R): string {
  return a.toString() + " " + b.toString();
}

proc concatenate(a: R, b: string): string {
  return a.toString() + " " + b;
}

proc concatenate(a: R, b: borrowed C): string {
  return a.toString() + " " + b.xx:string;
}

proc concatenate(a: R, b: int): string {
  return a.toString() + " " + b:string;
}

proc copy(in arg) {
  return arg;
}

proc acceptTwoIn(in a, in b) {
  return concatenate(a, b);
}

var globalR:R;

proc test1a() {
  writeln("test1a");
  var x = new R(1);
  writeln("test1a.a");
  var y = x;
  writeln("test1a.b");
  writeln(y.toString());
}
test1a();

proc test1b() {
  writeln("test1b");
  var x = new R(1);
  writeln("test1b.a");
  var y = x;
  writeln("test1b.b");
  return y;
}
{ writeln(test1b().toString()); }

proc test1c() {
  writeln("test1c");
  var x = new R(1);
  writeln("test1c.a");
  var y = x;
  writeln("test1c.b");
  return copy(y);
}
{ writeln(test1c().toString()); }

proc test1d() {
  writeln("test1d");
  var x = new R(1);
  writeln("test1d.a");
  writeln(copy(x).toString());
  writeln("test1d.b");
}
test1d();

proc test1e() {
  writeln("test1e");
  var x = new R(1);
  writeln("test1e.a");
  return copy(x);
}
{ writeln(test1e().toString()); }

proc test3a() {
  writeln("test3a");
  var x = new R(1);
  writeln("test3a.a");
  on Locales[numLocales-1] {
    writeln("test3a.b");
    var y = x;
    writeln(y.toString());
    assert(y.locale == here);
    assert(y.ptr.xx.locale == here);
  }
  writeln("test3a.c");
}
test3a();

proc test3b() {
  writeln("test3b");
  var x = new R(1);
  writeln("test3b.a");
  sync {
    begin on Locales[numLocales-1] {
      writeln("test3b.b");
      var y = x;
      writeln(y.toString());
      assert(y.locale == here);
      assert(y.ptr.xx.locale == here);
    }
  }
  writeln("test3b.c");
}
test3b();

proc test3c() {
  writeln("test3c");
  var s: sync int;
  var x = new R(1);
  writeln("test3c.a");
  begin on Locales[numLocales-1] {
    writeln("test3c.b");
    var y = x;
    writeln(y.toString());
    assert(y.locale == here);
    assert(y.ptr.xx.locale == here);
    s = 1;
  }
  s;
}
sync { test3c(); }

proc test4a() {
  writeln("test4a");
  var x = new R(1);
  writeln("test4a.a");
  sync {
    begin {
      writeln("test4a.b");
      var y = x;
      writeln(y.toString());
    }
  }
  writeln("test4a.c");
}
test4a();

proc test5a() {
  writeln("test5a");
  var x = new R(1);
  var y = new R(0);
  writeln("test5a.a");
  y = x;
  writeln("test5a.b");
  writeln(y.toString());
}
test5a();

pragma "unsafe" // avoid lifetime errors for this function
proc test5b() {
  writeln("test5b");
  var x = new R(1);
  writeln("test5b.a");
  var b = x.ptr.borrow();
  writeln(concatenate(copy(x), b));
}
test5b();

proc test5c() {
  writeln("test5c");
  var x = new R(1);
  writeln("test5c.a");
  const ref rx = x.ptr.xx;
  writeln(concatenate(copy(x), rx));
}
test5c();

writeln("end");

proc test6() {
  writeln("test6");
  var x = new R(1);
  writeln("test6.a");
  writeln(acceptTwoIn(x, x));
}
test6();

proc test7a() {
  writeln("test7a");
  var x = new R(0);
  writeln("test7a.a");
  var y = x;
  writeln("test7a.b");
  writeln(concatenate(x.set1(), y));
}
test7a();

proc test7b() {
  writeln("test7b");
  var x = new R(0);
  writeln("test7b.a");
  var y = x;
  writeln("test7b.b");
  return concatenate(x.set1(), y);
}
{ writeln(test7b()); }

proc test7c() {
  writeln("test7c");
  var x = new R(0);
  writeln("test7c.a");
  var y = x;
  writeln("test7c.b");
  writeln(concatenate(set1f(x), y));
}
test7c();

proc test7d() {
  writeln("test7d");
  var x = new R(0);
  writeln("test7d.a");
  var y = x;
  writeln("test7d.b");
  return concatenate(set1f(x), y);
}
{ writeln(test7d()); }


proc test8() {
  writeln("test8");
  var x = globalR;
}
test8();

proc test9() {
  writeln("test9");
  var x = new R(1);
  proc inner() {
    var y = x;
  }
  inner();
}
test9();

proc test10() {
  writeln("test10");
  var x = new R(1);
  const ref r = x;
  var y = r;
}
test10();

proc test11() {
  writeln("test11");
  var x = new R(1);
  ref r = x;
  var y = r;
}
test11();

proc test12() {
  writeln("test12");
  var x = new R(1);
  var y = x;
  var z = y;
}
test12();

proc test13() {
  writeln("test13");
  for i in 1..1 {
    var x = new R(1);
    var y = x;
  }
}
test13();

proc test14() {
  writeln("test14");
  coforall i in 1..1 {
    var x = new R(1);
    var y = x;
  }
}
test14();

proc test15() {
  writeln("test15");
  forall i in 1..1 {
    var x = new R(1);
    var y = x;
  }
}
test15();

proc test16() {
  writeln("test16");
  var i = 1;
  do {
    var x = new R(1);
    var y = x;
    i += 1;
  } while (i <= 1);
}
test16();

proc test17() {
  writeln("test17");
  var i = 1;
  while (i <= 1) {
    var x = new R(1);
    var y = x;
    i += 1;
  }
}
test17();

proc test18() {
  writeln("test18");
  cobegin {
    {
      var x = new R(1);
      var y = x;
    }
    {
      // do nothing
    }
  }
}
test18();

proc test19() {
  writeln("test19");
  sync {
    begin {
      var x = new R(1);
      var y = x;
    }
  }
}
test19();

proc test20() {
  writeln("test20");

  var done$: sync int;

  begin {
    {
      var x = new R(1);
      var y = x;
    }
    done$ = 1;
  }

  done$; // wait
}
test20();

writeln("test-public-global");
public var global1  = globalR;

writeln("test-private-global");
private var global2 = globalR;

writeln("test-default-global");
var global3 = globalR;

writeln("end");
