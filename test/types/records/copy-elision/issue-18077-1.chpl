class C {
  var x: int;
}

proc foo(): (owned C, owned C) {
  return (new owned C(45), new owned C(33));
}

var (x, y) = foo();
writeln((x,y));
