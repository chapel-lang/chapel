record R { }

class C {
  param r:R;
  proc init() { }
}

var c = (new owned C()).borrow();
writeln(c);
