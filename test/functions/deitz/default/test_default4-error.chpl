function foo(i : integer = 1, j : float = 2.0, k : string = "three") {
  writeln("foo of ", i, ", ", j, ", ", k);
}

function foo(i : float = 1.0, j : string = "two", k : integer = 3) {
  writeln("foo of ", i, ", ", j, ", ", k);
}

function foo(i : string = "one", j : integer = 2, k : float = 3.0) {
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
