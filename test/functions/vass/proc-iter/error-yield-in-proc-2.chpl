iter i11i() {
  proc prc() {
    yield 1;
  }
  yield prc();
}
for i in i11i() do write(i);
