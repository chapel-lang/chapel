class C {
  type t;
  var f : t;
}

var cObj = new C(f = 1);
var c = cObj.borrow();
writeln(c);

var dObj = new C(int, 12);
var d : borrowed C(int) = dObj.borrow();
writeln(d);
