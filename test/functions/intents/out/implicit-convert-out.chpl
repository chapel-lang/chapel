// This test shows some examples from issue #16582

proc int8Out(out arg: int(8)) {
  writeln("in int8Out arg=", arg);
  arg = 2;
}

proc test1() {
  writeln("test1");
  var myInt: int = 1;
  int8Out(myInt); // should this call resolve?
  writeln(myInt);
}
test1();

record R {
  var x: int = 0;
  proc init=(x: int) {
    writeln("In init=");
    this.x = x;
  }
}

proc =(ref r: R, x: int) {
  writeln("In assign");
  r.x = x;
}
operator :(x: int, type toType:R ) {
  var tmp: R = x;
  return tmp;
}


proc foo_ret(): int {
  return 33;
}

proc test2() {
  writeln("test2");
  var a:R;
  a = foo_ret(); // works today - split initializes and calls R.init=
  var b:R = new R();
  b = foo_ret(); // works today - calls = overload
  writeln(a);
  writeln(b);
}
test2();


proc foo_out(out x: int) {
  x = 45;
}

proc test3() {
  writeln("test3");
  var c:R;
  foo_out(c); // Doesn't resolve today due to arg mismatch
  var d:R = new R();
  foo_out(d); // Doesn't resolve today due to arg mismatch
  writeln(c);
  writeln(d);
}
test3();
