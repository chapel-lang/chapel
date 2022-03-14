class C {
  var x: int;
}

var (x, y) = (new owned C(45), new owned C(33));
writeln((x,y));
