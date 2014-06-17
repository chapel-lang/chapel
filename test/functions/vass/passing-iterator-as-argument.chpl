proc ti(it) {
  for i in it() do write(i); writeln();
}
iter i11a() {
  yield "11a";
}
ti(i11a);
