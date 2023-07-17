class C {
  type t;
  var f : t;
}

var cObj = new C(f = 1);
var c = cObj.borrow();
writeln(c);
