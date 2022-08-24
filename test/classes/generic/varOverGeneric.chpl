class C {
  type t;
  var x: t;
}

var myC: borrowed C = (new owned C(int, 1)).borrow();
writeln(myC);
