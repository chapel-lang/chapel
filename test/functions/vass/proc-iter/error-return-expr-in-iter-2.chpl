proc p11d() {
  iter i11h() {
    yield 1;
    return 2;
  }
  for i in i11h() do write(i);
}
p11d();
