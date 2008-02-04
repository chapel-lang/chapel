record A {
  var i : int;
}

var a1 : A = new A();
var a2 : A = new A();
a1.i = 1;
a2.i = 2;

writeln("1 = ", a1 != a2);

a2.i = 1;

writeln("0 = ", a1 != a2);
