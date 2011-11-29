proc foo(i : int = 1, j : real = 2.0, k : string = "three") {
  writeln("foo of ", i, ", ", j, ", ", k);
}

proc foo(i : real = 1.0, j : string = "two", k : int = 3) {
  writeln("foo of ", i, ", ", j, ", ", k);
}

proc foo(i : string = "one", j : int = 2, k : real = 3.0) {
  writeln("foo of ", i, ", ", j, ", ", k);
}

foo(4, 5.0, "six");
foo(4, 5.0);
foo(4);

foo(4.0, "five", 6);
foo(4.0, "five");
foo(4.0);

foo("four", 5, 6.0);
foo("four", 5);
foo("four");

foo();
