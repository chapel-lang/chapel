

function foo(out x) {
  x = 7;
}

var y;
foo(y);
writeln("expect 7 ", y);
