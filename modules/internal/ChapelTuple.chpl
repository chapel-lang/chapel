// ChapelTuple.chpl
//
// tuple data implementation as a record
//
pragma "no use ChapelStandard"
module ChapelTuple {
  
  pragma "tuple" record _tuple {
    param size : int;
  }
  
  //
  // syntactic support for tuples
  //
  pragma "build tuple"
  inline proc _build_tuple(x...) {
      return x;
  }
  
  pragma "allow ref" 
  pragma "build tuple"
  inline proc _build_tuple_always_allow_ref(x ...?size)
    return x;
  
  pragma "build tuple"
  inline proc _build_tuple(type t ...?size) type
    return t;
  
  pragma "build tuple"
  inline proc _build_tuple_always(x ...?size)
    return x;
  
  //
  // homogeneous tuple type syntax
  //
  proc *(param p: int, type t) type {
    var oneTuple: _build_tuple(t);
    proc _fill(param p: int, x: _tuple) {
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
  
  proc *(p: int, type t) type {
    compilerError("tuple size must be static");
  }
  
  // make it a tuple if it is not already
  inline proc chpl__tuplify(x) {
    if isTuple(x) then return x; else return (x,);
  }
  
  //
  // isTuple, isTupleType and isHomogeneousTuple param functions
  //
  proc isTuple(x: _tuple) param
    return true;
  
  proc isTuple(x) param
    return false;
  
  proc isHomogeneousTuple(x: _tuple) param
    return __primitive("is star tuple type", x);
  
  proc isTupleType(type t) param {
    const a: t;
    return isTuple(a);
  }
  
  //
  // tuple assignment
  //
  inline proc =(x: _tuple, y: _tuple) where x.size == y.size {
    for param i in 1..x.size do
      x(i) = y(i);
    return x;
  }
  
  //
  // homogeneous tuple accessor
  //
  proc _tuple.this(i : integral) var {
    if !isHomogeneousTuple(this) then
      compilerError("invalid access of non-homogeneous tuple by runtime value");
    if boundsChecking then
      if i < 1 || i > size then
        halt("tuple access out of bounds: ", i);
    return __primitive("get svec member", this, i);
  }

  config param CHPL_WARN_TUPLE_ITERATION = "unset";
  //
  // iterator support for tuples
  //
  iter _tuple.these() {

    if !isHomogeneousTuple(this) then
      compilerError("Cannot iterate over non-homogeneous tuples. If you intended to use zippered iteration, add the new keyword 'zip' before the tuple of iteratable expressions.");

    if CHPL_WARN_TUPLE_ITERATION == "true" then
      compilerWarning("Iterating over tuples. If you intended to use zippered iteration, add the new keyword 'zip' before the tuple of iteratable expressions.");

    for i in 1..this.size {
      yield(this(i));
    }
  }
  
  iter _tuple.these(param tag:iterKind) 
      where tag == iterKind.leader 
  {

    const numTasks = if dataParTasksPerLocale==0 then here.numCores
                     else dataParTasksPerLocale;
    const ignoreRunning = dataParIgnoreRunningTasks;
    const minIndicesPerTask = dataParMinGranularity;
    const length = this.size;
    const myRange = 1..#length;
    var (numChunks, _) = _computeChunkStuff(numTasks, ignoreRunning,
                                            minIndicesPerTask,
                                            (myRange,));

    if numChunks == 1 {
      yield myRange;
    } else {
  
      coforall chunk in 0..#numChunks {
        // _computeBlock assumes 0-based ranges
        const (lo,hi) = _computeBlock(length, numChunks, chunk, length-1);
        // adjust for 1-based indexing
        yield lo+1..hi+1;
      }
    }
  }
  
  iter _tuple.these(param tag:iterKind, followThis)
      where tag == iterKind.follower
  {
    for i in followThis do yield this(i);
  }
  
  //
  // tuple methods
  //
  proc _tuple.readWriteThis(f) {
    var st = f.styleElement(QIO_STYLE_ELEMENT_TUPLE);
    var start:ioLiteral;
    var comma:ioLiteral;
    var end:ioLiteral;
    var binary = f.binary();

    if st == QIO_TUPLE_FORMAT_SPACE {
      start = new ioLiteral("");
      comma = new ioLiteral(" ");
      end = new ioLiteral("");
    } else if st == QIO_TUPLE_FORMAT_JSON {
      start = new ioLiteral("[");
      comma = new ioLiteral(", ");
      end = new ioLiteral("]");
    } else {
      start = new ioLiteral("(");
      comma = new ioLiteral(", ");
      end = new ioLiteral(")");
    }

    if !binary {
      f <~> start;
    }
    if size != 0 {
      f <~> this(1);
      for param i in 2..size {
        if !binary {
          f <~> comma;
        }
        f <~> this(i);
      }
    }
    if !binary {
      f <~> end;
    }
  }
  
  //
  // tuple casts to complex(64) and complex(128)
  //
  inline proc _cast(type t, x: (?,?)) where t == complex(64) {
    var c: complex(64);
    c.re = x(1):real(32);
    c.im = x(2):real(32);
    return c;
  }
  
  inline proc _cast(type t, x: (?,?)) where t == complex(128) {
    var c: complex(128);
    c.re = x(1):real(64);
    c.im = x(2):real(64);
    return c;
  }
  
  //
  // helper function to return a tuple of all of the components in a
  // tuple except the first
  //
  inline proc chpl__tupleRest(t: _tuple) {
    proc chpl__tupleRestHelper(first, rest...)
      return rest;
    return chpl__tupleRestHelper((...t));
  }
  
  //
  // standard overloaded unary operators on tuples
  //
  inline proc +(a: _tuple) {
    if a.size == 1 then
      return (+a(1), );
    else
      return (+a(1), (...+chpl__tupleRest(a)));
  }
  
  inline proc -(a: _tuple) {
    if a.size == 1 then
      return (-a(1),);
    else
      return (-a(1), (...-chpl__tupleRest(a)));
  }
  
  inline proc ~(a: _tuple) {
    if a.size == 1 then
      return (~a(1),);
    else
      return (~a(1), (...~chpl__tupleRest(a)));
  }
  
  inline proc !(a: _tuple) {
    if a.size == 1 then
      return (!a(1),);
    else
      return (!a(1), (...!chpl__tupleRest(a)));
  }
  
  //
  // Return a tuple of type t with all values set to the max/min for that type
  //
  proc max(type t): t where isTupleType(t) {
    var result: t;
    for param i in 1..result.size {
      result(i) = max(t(i));
    }
    return result;
  }
  
  proc min(type t): t where isTupleType(t) {
    var result: t;
    for param i in 1..result.size {
      result(i) = min(t(i));
    }
    return result;
  }
  
  //
  // standard overloaded binary operators on tuples
  //
  inline proc +(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to + have different sizes");
    if a.size == 1 then
      return (a(1)+b(1),);
    else
      return (a(1)+b(1), (...chpl__tupleRest(a)+chpl__tupleRest(b)));
  }
  
  inline proc -(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to - have different sizes");
    if a.size == 1 then
      return (a(1)-b(1),);
    else
      return (a(1)-b(1), (...chpl__tupleRest(a)-chpl__tupleRest(b)));
  }
  
  inline proc *(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to * have different sizes");
    if a.size == 1 then
      return (a(1)*b(1),);
    else
      return (a(1)*b(1), (...chpl__tupleRest(a)*chpl__tupleRest(b)));
  }
  
  inline proc /(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to / have different sizes");
    if a.size == 1 then
      return (a(1)/b(1),);
    else
      return (a(1)/b(1), (...chpl__tupleRest(a)/chpl__tupleRest(b)));
  }
  
  inline proc %(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to % have different sizes");
    if a.size == 1 then
      return (a(1)%b(1),);
    else
      return (a(1)%b(1), (...chpl__tupleRest(a)%chpl__tupleRest(b)));
  }
  
  inline proc **(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to ** have different sizes");
    if a.size == 1 then
      return (a(1)**b(1),);
    else
      return (a(1)**b(1), (...chpl__tupleRest(a)**chpl__tupleRest(b)));
  }
  
  inline proc &(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to & have different sizes");
    if a.size == 1 then
      return (a(1)&b(1),);
    else
      return (a(1)&b(1), (...chpl__tupleRest(a)&chpl__tupleRest(b)));
  }
  
  inline proc |(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to | have different sizes");
    if a.size == 1 then
      return (a(1)|b(1),);
    else
      return (a(1)|b(1), (...chpl__tupleRest(a)|chpl__tupleRest(b)));
  }
  
  inline proc ^(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to ^ have different sizes");
    if a.size == 1 then
      return (a(1)^b(1),);
    else
      return (a(1)^b(1), (...chpl__tupleRest(a)^chpl__tupleRest(b)));
  }
  
  inline proc <<(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to << have different sizes");
    if a.size == 1 then
      return (a(1)<<b(1),);
    else
      return (a(1)<<b(1), (...chpl__tupleRest(a)<<chpl__tupleRest(b)));
  }
  
  inline proc >>(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to >> have different sizes");
    if a.size == 1 then
      return (a(1)>>b(1),);
    else
      return (a(1)>>b(1), (...chpl__tupleRest(a)>>chpl__tupleRest(b)));
  }
  
  //
  // standard overloaded relational operators on tuples
  //
  inline proc >(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to > have different sizes");
    for param i in 1..a.size do
      if a(i) > b(i) then
        return true;
      else if a(i) < b(i) then
        return false;
    return false;
  }
  
  inline proc >=(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to >= have different sizes");
    for param i in 1..a.size do
      if a(i) > b(i) then
        return true;
      else if a(i) < b(i) then
        return false;
    return true;
  }
  
  inline proc <(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to < have different sizes");
    for param i in 1..a.size do
      if a(i) < b(i) then
        return true;
      else if a(i) > b(i) then
        return false;
    return false;
  }
  
  inline proc <=(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to <= have different sizes");
    for param i in 1..a.size do
      if a(i) < b(i) then
        return true;
      else if a(i) > b(i) then
        return false;
    return true;
  }
  
  inline proc ==(a: _tuple, b: _tuple) {
    if a.size != b.size then
      return false;
    for param i in 1..a.size do
      if a(i) != b(i) then
        return false;
    return true;
  }
  
  inline proc !=(a: _tuple, b: _tuple) {
    if a.size != b.size then
      return true;
    for param i in 1..a.size do
      if a(i) != b(i) then
        return true;
    return false;
  }


  // standard overloaded binary operators on homog tuple / scalar pairs
  
  inline proc +(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    if x.size == 1 then
      return (x(1)+y,);
    else
      return (x(1)+y, (...chpl__tupleRest(x)+y));
  }

  inline proc +(x: ?t, y: _tuple) where isHomogeneousTuple(y) && 
                                       t: (y(1).type)  {
    if y.size == 1 then
      return (x+y(1),);
    else
      return (x+y(1), (...x+chpl__tupleRest(y)));
  }

  inline proc -(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    if x.size == 1 then
      return (x(1)-y,);
    else
      return (x(1)-y, (...chpl__tupleRest(x)-y));
  }

  inline proc -(x: ?t, y: _tuple) where isHomogeneousTuple(y) && 
                                       t: (y(1).type)  {
    if y.size == 1 then
      return (x-y(1),);
    else
      return (x-y(1), (...x-chpl__tupleRest(y)));
  }

  inline proc *(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    if x.size == 1 then
      return (x(1)*y,);
    else
      return (x(1)*y, (...chpl__tupleRest(x)*y));
  }

  inline proc *(x: ?t, y: _tuple) where isHomogeneousTuple(y) && 
                                       x: (y(1).type)  {
    if y.size == 1 then
      return (x*y(1),);
    else
      return (x*y(1), (...x*chpl__tupleRest(y)));
  }

  inline proc /(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    if x.size == 1 then
      return (x(1)/y,);
    else
      return (x(1)/y, (...chpl__tupleRest(x)/y));
  }

  inline proc /(x: ?t, y: _tuple) where isHomogeneousTuple(y) && 
                                       t: (y(1).type)  {
    if y.size == 1 then
      return (x/y(1),);
    else
      return (x/y(1), (...x/chpl__tupleRest(y)));
  }

  inline proc %(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    if x.size == 1 then
      return (x(1)%y,);
    else
      return (x(1)%y, (...chpl__tupleRest(x)%y));
  }

  inline proc %(x: ?t, y: _tuple) where isHomogeneousTuple(y) && 
                                       t: (y(1).type)  {
    if y.size == 1 then
      return (x%y(1),);
    else
      return (x%y(1), (...x%chpl__tupleRest(y)));
  }

  inline proc **(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    if x.size == 1 then
      return (x(1)**y,);
    else
      return (x(1)**y, (...chpl__tupleRest(x)**y));
  }

  inline proc **(x: ?t, y: _tuple) where isHomogeneousTuple(y) && 
                                       t: (y(1).type)  {
    if y.size == 1 then
      return (x**y(1),);
    else
      return (x**y(1), (...x**chpl__tupleRest(y)));
  }

  inline proc &(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    if x.size == 1 then
      return (x(1)&y,);
    else
      return (x(1)&y, (...chpl__tupleRest(x)&y));
  }

  inline proc &(x: ?t, y: _tuple) where isHomogeneousTuple(y) && 
                                       t: (y(1).type)  {
    if y.size == 1 then
      return (x&y(1),);
    else
      return (x&y(1), (...x&chpl__tupleRest(y)));
  }

  inline proc |(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    if x.size == 1 then
      return (x(1)|y,);
    else
      return (x(1)|y, (...chpl__tupleRest(x)|y));
  }

  inline proc |(x: ?t, y: _tuple) where isHomogeneousTuple(y) && 
                                       t: (y(1).type)  {
    if y.size == 1 then
      return (x|y(1),);
    else
      return (x|y(1), (...x|chpl__tupleRest(y)));
  }

  inline proc ^(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    if x.size == 1 then
      return (x(1)^y,);
    else
      return (x(1)^y, (...chpl__tupleRest(x)^y));
  }

  inline proc ^(x: ?t, y: _tuple) where isHomogeneousTuple(y) && 
                                       t: (y(1).type)  {
    if y.size == 1 then
      return (x^y(1),);
    else
      return (x^y(1), (...x^chpl__tupleRest(y)));
  }

  inline proc <<(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    if x.size == 1 then
      return (x(1)<<y,);
    else
      return (x(1)<<y, (...chpl__tupleRest(x)<<y));
  }

  inline proc <<(x: ?t, y: _tuple) where isHomogeneousTuple(y) && 
                                       t: (y(1).type)  {
    if y.size == 1 then
      return (x<<y(1),);
    else
      return (x<<y(1), (...x<<chpl__tupleRest(y)));
  }

  inline proc >>(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    if x.size == 1 then
      return (x(1)>>y,);
    else
      return (x(1)>>y, (...chpl__tupleRest(x)>>y));
  }

  inline proc >>(x: ?t, y: _tuple) where isHomogeneousTuple(y) && 
                                       t: (y(1).type)  {
    if y.size == 1 then
      return (x>>y(1),);
    else
      return (x>>y(1), (...x>>chpl__tupleRest(y)));
  }



}
