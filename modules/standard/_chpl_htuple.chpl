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

  def _getHeadCursorHelp(param i: int, c: _tuple) {
    if (i <= size) {
      var cc = ((...c), this(i).getHeadCursor());
      return _getHeadCursorHelp(i+1, cc);
    } else {
      return c;
    }
  }

  def getHeadCursor() {
    var c = this(1).getHeadCursor();
    var ct : 1*c;
    ct(1) = c;
    return _getHeadCursorHelp(2, ct);
  }

  def getNextCursor(c) {
    for param i in 1..size do
      c(i) = this(i).getNextCursor(c(i));
    return c;
  }

  def _getValueHelp(param i: int, v: _tuple, c: _tuple) {
    if (i <= size) {
      var vv = ((...v), this(i).getValue(c(i)));
      return _getValueHelp(i+1, vv, c);
    } else {
      return v;
    }
  }

  def getValue(c) {
    var v = this(1).getValue(c(1));
    var vt : 1*v;
    vt(1) = v;
    return _getValueHelp(2, vt, c);
  }

  def isValidCursor?(c) {
    for param i in 1..size do
      if !this(i).isValidCursor?(c(i)) then
        return false;
    return true;
  }
}

pragma "inline" def =(x : _tuple, y) {
  for param i in 1..x.size do
    x(i) = y(i);
  return x;
}

def ==( a: _tuple, b: _tuple): bool {
  if (a.size != b.size) then
    return false;
  for param i in 1..a.size do
    if (a(i) != b(i)) then
      return false;
  return true;
}

def !=( a: _tuple, b: _tuple): bool {
  if (a.size != b.size) then
    return true;
  for param i in 1..a.size do
    if (a(i) != b(i)) then
      return true;
  return false;
}

def _tuple.writeThis(f: Writer) {
  f.write("(", this(1));
  for param i in 2..size do
    f.write(", ", this(i));
  f.write(")");
}

def _seq_to_tuple(s: seq, param i: int) {
  var t: i*s.eltType;
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

pragma "inline" def _cast(type t, x: _tuple) var where (t == complex(64)) & (x.size == 2) {
  var c: complex(64);
  c.re = x(1);
  c.im = x(2);
  return c;
}

pragma "inline" def _cast(type t, x: _tuple) var where (t == complex(128)) & (x.size == 2) {
  var c: complex(128);
  c.re = x(1);
  c.im = x(2);
  return c;
}

pragma "inline" def _cast(type t, x: _tuple) var where (t == complex(256)) & (x.size == 2) {
  var c: complex(256);
  c.re = x(1);
  c.im = x(2);
  return c;
}

record _square_tuple {
  param size: int;
  var tuple;

  def getHeadCursor()
    return tuple.getHeadCursor();

  def getNextCursor(c) {
    for param i in 1..size {
      c(size-i+1) = tuple(size-i+1).getNextCursor(c(size-i+1));
      if tuple(size-i+1).isValidCursor?(c(size-i+1)) then
        return c;
      else if i < size then
        c(size-i+1) = tuple(size-i+1).getHeadCursor();
    }
    return c;
  }

  def getValue(c)
    return tuple.getValue(c);

  def isValidCursor?(c)
    return tuple.isValidCursor?(c);
}

def _build_domain(x ...?size) where size > 1
  return _square_tuple(size, x);

def _build_tuple(x ...?size) {
  if size == 1 then
    return x(1);
  else
    return x;
}
