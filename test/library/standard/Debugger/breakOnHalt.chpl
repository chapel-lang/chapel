config const myVar = 42;
proc foo(x) throws {
  if x > 0 {
    halt("x is greater than 0");
  } else {
    throw new Error("x is not greater than 0");
  }
}

proc main() {
  foo(myVar);
}
