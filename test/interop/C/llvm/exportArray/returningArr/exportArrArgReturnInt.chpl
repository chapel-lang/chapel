export proc numEven(x: [] int): int {
  var evens = 0;
  for i in x {
    if (i % 2 == 0) {
      evens += 1;
    }
  }
  return evens;
}
