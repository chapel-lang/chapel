pragma "tuple" record _tuple {
  param size : int;

  pragma "tuple get" fun this(param i : int)
    return 0;

  pragma "tuple set" fun =this(param i : int, y);

  fun this(i : int) {
    for param j in 1..size do
      if i == j then
        return this(j);
    halt("tuple indexing out-of-bounds error");
  }

  fun =this(i : int, y) {
    var good = false;
    for param j in 1..size do
      if i == j {
        this(j) = y;
        good = true;
      }
    if !good then
      halt("tuple indexing out-of-bounds error");
  }
}

fun =(x : _tuple, y) {
  for param i in 1..x.size do
    x(i) = y(i);
  return x;
}

fun fwrite(f : file, x : _tuple) {
  fwrite(f, "(", x(1));
  for param i in 2..x.size do
    fwrite(f, ", ", x(i));
  fwrite(f, ")");
}
