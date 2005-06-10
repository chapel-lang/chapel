function foo(s) {

  writeln("s is: ", s);

  return s;
}

var t: seq(integer) = (/ 1, 2, 3 /);
var a = foo(t);
writeln("a is: ", a);
t = t # 4;
writeln("t is: ", t);
writeln("a is: ", a);
