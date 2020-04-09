proc foo(ref x) {
  x += 1;
}

var x = 23;

foo(x);
writeln("x is: ", x);
