iter foo(): int {
  writeln("In foo");
}

var A = foo();
writeln(A);
writeln(A.domain);
