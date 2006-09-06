pragma "destructure" pragma "tuple" record _tuple {
  param size : int;

  pragma "inline" pragma "tuple get" def this(param i : int)
    return 0;

  pragma "inline" pragma "tuple set" def =this(param i : int, y);

  def this(i : int) var {
    for param j in 1..size do
      if i == j then
        return this(j);
    halt("tuple indexing out-of-bounds error");
  }
}

pragma "inline" def =(x : _tuple, y) {
  for param i in 1..x.size do
    x(i) = y(i);
  return x;
}

def fwrite(f : file, x : _tuple) {
  fwrite(f, "(", x(1));
  for param i in 2..x.size do
    fwrite(f, ", ", x(i));
  fwrite(f, ")");
}

def _seq_to_tuple(s: seq, param i: int) {
  var t: i*s.elt_type;
  for param j in 1..i do
    t(j) = s(j);
  return t;
}

def _tuple_to_seq(t: _tuple) {
  var s = (/ t(1) /);
  for param j in 2..t.size do
    s #= t(j);
  return s;
}
