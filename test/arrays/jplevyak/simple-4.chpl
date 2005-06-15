
type arr : [3] integer;

class C {
  var a : integer;
}
class D {
  var aa : integer;
  function a(i : integer) { return aa; }
  function =a(i : integer) { aa = i; }
}
class E {
  var a : arr;
}

var a : arr;
var x = 1;
var z = 1;
/*
var y1 = C();
var y2 = D();
var y3 = E();
*/

a(x) = z;
/*
writeln(a(x));
y1.a = z;
writeln(y1.a);
y1.a() = z;
writeln(y1.a());
y2.a(2) = z;
writeln(y2.a(2));
y3.a()(2) = z;
writeln(y3.a()(2));
*/
