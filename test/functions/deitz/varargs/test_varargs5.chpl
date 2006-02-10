function foo(s : string) {
  write("string ", s);
}

function foo(i : integer) {
  write("integer ", i);
}

function foo(ys ...2) {
  foo(ys(1));
  write(", ");
  foo(ys(2));
  writeln();
}

foo(2, "hello world");
foo(2, 3);
foo("hello", "world");
