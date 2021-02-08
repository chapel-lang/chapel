proc foo(): void {
  writeln("In foo()");
}

proc bar(): void {
  writeln("In bar()");
  return foo();
}

bar();
