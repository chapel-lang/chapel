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
var ownY1 = new owned C();
var Y1 = ownY1.borrow();
var ownY2 = new owned D();
var Y2 = ownY2.borrow();
var ownY3 = new owned E();
var Y3 = ownY3.borrow();

a(x) = z;
writeln(a(x));
Y1.a = z;
writeln(Y1.a);
Y2.a(2) = z;
writeln(Y2.a(2));
Y3.a(2) = z;
writeln(Y3.a(2));
