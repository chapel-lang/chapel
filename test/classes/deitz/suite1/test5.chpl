class C {
  type t;
  var x : t;
}

var c : borrowed C(t = int) = (new owned C(t = int)).borrow();
c.x = 12;
writeln(c);
