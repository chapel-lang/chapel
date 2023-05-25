class C {
  type t;
  var f : t;
}

var c = (new owned C(f = 1)).borrow();

writeln(c);
