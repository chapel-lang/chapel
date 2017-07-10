// Can't yield non-const vars by const ref.

var globalVar: int;

iter MYITER() const ref {
  yield globalVar;  // illegal
}

proc main {
  for IDX in MYITER() {
    writeln(IDX);
  }
}
