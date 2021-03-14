
type arr = [1..3] int;

class C {
  var a : int;
}
class D {
  var aa : int;
  proc a(i : int) ref { return aa; }
}
class E {
  var a : arr;
}

var a : arr;
var x = 1;
var z = 1;

var y1 = new owned C();
var y2 = new owned D();
var y3 = new owned E();


a(x) = z;
writeln(a(x));
y1.a = z;
writeln(y1.a);
y2.a(2) = z;
writeln(y2.a(2));
y3.a(2) = z;
writeln(y3.a(2));
