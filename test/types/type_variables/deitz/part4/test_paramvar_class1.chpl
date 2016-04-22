class foo {
  param x : int;
  var str : string;
}

var f1 : foo(2) = new foo(x=2);
var f2 : foo(3) = new foo(x=3);

f1.str = "hello";
f2.str = "world";

writeln("{x = ", f1.x, ", str = ", f1.str, "}");
writeln("{x = ", f2.x, ", str = ", f2.str, "}");

delete f2;
delete f1;
