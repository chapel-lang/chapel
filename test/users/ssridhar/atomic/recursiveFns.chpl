var x: int; 

proc main() {
  writeln("In main before foo x = ", x);
  atomic {
    foo();
  }
  writeln("In main after foo x = ", x);
}

proc foo() {
  bar();
}

proc bar() {
  x += 1;
  if x < 5 then bar();
}