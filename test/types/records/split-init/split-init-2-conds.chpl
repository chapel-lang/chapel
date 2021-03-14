class C { var x: int; }

config const option = true;

proc test1() {
  writeln("test1");
  var a;
  var b;
  if option then
    a = new C(1);
  else
    a = new C(2);
  if !option then
    b = new C(3);
  else
    b = new C(4);

  writeln(a.type:string, " ", a);
  writeln(b.type:string, " ", b);
}
test1();

record R {
  var x: int = 0;
  var ptr: owned C = new owned C();
  proc init() {
    this.x = 0;
    writeln("init");
  }
  proc init(arg:int) {
    this.x = arg;
    this.ptr = new owned C(arg);
    writeln("init ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    writeln("init= ", other.x);
  }
}
proc =(ref lhs:R, rhs:R) {
  writeln("= ", lhs.x, " ", rhs.x);
  lhs.x = rhs.x;
}

proc test2() {
  writeln("test2");
  var a;
  if option then
    return;
  else
    a = new R(1);
}
test2();

proc test3() {
  writeln("test3");
  var a;
  if !option then
    return;
  else
    a = new R(1);
}
test3();

proc test4() throws {
  writeln("test4");
  var a;
  if option then
    a = new R(1);
  else
    throw new Error();
}
try { test4(); } catch e { writeln(e); }

proc test5() throws {
  writeln("test5");
  var a;
  if !option then
    a = new R(1);
  else
    throw new Error();
}
try { test5(); } catch e { writeln(e); }

proc test6() throws {
  writeln("test6");
  var a;
  {
    if option {
      {
        if option {
          return;
        } else {
          throw new Error();
        }
      }
    } else {
      a = new R(1);
    }
  }
}
try { test6(); } catch e { writeln(e); }

proc test7() throws {
  writeln("test7");
  var a;
  {
    if !option {
      {
        if option {
          return;
        } else {
          throw new Error();
        }
      }
    } else {
      a = new R(1);
    }
  }
}
try { test7(); } catch e { writeln(e); }
