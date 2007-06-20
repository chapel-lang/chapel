def _iteratorUnbounded(x) param
  return false;

def _iteratorUnbounded(r: range) param where r.boundedType != bounded
  return true;

pragma "inline" def _getIteratorTupleHelp(t: _tuple, param i: int) {
  if i == t.size {
    return (_getIterator(t(i)), 1); // ignore last == 1
  } else {
    return (_getIterator(t(i)), (..._getIteratorTupleHelp(t, i+1)));
  }
}

pragma "inline" def _getZipCursor1TupleHelp(t: _tuple, param i: int) {
  if i == t.size-1 {
    return (t(i).getZipCursor1(), 1); // ignore last == 1
  } else {
    return (t(i).getZipCursor1(), (..._getZipCursor1TupleHelp(t, i+1)));
  }
}

pragma "inline" def _getValueTupleHelp(ic: _tuple, c: _tuple, param i: int) {
  if i == ic.size-2 {
    return (ic(1).getValue(c(1)), ic(2).getValue(c(2)));
  } else {
    return (ic(1).getValue(c(1)), (..._getValueTupleHelp(ic, c, i+1)));
  }
}

pragma "scalar replace tuples" pragma "tuple" record _tuple {
  param size : int;

  pragma "inline" pragma "tuple get" def this(param i : int) var
    return 0;

  def this(i : int) var {
    for param j in 1..size do
      if i == j then
        return this(j);
    halt("tuple indexing out-of-bounds error");
  }

  def these() {
    if size == 1 {
      for i in this(1) {
        var t: 1*i.type;
        t(1) = i;
        yield t;
      }
    } else {
      var ic = _getIteratorTupleHelp(this, 2);
      var c = _getZipCursor1TupleHelp(ic, 1);
      if c.size != ic.size then
        compilerError("internal size check failure; zipper failed");
      for i in this(1) {
        for param i in 1..ic.size-1 do
          c(i) = ic(i).getZipCursor2(c(i));
        // _iteratorUnbounded makes it so that we don't insert these
        // breaks if the first iterator can fully control the loop;
        // this is important so that the single loop iterator
        // optimization can fire; the break in the loop disables it
        if _iteratorUnbounded(this(1)) {
          for param i in 1..ic.size-1 do
            if !ic(i).isValidCursor(c(i)) then
              break;
        } else if boundsChecking {
          for param i in 1..ic.size-1 do
            if !_iteratorUnbounded(this(i+1)) then
              if !ic(i).isValidCursor(c(i)) then
                halt("zippered iterations have non-equal lengths");
        }
        if ic.size == 2 then
          yield (i, ic(1).getValue(c(1)));
        else
          yield (i, (..._getValueTupleHelp(ic, c, 1)));
        for param i in 1..ic.size-1 do
          c(i) = ic(i).getZipCursor3(c(i));
      }
      for param i in 1..ic.size-1 do
        c(i) = ic(i).getZipCursor4(c(i));
      if boundsChecking then
        for param i in 1..ic.size-1 do
          if !_iteratorUnbounded(this(i+1)) then
            if ic(i).isValidCursor(c(i)) then
              halt("zippered iterations have non-equal lengths");
    }
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

pragma "inline" def _cast(type t, x: _tuple) where (t == complex(64)) & (x.size == 2) {
  var c: complex(64);
  c.re = x(1):real(32);
  c.im = x(2):real(32);
  return c;
}

pragma "inline" def _cast(type t, x: _tuple) where (t == complex(128)) & (x.size == 2) {
  var c: complex(128);
  c.re = x(1):real(64);
  c.im = x(2):real(64);
  return c;
}

pragma "inline" def _cast(type t, x: _tuple) where (t == complex(256)) & (x.size == 2) {
  var c: complex(256);
  c.re = x(1):real(128);
  c.im = x(2):real(128);
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
    if i == a.size {
      var result = a(i) + b;
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return (onetuple);
    } else {
      var rest = help(i+1);
      return (a(i) + b, (...rest));
    }
  }
  return help(1);
}

pragma "inline" def +(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size {
      var result = a + b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a + b(i), (...rest));
    }
  }
  return help(1);
}

pragma "inline" def +(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) + b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) + b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  return help(1);
}

pragma "inline" def -(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) - b;
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) - b, (...rest));
    }
  }
  return help(1);
}

pragma "inline" def -(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size {
      var result = a - b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a - b(i), (...rest));
    }
  }
  return help(1);
}

pragma "inline" def -(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) - b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) - b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  return help(1);
}

pragma "inline" def *(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) * b;
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) * b, (...rest));
    }
  }
  return help(1);
}

pragma "inline" def *(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size {
      var result = a * b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a * b(i), (...rest));
    }
  }
  return help(1);
}

pragma "inline" def *(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) * b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) * b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  return help(1);
}

pragma "inline" def /(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) / b;
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) / b, (...rest));
    }
  }
  return help(1);
}

pragma "inline" def /(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size {
      var result = a / b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a / b(i), (...rest));
    }
  }
  return help(1);
}

pragma "inline" def /(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) / b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) / b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  return help(1);
}

pragma "inline" def %(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) % b;
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) % b, (...rest));
    }
  }
  return help(1);
}

pragma "inline" def %(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size {
      var result = a % b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a % b(i), (...rest));
    }
  }
  return help(1);
}

pragma "inline" def %(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) % b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) % b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  return help(1);
}

pragma "inline" def **(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) ** b;
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) ** b, (...rest));
    }
  }
  return help(1);
}

pragma "inline" def **(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size {
      var result = a ** b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a ** b(i), (...rest));
    }
  }
  return help(1);
}

pragma "inline" def **(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) ** b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) ** b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  return help(1);
}

pragma "inline" def ~(a: _tuple) {
  for param i in 1..a.size do
    a(i) = ~a(i);
  return a;
}

pragma "inline" def &(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) & b;
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) & b, (...rest));
    }
  }
  return help(1);
}

pragma "inline" def &(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size {
      var result = a & b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a & b(i), (...rest));
    }
  }
  return help(1);
}

pragma "inline" def &(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) & b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) & b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  return help(1);
}

pragma "inline" def |(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) | b;
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) | b, (...rest));
    }
  }
  return help(1);
}

pragma "inline" def |(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size {
      var result = a | b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a | b(i), (...rest));
    }
  }
  return help(1);
}

pragma "inline" def |(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) | b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) | b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  return help(1);
}

pragma "inline" def ^(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) ^ b;
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) ^ b, (...rest));
    }
  }
  return help(1);
}

pragma "inline" def ^(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size {
      var result = a ^ b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a ^ b(i), (...rest));
    }
  }
  return help(1);
}

pragma "inline" def ^(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) ^ b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) ^ b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  return help(1);
}

pragma "inline" def <<(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) << b;
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) << b, (...rest));
    }
  }
  return help(1);
}

pragma "inline" def <<(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size {
      var result = a << b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a << b(i), (...rest));
    }
  }
  return help(1);
}

pragma "inline" def <<(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) << b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) << b(i), (...rest));
    }
  }

  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

  return help(1);
}

pragma "inline" def >>(a: _tuple, b) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) >> b;
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) >> b, (...rest));
    }
  }
  return help(1);
}

pragma "inline" def >>(a, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == b.size {
      var result = a >> b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a >> b(i), (...rest));
    }
  }
  return help(1);
}

pragma "inline" def >>(a: _tuple, b: _tuple) {
  pragma "inline" def help(param i: int) {
    if i == a.size {
      var result = a(i) >> b(i);
      var onetuple: 1 * result.type;
      onetuple(1) = result;
      return onetuple;
    } else {
      var rest = help(i+1);
      return (a(i) >> b(i), (...rest));
    }
  }
  if (a.size != b.size) then
    compilerError("Tuples must have the same size");

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

  def these_help(param dim: int) {
    if dim == size - 1 {
      for i in tuple(dim) do
        for j in tuple(size) do
          yield (i, j);
    } else {
      for i in tuple(dim) do
        for j in these_help(dim+1) do
          yield (i, (...j));
    }
  }

  def these() {
    if size == 1 {
      for i in tuple(1) do
        yield i;
    } else {
      for i in these_help(1) do
        yield i;
    }
  }
}

def _build_domain(x ...?size) where size > 1
  return _square_tuple(size, x);

pragma "inline" def _build_tuple(x ...?size) {
  if size == 1 then
    return x(1);
  else
    return x;
}
