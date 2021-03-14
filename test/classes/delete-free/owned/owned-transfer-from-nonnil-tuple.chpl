class C { var x: int; }

proc test() {
  var x = new owned C(1);
  return (x, x);
}

var t = test();
writeln(t);
