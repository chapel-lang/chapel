function foo(i : int) {
  writeln(i);
}

function foo(i : int, s : string = "hello") {
  writeln(i);
  writeln(s);
}

foo(17);
foo(17, "hi there");
