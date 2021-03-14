class foo {
  param x : int;
  var str : string;
}

var f1 : borrowed foo(2) = new borrowed foo(x=2);
var f2 : borrowed foo(3) = new borrowed foo(x=3);

f1.str = "hello";
f2.str = "world";

writeln("{x = ", f1.x, ", str = ", f1.str, "}");
writeln("{x = ", f2.x, ", str = ", f2.str, "}");
