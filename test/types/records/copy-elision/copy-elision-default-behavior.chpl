// Similar to copy-elision but uses a record
// with default init= and =

config param printInitDeinit = true;

class C {
  var xx: int = 0;
  proc init(arg: int) {
    this.xx = arg;
    if printInitDeinit then writeln("C.init ", arg);
  }
  proc deinit() {
    if printInitDeinit then writeln("C.deinit ", xx);
  }
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
