def *(param p: int, type t) type {
  var oneTuple: _build_tuple(t);
  def _fill(param p: int, x: _tuple) {
    if x.size == p then
      return x;
    else if 2*x.size <= p then
      return _fill(p, ((...x), (...x)));
    else
      return _fill(p, ((...x), (..._fill(p-x.size, oneTuple))));
  }
  return _fill(p, oneTuple);
}


def *(p: int, type t) type {
  compilerError("tuple size must be static");
}

pragma "inline" def _copy(x: _tuple) return _build_tuple_always((...x));

pragma "tuple" record _tuple {
  param size : int;

  def this(i : int) var {  // for homogeneous tuples
    for param j in 1..size do
      if i == j then
        return this(j);
    halt("tuple indexing out-of-bounds error");
    return this(1);
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
  else {
    for param i in 1..a.size do
      if (a(i) != b(i)) then
        return false;
    return true;
  }
}

def !=( a: _tuple, b: _tuple): bool {
  if (a.size != b.size) then
    return true;
  else {
    for param i in 1..a.size do
      if (a(i) != b(i)) then
        return true;
    return false;
  }
}

def _tuple.writeThis(f: Writer) {
  f.write("(", this(1));
  for param i in 2..size do
    f.write(", ", this(i));
  f.write(")");
}

pragma "inline" def _cast(type t, x: _tuple) where (t == complex(64)) && (x.size == 2) {
  var c: complex(64);
  c.re = x(1):real(32);
  c.im = x(2):real(32);
  return c;
}

pragma "inline" def _cast(type t, x: _tuple) where (t == complex(128)) && (x.size == 2) {
  var c: complex(128);
  c.re = x(1):real(64);
  c.im = x(2):real(64);
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

// def _isHomogeneous(t: _tuple) param {
//   def _isHomogeneousHelp(param i: int, t: _tuple) param {
//     if t(i).type != t(1).type then
//       return false;
//     else if i < t.size then
//       return _isHomogeneousHelp(i+1, t);
//     else
//       return true;
//   }
//   if t.size == 1 then
//     return true;
//   else
//     return _isHomogeneousHelp(2, t);
// }

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

    /*
pragma "inline" def +(a: _tuple, b) where _isHomogeneous(a) && b:a(1).type {
  var first = a(1) + b;
  var result: a.size*first.type;
  result(1) = first;
  for param i in 2..a.size do
    result(i) = a(i) + b;
  return result;
}

pragma "inline" def +(b, a: _tuple) where _isHomogeneous(a) && b:a(1).type {
  var first = b + a(1);
  var result: a.size*first.type;
  result(1) = first;
  for param i in 2..a.size do
    result(i) = b + a(i);
  return result;
}

pragma "inline" def +(a: _tuple, b: _tuple) where _isHomogeneous(a) && _isHomogeneous(b) && a.size == b.size {
  var first = a(1) + b(1);
  var result: a.size*first.type;
  result(1) = first;
  for param i in 2..a.size do
    result(i) = a(i) + b(i);
  return result;
}
    */

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

record _square_tuple {
  param size: int;
  var tuple;

  pragma "expand tuples with values"
  def these_help(param dim: int) {
    if dim == size - 1 {
      for i in tuple(dim) do
        for j in tuple(size) do
          yield _build_tuple_always_allow_ref(i, j);
    } else {
      for i in tuple(dim) do
        for j in these_help(dim+1) do
          yield _build_tuple_always_allow_ref(i, (...j));
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

def chpl__buildDomainExpr(x ...?size) where size > 1
  return new _square_tuple(size, x);

pragma "inline" def _build_tuple(x ...?size) {
  if size == 1 then
    return x(1);
  else
    return x;
}

pragma "inline" pragma "allow ref" def _build_tuple_always_allow_ref(x ...?size)
  return x;

pragma "inline" def _build_tuple(type t ...?size) type
  return t;

pragma "inline" def _build_tuple_always(x ...?size)
  return x;
