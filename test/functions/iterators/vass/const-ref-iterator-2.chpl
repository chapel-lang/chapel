// Can't write to the index variable
// when the loop's iterator is 'const ref'.

const globalVar: int;

iter MYITER() const ref {
  yield globalVar;
}

proc main {
  for IDX in MYITER() {
    IDX = 5;  // illegal
  }
  writeln(globalVar);
}
