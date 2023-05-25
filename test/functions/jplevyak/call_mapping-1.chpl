class C {
  var a : int;
}
class D {
  var aa : int;
  proc a(i : int) ref { return aa; }
}
var d : domain(1) = {1..3};
class E {
  var a : [d] int;
}

var a : [d] int;
var x = 1;
var z = 1;
var Y1 = (new owned C()).borrow();
var Y2 = (new owned D()).borrow();
var Y3 = (new owned E()).borrow();

a(x) = z;
writeln(a(x));
Y1.a = z;
writeln(Y1.a);
Y2.a(2) = z;
writeln(Y2.a(2));
Y3.a(2) = z;
writeln(Y3.a(2));
