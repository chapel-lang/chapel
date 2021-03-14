proc foo(s : string) {
  write("string ", s);
}

proc foo(i : int) {
  write("int ", i);
}

proc foo(ys ...2) {
  foo(ys(0));
  write(", ");
  foo(ys(1));
  writeln();
}

foo(2, "hello world");
foo(2, 3);
foo("hello", "world");
