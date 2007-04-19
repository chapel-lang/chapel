pragma "scalar replace tuples" pragma "tuple" record _tuple {
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

  def isValidCursor(c) {
    for param i in 1..size do
      if !this(i).isValidCursor(c(i)) then
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

pragma "inline" def +(a: _tuple) {
  return a;
}

pragma "inline" def -(a: _tuple) {
  for param i in 1..a.size do
    a(i) = -a(i);
  return a;
}

pragma "inline" def +(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) + b, a(a.size) + b);
    } else {
      var rest = help(i+1);
      return (a(i) + b, (...rest));
    }
  }
  if a.size == 1 then
    return a(1) + b;
  else
    return help(1);
}

pragma "inline" def +(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size-1 {
      return (a + b(i), a + b(b.size));
    } else {
      var rest = help(i+1);
      return (a + b(i), (...rest));
    }
  }
  if b.size == 1 then
    return a + b(1);
  else
    return help(1);
}

pragma "inline" def +(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) + b(i), a(a.size) + b(b.size));
    } else {
      var rest = help(i+1);
      return (a(i) + b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  if (a.size == 1)
    return a(1)+b(1);
  else
    return help(1);
}

pragma "inline" def -(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) - b, a(a.size) - b);
    } else {
      var rest = help(i+1);
      return (a(i) - b, (...rest));
    }
  }
  if a.size == 1 then
    return a(1) - b;
  else
    return help(1);
}

pragma "inline" def -(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size-1 {
      return (a - b(i), a - b(b.size));
    } else {
      var rest = help(i+1);
      return (a - b(i), (...rest));
    }
  }
  if b.size == 1 then
    return a - b(1);
  else
    return help(1);
}

pragma "inline" def -(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) - b(i), a(a.size) - b(b.size));
    } else {
      var rest = help(i+1);
      return (a(i) - b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  if a.size == 1 then
    return a(1) - b(1);
  else
    return help(1);
}

pragma "inline" def *(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) * b, a(a.size) * b);
    } else {
      var rest = help(i+1);
      return (a(i) * b, (...rest));
    }
  }
  if a.size == 1 then
    return a(1) * b;
  else
    return help(1);
}

pragma "inline" def *(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size-1 {
      return (a * b(i), a * b(b.size));
    } else {
      var rest = help(i+1);
      return (a * b(i), (...rest));
    }
  }
  if b.size == 1 then
    return a * b(1);
  else
    return help(1);
}

pragma "inline" def *(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) * b(i), a(a.size) * b(b.size));
    } else {
      var rest = help(i+1);
      return (a(i) * b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  if a.size == 1 then
    return a(1) * b(1);
  else
    return help(1);
}

pragma "inline" def /(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) / b, a(a.size) / b);
    } else {
      var rest = help(i+1);
      return (a(i) / b, (...rest));
    }
  }
  if a.size == 1 then
    return a(1) / b;
  else
    return help(1);
}

pragma "inline" def /(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size-1 {
      return (a / b(i), a / b(b.size));
    } else {
      var rest = help(i+1);
      return (a / b(i), (...rest));
    }
  }
  if b.size == 1 then
    return a / b(1);
  else
    return help(1);
}

pragma "inline" def /(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) / b(i), a(a.size) / b(b.size));
    } else {
      var rest = help(i+1);
      return (a(i) / b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  if a.size == 1 then
    return a(1) / b(1);
  else
    return help(1);
}

pragma "inline" def %(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) % b, a(a.size) % b);
    } else {
      var rest = help(i+1);
      return (a(i) % b, (...rest));
    }
  }
  if a.size == 1 then
    return a(1) % b;
  else
    return help(1);
}

pragma "inline" def %(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size-1 {
      return (a % b(i), a % b(b.size));
    } else {
      var rest = help(i+1);
      return (a % b(i), (...rest));
    }
  }
  if b.size == 1 then
    return a % b(1);
  else
    return help(1);
}

pragma "inline" def %(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) % b(i), a(a.size) % b(b.size));
    } else {
      var rest = help(i+1);
      return (a(i) % b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  if a.size == 1 then
    return a(1) % b(1);
  else
    return help(1);
}

pragma "inline" def **(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) ** b, a(a.size) ** b);
    } else {
      var rest = help(i+1);
      return (a(i) ** b, (...rest));
    }
  }
  if a.size == 1 then
    return a(1) ** b;
  else
    return help(1);
}

pragma "inline" def **(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size-1 {
      return (a ** b(i), a ** b(b.size));
    } else {
      var rest = help(i+1);
      return (a ** b(i), (...rest));
    }
  }
  if b.size == 1 then
    return a ** b(1);
  else
    return help(1);
}

pragma "inline" def **(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) ** b(i), a(a.size) ** b(b.size));
    } else {
      var rest = help(i+1);
      return (a(i) ** b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  if a.size == 1 then
    return a(1) ** b(1);
  else
    return help(1);
}

pragma "inline" def ~(a: _tuple) {
  for param i in 1..a.size do
    a(i) = ~a(i);
  return a;
}

pragma "inline" def &(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) & b, a(a.size) & b);
    } else {
      var rest = help(i+1);
      return (a(i) & b, (...rest));
    }
  }
  if a.size == 1 then
    return a(1) & b;
  else
    return help(1);
}

pragma "inline" def &(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size-1 {
      return (a & b(i), a & b(b.size));
    } else {
      var rest = help(i+1);
      return (a & b(i), (...rest));
    }
  }
  if b.size == 1 then
    return a & b(1);
  else
    return help(1);
}

pragma "inline" def &(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) & b(i), a(a.size) & b(b.size));
    } else {
      var rest = help(i+1);
      return (a(i) & b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  if a.size == 1 then
    return a(1) & b(1);
  else
    return help(1);
}

pragma "inline" def |(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) | b, a(a.size) | b);
    } else {
      var rest = help(i+1);
      return (a(i) | b, (...rest));
    }
  }
  if a.size == 1 then
    return a(1) | b;
  else
    return help(1);
}

pragma "inline" def |(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size-1 {
      return (a | b(i), a | b(b.size));
    } else {
      var rest = help(i+1);
      return (a | b(i), (...rest));
    }
  }
  if b.size == 1 then
    return a | b(1);
  else
    return help(1);
}

pragma "inline" def |(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) | b(i), a(a.size) | b(b.size));
    } else {
      var rest = help(i+1);
      return (a(i) | b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  if a.size == 1 then
    return a(1) | b(1);
  else
    return help(1);
}

pragma "inline" def ^(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) ^ b, a(a.size) ^ b);
    } else {
      var rest = help(i+1);
      return (a(i) ^ b, (...rest));
    }
  }
  if a.size == 1 then
    return a(1) ^ b;
  else
    return help(1);
}

pragma "inline" def ^(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size-1 {
      return (a ^ b(i), a ^ b(b.size));
    } else {
      var rest = help(i+1);
      return (a ^ b(i), (...rest));
    }
  }
  if b.size == 1 then
    return a ^ b(1);
  else
    return help(1);
}

pragma "inline" def ^(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) ^ b(i), a(a.size) ^ b(b.size));
    } else {
      var rest = help(i+1);
      return (a(i) ^ b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  if a.size == 1 then
    return a(1) ^ b(1);
  else
    return help(1);
}

pragma "inline" def <<(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) << b, a(a.size) << b);
    } else {
      var rest = help(i+1);
      return (a(i) << b, (...rest));
    }
  }
  if a.size == 1 then
    return a(1) << b;
  else
    return help(1);
}

pragma "inline" def <<(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size-1 {
      return (a << b(i), a << b(b.size));
    } else {
      var rest = help(i+1);
      return (a << b(i), (...rest));
    }
  }
  if b.size == 1 then
    return a << b(1);
  else
    return help(1);
}

pragma "inline" def <<(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) << b(i), a(a.size) << b(b.size));
    } else {
      var rest = help(i+1);
      return (a(i) << b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  if a.size == 1 then
    return a(1) << b(1);
  else
    return help(1);
}

pragma "inline" def >>(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) >> b, a(a.size) >> b);
    } else {
      var rest = help(i+1);
      return (a(i) >> b, (...rest));
    }
  }
  if a.size == 1 then
    return a(1) >> b;
  else
    return help(1);
}

pragma "inline" def >>(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size-1 {
      return (a >> b(i), a >> b(b.size));
    } else {
      var rest = help(i+1);
      return (a >> b(i), (...rest));
    }
  }
  if b.size == 1 then
    return a >> b(1);
  else
    return help(1);
}

pragma "inline" def >>(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size-1 {
      return (a(i) >> b(i), a(a.size) >> b(b.size));
    } else {
      var rest = help(i+1);
      return (a(i) >> b(i), (...rest));
    }
  }
  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  if a.size == 1 then
    return a(1) >> b(1);
  else
    return help(1);
}

pragma "inline" def !(a: _tuple) {
  var b: a.size * bool;
  for param i in 1..a.size do
    b(i) = !a(i);
  return b;
}

pragma "inline" def >(a: _tuple, b) {
  for param i in 1..a.size do
    if a(i) > b then
      return true;
    else if a(i) < b then
      return false;
  return false;
}

pragma "inline" def >(a, b: _tuple) {
  for param i in 1..b.size do
    if a > b(i) then
      return true;
    else if a < b(i) then
      return false;
  return false;
}

pragma "inline" def >(a: _tuple, b: _tuple) {
  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  for param i in 1..a.size do
    if a(i) > b(i) then
      return true;
    else if a(i) < b(i) then
      return false;
  return false;
}

pragma "inline" def >=(a: _tuple, b) {
  for param i in 1..a.size do
    if a(i) > b then
      return true;
    else if a(i) < b then
      return false;
  return true;
}

pragma "inline" def >=(a, b: _tuple) {
  for param i in 1..b.size do
    if a > b(i) then
      return true;
    else if a < b(i) then
      return false;
  return true;
}

pragma "inline" def >=(a: _tuple, b: _tuple) {
  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  for param i in 1..a.size do
    if a(i) > b(i) then
      return true;
    else if a(i) < b(i) then
      return false;
  return true;
}

pragma "inline" def <(a: _tuple, b) {
  for param i in 1..a.size do
    if a(i) < b then
      return true;
    else if a(i) > b then
      return false;
  return false;
}

pragma "inline" def <(a, b: _tuple) {
  for param i in 1..b.size do
    if a < b(i) then
      return true;
    else if a > b(i) then
      return false;
  return false;
}

pragma "inline" def <(a: _tuple, b: _tuple) {
  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  for param i in 1..a.size do
    if a(i) < b(i) then
      return true;
    else if a(i) > b(i) then
      return false;
  return false;
}

pragma "inline" def <=(a: _tuple, b) {
  for param i in 1..a.size do
    if a(i) < b(i) then
      return true;
    else if a(i) > b(i) then
      return false;
  return true;
}

pragma "inline" def <=(a, b: _tuple) {
  for param i in 1..b.size do
    if a < b(i) then
      return true;
    else if a > b(i) then
      return false;
  return true;
}

pragma "inline" def <=(a: _tuple, b: _tuple) {
  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  for param i in 1..a.size do
    if a(i) < b(i) then
      return true;
    else if a(i) > b(i) then
      return false;
  return true;
}

pragma "scalar replace tuples"
record _square_tuple {
  param size: int;
  var tuple;

  iterator ault_help(param dim: int) {
    if dim == size - 1 {
      for i in tuple(dim) do
        for j in tuple(size) do
          yield (i, j);
    } else {
      for i in tuple(dim) do
        for j in ault_help(dim+1) do
          yield (i, (...j));
    }
  }

  iterator ault() {
    if size == 1 {
      for i in tuple(1) do
        yield i;
    } else {
      for i in ault_help(1) do
        yield i;
    }
  }
}

def _build_domain(x ...?size) where size > 1
  return _square_tuple(size, x);

def _build_tuple(x ...?size) {
  if size == 1 then
    return x(1);
  else
    return x;
}

pragma "inline" def _getIteratorTupleHelp(t: _tuple, param i: int) {
  if i == t.size-1 {
    return (_getIterator(t(i)), _getIterator(t(t.size)));
  } else {
    return (_getIterator(t(i)), (..._getIteratorTupleHelp(t, i+1)));
  }
}

pragma "inline" def _getIterator(t: _tuple)
  return _getIteratorTupleHelp(t, 1);
