proc foo(i : int, s : string = "hello") {
  writeln(i);
  writeln(s);
}

foo(17);
foo(17, "hi there");
