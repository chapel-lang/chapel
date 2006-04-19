record _tuple {
  param size : int;
  var component ...size;

  fun this(param i : int) var
    return component(i);

  fun this(i : int) var
    return component(i);
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

record _htuple {
  type elt_type;
  param size : int;
  var elements : _ddata(elt_type) = _ddata(elt_type, size);
  fun initialize() {
    elements.init();
  }
  fun this(i : int) var : elt_type {
    if i < 1 || i > size then
      halt("tuple indexing out-of-bounds error");
    return elements(i-1);
  }
}

fun =(x : _htuple, y) {
  for param i in 1..x.size do
    x(i) = y(i);
  return x;
}

fun fwrite(f : file, x : _htuple) {
  fwrite(f, "(", x(1));
  for param i in 2..x.size do
    fwrite(f, ", ", x(i));
  fwrite(f, ")");
}
