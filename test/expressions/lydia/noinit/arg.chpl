// The error message for this case is not as helpful as I would like

proc foo (a: int(64) = noinit) {
  if (a == 0) {
    writeln("coooooool");
  } else {
    a = 0;
  }
}

foo(0);
foo(1);
foo();
