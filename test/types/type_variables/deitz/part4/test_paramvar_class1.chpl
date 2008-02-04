class foo {
  param x : int;
  var str : string;
}

var f1 : foo(2) = new foo(x=2);
var f2 : foo(3) = new foo(x=3);

f1.str = "hello";
f2.str = "world";

writeln(f1);
writeln(f2);
