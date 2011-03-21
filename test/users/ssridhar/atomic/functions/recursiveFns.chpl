var x: int;

proc main() {
  atomic {
    foo();
  }
  writeln("x = ", x);
}

proc foo() {
  bar();
}

proc bar() {
  x += 1;
  if x < 5 then bar();
}