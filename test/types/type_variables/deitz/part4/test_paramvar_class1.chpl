class foo {
  param x : int;
  var str : string;
}

var ownF1 = new owned foo(x=2);
var f1 : borrowed foo(2) = ownF1.borrow();
var ownF2 = new owned foo(x=3);
var f2 : borrowed foo(3) = ownF2.borrow();

f1.str = "hello";
f2.str = "world";

writeln("{x = ", f1.x, ", str = ", f1.str, "}");
writeln("{x = ", f2.x, ", str = ", f2.str, "}");
