export proc printIfEmptyArr(const ref x: [] int) {
  if (x.size == 0) {
    writeln("EMPTY! YAY!");
  } else {
    writeln("awwww");
  }
}
