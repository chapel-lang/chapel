class A { var a : int; }
class B { var b : float; }
var a = A(1);
var b = B(2.0);
fun f(x : A, y) { writeln(x.a + y); }
fun f(x : B, y) { writeln(x.b + y); }
var c = => f(a);
c(3);
c = => f(b);
c(4);
