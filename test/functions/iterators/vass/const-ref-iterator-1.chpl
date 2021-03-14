// Can't yield non-const vars by const ref.

const globalVar = 5;

proc refToGlobalVar() const ref {
  return globalVar;
}

iter MYITER() ref {
  yield globalVar;         // illegal
  yield refToGlobalVar();  // illegal
}

proc main {
  for IDX in MYITER() {
    writeln(IDX);
    IDX += 10;
  }
}
