function foo(a) {
  writeln("in foo");
  bar(1)
}

function bar(a) {
  writeln("in bar");
}

foo(0);
bar(0);

