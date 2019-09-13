class C { var x: int; }

proc foo(arg: owned C) { }

proc test1() {
  var x = new owned C();
  foo(x);
}
test1();
