class foo {
  param x : integer;
  var str : string;
}

var f1 : foo(2) = foo(x=2);
var f2 : foo(3) = foo(x=3);

f1.str = "hello";
f2.str = "world";

writeln(f1);
writeln(f2);
