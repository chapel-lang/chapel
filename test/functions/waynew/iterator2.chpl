// Test iterator->class transformation with more than 1 argument.

iter jam( min: int, max: int): int {
  var i, j: int;
  i = min;
  while (i <= max) {
    if (i % 2) {
      yield i;
    } else {
      yield i*2;
    }
    i = i + 1;
  }

  j = min*2;
  while (j <= max/2) {
    yield j;
    j = j + 1;
  }
}


proc test1() {
  for k in jam( 0, 10) {
    write( " ", k);
  }
  writeln();
}
      
test1();
