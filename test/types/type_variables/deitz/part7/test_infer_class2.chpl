class C {
  type t;
  var f : t;
}

var c = (new owned C(f = 1)).borrow();

writeln(c);

var d : borrowed C(int) = (new owned C(int, 12)).borrow();

writeln(d);
