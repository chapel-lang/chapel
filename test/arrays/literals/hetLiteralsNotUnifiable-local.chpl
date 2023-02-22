proc foo() {
  bar();
}

proc bar() {
  var A = [1.0, true, "hi"];
  writeln(A, ": ", A.type:string);
}

foo();

