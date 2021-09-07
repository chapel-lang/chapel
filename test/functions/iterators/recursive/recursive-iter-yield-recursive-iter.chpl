iter recursivator(n: int): int {
  for i in 0..#n {
    yield recursivator(i)+1;
  }
  yield 0;
}

for i in recursivator(2) {
}
