record A {
  var i : integer = 1;
  var f : float = 2.0;
  var s : string = "three";
}

var a1 : A;
var a2 : A;

writeln("a1 is ", a1);
writeln("a2 is ", a2);

a2.i = 12;
a2.f = 13.0;
a2.s = "fourteen";

writeln("a1 is ", a1);
writeln("a2 is ", a2);

a1 = a2;
a2 = A();

writeln("a1 is ", a1);
writeln("a2 is ", a2);
