class A { var a : integer; }
class B { var b : float; }
var a = A(1);
var b = B(2.0);
function f(x : A, y) { writeln(x.a + y); }
function f(x : B, y) { writeln(x.b + y); }
var c;
c = => f(a);
c(3);
c = => f(b);
c(4);
