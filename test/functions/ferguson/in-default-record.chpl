record R {
  var x:int;

  proc init(arg:int) {
    writeln("R.init ", arg); 
    this.x = arg;
  }

  proc init=(from:R) {
    writeln("R.init copy ", from.x);
    this.x = from.x;
  }

  proc deinit() {
    writeln("R.deinit ", this.x);
  }
}

proc =(ref lhs:R, rhs:R) {
  writeln("= ", lhs.x, " ", rhs.x);
  lhs.x = rhs.x;
}


proc makeR(arg:int) {
  return new R(arg);
}

proc bar(const ref x) const ref {
  return x;
}

proc foo(in x = new R(1), const ref y = bar(x)) {
  writeln("starting foo, x=", x, " y=", y);
  x.x += 100;
  writeln("ending   foo, x=", x, " y=", y);
}

proc test1() {
  writeln("test1");
  foo();
}

proc test2() {
  writeln("test2");
  foo(new R(2));
}

proc test3() {
  writeln("test3");
  foo(makeR(3));
}

proc test4() {
  writeln("test4");
  var c = new R(4);
  foo(c);
}

test1();
test2();
test3();
test4();
