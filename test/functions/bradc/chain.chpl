function foo(a) {
  writeln("in foo, a is ", a);
  bar(7);
}

function bar(a) {
  writeln("in bar, a is ", a);
}

foo(3);
bar(5);

