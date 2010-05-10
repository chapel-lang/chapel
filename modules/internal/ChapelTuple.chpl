//
// tuple data implementation as a record
//
pragma "tuple" record _tuple {
  param size : int;
}

//
// syntactic support for tuples
//
pragma "inline" def _build_tuple(x...) {
  if x.size == 1 then
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

//
// homogeneous tuple type syntax
//
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
  if p <= 0 then
    compilerError("tuple must have positive size");
  return _fill(p, oneTuple).type;
}

def *(p: int, type t) type {
  compilerError("tuple size must be static");
}

//
// standard tuple creation function
//
pragma "inline" def tuple(x ...?size)
  return x;

//
// isTuple, isTupleType and isHomogeneousTuple param functions
//
def isTuple(x: _tuple) param
  return true;

def isTuple(x) param
  return false;

def isHomogeneousTuple(x: _tuple) param
  return __primitive("is star tuple type", x);

def isTupleType(type t) param {
  const a: t;
  return isTuple(a);
}

//
// tuple assignment
//
pragma "inline" def =(x: _tuple, y: _tuple) where x.size == y.size {
  for param i in 1..x.size do
    x(i) = y(i);
  return x;
}

//
// homogeneous tuple accessor
//
def _tuple.this(i : integral) var {
  if !isHomogeneousTuple(this) then
    compilerError("invalid access of non-homogeneous tuple by runtime value");
  if boundsChecking then
    if i < 1 || i > size then
      halt("tuple access out of bounds: ", i);
  return __primitive("get svec member", this, i);
}

//
// tuple methods
//
def _tuple.writeThis(f: Writer) {
  f.write("(", this(1));
  for param i in 2..size do
    f.write(", ", this(i));
  f.write(")");
}

//
// tuple casts to complex(64) and complex(128)
//
pragma "inline" def _cast(type t, x: (?,?)) where t == complex(64) {
  var c: complex(64);
  c.re = x(1):real(32);
  c.im = x(2):real(32);
  return c;
}

pragma "inline" def _cast(type t, x: (?,?)) where t == complex(128) {
  var c: complex(128);
  c.re = x(1):real(64);
  c.im = x(2):real(64);
  return c;
}

//
// helper function to return a tuple of all of the components in a
// tuple except the first
//
pragma "inline" def chpl__tupleRest(t: _tuple) {
  def chpl__tupleRestHelper(first, rest...)
    return rest;
  return chpl__tupleRestHelper((...t));
}

//
// standard overloaded unary operators on tuples
//
pragma "inline" def +(a: _tuple) {
  if a.size == 1 then
    return tuple(+a(1));
  else
    return tuple(+a(1), (...+chpl__tupleRest(a)));
}

pragma "inline" def -(a: _tuple) {
  if a.size == 1 then
    return tuple(-a(1));
  else
    return tuple(-a(1), (...-chpl__tupleRest(a)));
}

pragma "inline" def ~(a: _tuple) {
  if a.size == 1 then
    return tuple(~a(1));
  else
    return tuple(~a(1), (...~chpl__tupleRest(a)));
}

pragma "inline" def !(a: _tuple) {
  if a.size == 1 then
    return tuple(!a(1));
  else
    return tuple(!a(1), (...!chpl__tupleRest(a)));
}

//
// Return a tuple of type t with all values set to the max/min for that type
//
def max(type t): t where isTupleType(t) {
  var result: t;
  for param i in 1..result.size {
    result(i) = max(t(i));
  }
  return result;
}

def min(type t): t where isTupleType(t) {
  var result: t;
  for param i in 1..result.size {
    result(i) = min(t(i));
  }
  return result;
}

//
// standard overloaded binary operators on tuples
//
pragma "inline" def +(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to + have different sizes");
  if a.size == 1 then
    return tuple(a(1)+b(1));
  else
    return tuple(a(1)+b(1), (...chpl__tupleRest(a)+chpl__tupleRest(b)));
}

pragma "inline" def -(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to - have different sizes");
  if a.size == 1 then
    return tuple(a(1)-b(1));
  else
    return tuple(a(1)-b(1), (...chpl__tupleRest(a)-chpl__tupleRest(b)));
}

pragma "inline" def *(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to * have different sizes");
  if a.size == 1 then
    return tuple(a(1)*b(1));
  else
    return tuple(a(1)*b(1), (...chpl__tupleRest(a)*chpl__tupleRest(b)));
}

pragma "inline" def /(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to / have different sizes");
  if a.size == 1 then
    return tuple(a(1)/b(1));
  else
    return tuple(a(1)/b(1), (...chpl__tupleRest(a)/chpl__tupleRest(b)));
}

pragma "inline" def %(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to % have different sizes");
  if a.size == 1 then
    return tuple(a(1)%b(1));
  else
    return tuple(a(1)%b(1), (...chpl__tupleRest(a)%chpl__tupleRest(b)));
}

pragma "inline" def **(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to ** have different sizes");
  if a.size == 1 then
    return tuple(a(1)**b(1));
  else
    return tuple(a(1)**b(1), (...chpl__tupleRest(a)**chpl__tupleRest(b)));
}

pragma "inline" def &(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to & have different sizes");
  if a.size == 1 then
    return tuple(a(1)&b(1));
  else
    return tuple(a(1)&b(1), (...chpl__tupleRest(a)&chpl__tupleRest(b)));
}

pragma "inline" def |(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to | have different sizes");
  if a.size == 1 then
    return tuple(a(1)|b(1));
  else
    return tuple(a(1)|b(1), (...chpl__tupleRest(a)|chpl__tupleRest(b)));
}

pragma "inline" def ^(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to ^ have different sizes");
  if a.size == 1 then
    return tuple(a(1)^b(1));
  else
    return tuple(a(1)^b(1), (...chpl__tupleRest(a)^chpl__tupleRest(b)));
}

pragma "inline" def <<(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to << have different sizes");
  if a.size == 1 then
    return tuple(a(1)<<b(1));
  else
    return tuple(a(1)<<b(1), (...chpl__tupleRest(a)<<chpl__tupleRest(b)));
}

pragma "inline" def >>(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to >> have different sizes");
  if a.size == 1 then
    return tuple(a(1)>>b(1));
  else
    return tuple(a(1)>>b(1), (...chpl__tupleRest(a)>>chpl__tupleRest(b)));
}

//
// standard overloaded relational operators on tuples
//
pragma "inline" def >(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to > have different sizes");
  for param i in 1..a.size do
    if a(i) > b(i) then
      return true;
    else if a(i) < b(i) then
      return false;
  return false;
}

pragma "inline" def >=(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to >= have different sizes");
  for param i in 1..a.size do
    if a(i) > b(i) then
      return true;
    else if a(i) < b(i) then
      return false;
  return true;
}

pragma "inline" def <(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to < have different sizes");
  for param i in 1..a.size do
    if a(i) < b(i) then
      return true;
    else if a(i) > b(i) then
      return false;
  return false;
}

pragma "inline" def <=(a: _tuple, b: _tuple) {
  if a.size != b.size then
    compilerError("tuple operands to <= have different sizes");
  for param i in 1..a.size do
    if a(i) < b(i) then
      return true;
    else if a(i) > b(i) then
      return false;
  return true;
}

pragma "inline" def ==(a: _tuple, b: _tuple) {
  if a.size != b.size then
    return false;
  for param i in 1..a.size do
    if a(i) != b(i) then
      return false;
  return true;
}

pragma "inline" def !=(a: _tuple, b: _tuple) {
  if a.size != b.size then
    return true;
  for param i in 1..a.size do
    if a(i) != b(i) then
      return true;
  return false;
}

//
// square tuples
//
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

  pragma "inline iterator"
  pragma "expand tuples with values"
  def lead_help(param dim: int) {
    if dim == size - 1 {
      for i in _toLeader(tuple(dim)) do
        for j in _toLeader(tuple(size)) do
          yield _build_tuple_always_allow_ref(i, j);
    } else {
      for i in _toLeader(tuple(dim)) do
        for j in lead_help(dim+1) do
          yield _build_tuple_always_allow_ref(i, (...j));
    }
  }

  def these(param tag: iterator) where tag == iterator.leader {
    if size == 1 {
      for i in _toLeader(tuple(1)) do
        yield i;
    } else {
      for i in lead_help(1) do
        yield i;
    }
  }

  pragma "expand tuples with values"
  def follow_help(param dim: int, follower) {
    if dim == size - 1 {
      for i in _toFollower(tuple(dim), follower(dim)) do
        for j in _toFollower(tuple(size), follower(size)) do
          yield _build_tuple_always_allow_ref(i, j);
    } else {
      for i in _toFollower(tuple(dim), follower(dim)) do
        for j in follow_help(dim+1) do
          yield _build_tuple_always_allow_ref(i, (...j));
    }
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    if size == 1 {
      for i in _toFollower(tuple(1), follower) do
        yield i;
    } else {
      for i in follow_help(1, follower) do
        yield i;
    }
  }
}

//
// syntactic support for square tuples
//
def chpl__buildDomainExpr(x ...?size) where size > 1
  return new _square_tuple(size, x);
