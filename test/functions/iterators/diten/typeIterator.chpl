iter foo() type {
  yield int;
  yield int;
}

for t in foo() {
  writeln(t); // should be writeln(t:string);
}
