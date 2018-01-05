/*
 * Copyright 2004-2018 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
Predefined Functions on Tuples.

Tuples are a predefined structured type in Chapel. They are specified
in the Tuples chapter of the Chapel Language Specification.
This page lists the predefined functions on tuples.
They are always available to all Chapel programs.

Besides the functions defined here, the Chapel Language specification
defines other operations available on tuples: indexing, iteration,
assignment, and unary, binary, and relational operators.

The following method is also available:

  .. code-block:: chapel

    proc tuple.size param

It returns the number of components of the tuple.
*/
module ChapelTuple {
  use ChapelStandard;

  pragma "tuple" record _tuple {
    param size : int;
  }

  //
  // syntactic support for tuples
  //

  // tuple type (refs for types with blank intent=ref intent)
  pragma "build tuple"
  pragma "build tuple type"
  inline proc _build_tuple(type t...) type {
    // body inserted during generic instantiation
  }

  // tuple value (refs)
  pragma "build tuple"
  inline proc _build_tuple(x...) {
      return x;
  }

  // tuple type (no refs)
  pragma "do not allow ref"
  pragma "build tuple"
  pragma "build tuple type"
  inline proc _build_tuple_noref(type t...) type {
    // body inserted during generic instantiation
  }

  // tuple value allowing refs (ref actuals)
  pragma "allow ref"
  pragma "build tuple"
  inline proc _build_tuple_always_allow_ref(x...)
    return x;

  inline proc chpl__unref(type t) type {
    if isTupleType(t) then
      return _build_tuple_noref((...t));
    else
      return t;
  }

   // homogeneous tuple type
  pragma "build tuple"
  pragma "build tuple type"
  pragma "star tuple"
  proc *(param p: int, type t) type {
    // body inserted during generic instantiation
  }

  pragma "do not allow ref"
  pragma "build tuple"
  pragma "build tuple type"
  pragma "star tuple"
  proc _build_star_tuple_noref(param p: int, type t) type {
    // body inserted during generic instantiation
  }

  pragma "last resort"
  proc *(type t, param p: int) {
    compilerError("<type>*<param int> not supported.  If you're trying to specify a homogeneous tuple type, use <param int>*<type>.");
  }

  // last resort since if this resolves some other way, OK
  pragma "last resort"
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
  pragma "no doc" // we are not advertising isXxxValue() functions at present
  proc isTupleValue(x: _tuple) param
    return true;

  pragma "no doc"
  proc isTupleValue(x) param
    return false;

  pragma "no doc"
  proc isHomogeneousTupleValue(x) param
    return __primitive("is star tuple type", x);

  /*
    Returns `true` if its argument is a tuple type.
    The argument must be a type.
  */
  proc isTupleType(type t) param
    return __primitive("is tuple type", t);

  /*
    Returns `true` if its argument is a homogeneous tuple type.
    The argument must be a type.
  */
  proc isHomogeneousTupleType(type t) param
    return __primitive("is star tuple type", t);

  //
  // tuple assignment
  //
  pragma "compiler generated"
  pragma "last resort"
  inline proc =(ref x: _tuple, y: _tuple) where x.size == y.size {
    for param i in 1..x.size do
      x(i) = y(i);
  }

  //
  // homogeneous tuple accessor
  // the result is const when the tuple is
  //
  pragma "no doc"
  pragma "reference to const when const this"
  pragma "star tuple accessor"
  proc _tuple.this(i : integral) ref {
    if !isHomogeneousTuple(this) then
      compilerError("invalid access of non-homogeneous tuple by runtime value");
    if boundsChecking then
      if i < 1 || i > size then
        halt("tuple access out of bounds: ", i);
    return __primitive("get svec member", this, i);
  }

  // This is controlled with --[no-]warn-tuple-iteration
  // so we are not chpldoc-ing it.
  //
  // When this param is set to the string "true",
  // the compiler will issue a warning when iterating over components
  // of a homogeneous tuple, e.g. ``for i in (A,B,C)``.
  //
  // This is useful to expose code where zippered iteration,
  // e.g. ``for abc in zip(A,B,C)``, may have been intended.
  //
  pragma "no doc"
  config param CHPL_WARN_TUPLE_ITERATION = "unset";

  //
  // iterator support for tuples
  //
  pragma "no doc"
  iter _tuple.these() {

    if !isHomogeneousTuple(this) then
      compilerError("Cannot iterate over non-homogeneous tuples. If you intended to use zippered iteration, add the new keyword 'zip' before the tuple of iteratable expressions.");

    if CHPL_WARN_TUPLE_ITERATION == "true" then
      compilerWarning("Iterating over tuples. If you intended to use zippered iteration, add the new keyword 'zip' before the tuple of iteratable expressions.");

    for i in 1..this.size {
      yield(this(i));
    }
  }

  pragma "no doc"
  iter _tuple.these(param tag:iterKind)
      where tag == iterKind.leader
  {

    const numTasks = if dataParTasksPerLocale==0 then here.maxTaskPar
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

  pragma "no doc"
  iter _tuple.these(param tag:iterKind, followThis)
      where tag == iterKind.follower
  {
    for i in followThis do yield this(i);
  }

  //
  // tuple methods
  //
  pragma "no doc"
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
    var c: complex(64) = noinit;
    // There is no point allowing this to default initialize, we're just going
    // to overwrite it anyways
    c.re = x(1):real(32);
    c.im = x(2):real(32);
    return c;
  }

  inline proc _cast(type t, x: (?,?)) where t == complex(128) {
    var c: complex(128) = noinit;
    // There is no point allowing this to default initialize, we're just going
    // to overwrite it anyways
    c.re = x(1):real(64);
    c.im = x(2):real(64);
    return c;
  }

  //
  // General tuple cast function
  //
  pragma "tuple cast fn"
  inline proc _cast(type t, x: _tuple) where t:_tuple {
    // body filled in during resolution
  }

  //
  // helper function to return a tuple of all of the components in a
  // tuple except the first
  //
  inline proc chpl__tupleRest(t: _tuple) {
    inline proc chpl__tupleRestHelper(first, rest...)
      return rest;
    return chpl__tupleRestHelper((...t));
  }

  //
  // standard overloaded unary operators on tuples.
  //
  inline proc +(a: _tuple) {
    var result: a.type;
    for param d in 1..a.size do
      result(d) = +a(d);
    return result;
  }

  inline proc -(a: _tuple) {
    var result: a.type;
    for param d in 1..a.size do
      result(d) = -a(d);
    return result;
  }

  inline proc ~(a: _tuple) {
    var result: a.type;
    for param d in 1..a.size do
      result(d) = ~a(d);
    return result;
  }

  inline proc !(a: _tuple) {
    var result: a.type;
    for param d in 1..a.size do
      result(d) = !a(d);
    return result;
  }

  /*
    Returns a tuple of type t with each component set to ``max``
    of the type in the corresponding component of the argument.
  */
  proc max(type t): t where isTupleType(t) {
    var result: t;
    for param i in 1..result.size {
      result(i) = max(t(i));
    }
    return result;
  }

  /*
  Returns a tuple of type t with each component set to ``min``
  of the type in the corresponding component of the argument.
  */
  proc min(type t): t where isTupleType(t) {
    var result: t;
    for param i in 1..result.size {
      result(i) = min(t(i));
    }
    return result;
  }

  proc chpl_TwoHomogTuples(t1, t2) param {
    return isHomogeneousTuple(t1) && isHomogeneousTuple(t2);
  }

  //
  // standard overloaded binary operators on tuples.  Each pair starts
  // with a case optimized for homogeneous tuples (because the result
  // type is easy to express)...
  inline proc +(a: _tuple, b: _tuple) where chpl_TwoHomogTuples(a,b) {
    if a.size != b.size then
      compilerError("tuple operands to + have different sizes");

    var result: a.size*(a(1) + b(1)).type;
    for param d in 1..a.size do
      result(d) = a(d) + b(d);

    return result;
  }

  // ...followed by a more complicated/general case for heterogeneous
  // tuples because the result type is more complex to describe.
  //
  inline proc +(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to + have different sizes");
    if a.size == 1 then
      return (a(1)+b(1),);
    else
      return (a(1)+b(1), (...chpl__tupleRest(a)+chpl__tupleRest(b)));
  }


  inline proc -(a: _tuple, b: _tuple) where chpl_TwoHomogTuples(a,b) {
    if a.size != b.size then
      compilerError("tuple operands to - have different sizes");

    var result: a.size*(a(1) - b(1)).type;
    for param d in 1..a.size do
      result(d) = a(d) - b(d);

    return result;
  }

  inline proc -(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to - have different sizes");
    if a.size == 1 then
      return (a(1)-b(1),);
    else
      return (a(1)-b(1), (...chpl__tupleRest(a)-chpl__tupleRest(b)));
  }


  inline proc *(a: _tuple, b: _tuple) where chpl_TwoHomogTuples(a,b) {
    if a.size != b.size then
      compilerError("tuple operands to * have different sizes");

    var result: a.size*(a(1) * b(1)).type;
    for param d in 1..a.size do
      result(d) = a(d) * b(d);

    return result;
  }

  inline proc *(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to * have different sizes");
    if a.size == 1 then
      return (a(1)*b(1),);
    else
      return (a(1)*b(1), (...chpl__tupleRest(a)*chpl__tupleRest(b)));
  }

  inline proc /(a: _tuple, b: _tuple) where chpl_TwoHomogTuples(a,b) {
    if a.size != b.size then
      compilerError("tuple operands to / have different sizes");

    var result: a.size*(a(1) / b(1)).type;
    for param d in 1..a.size do
      result(d) = a(d) / b(d);

    return result;
  }

  inline proc /(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to / have different sizes");
    if a.size == 1 then
      return (a(1)/b(1),);
    else
      return (a(1)/b(1), (...chpl__tupleRest(a)/chpl__tupleRest(b)));
  }

  inline proc %(a: _tuple, b: _tuple) where chpl_TwoHomogTuples(a,b) {
    if a.size != b.size then
      compilerError("tuple operands to % have different sizes");

    var result: a.size*(a(1) % b(1)).type;
    for param d in 1..a.size do
      result(d) = a(d) % b(d);

    return result;
  }

  inline proc %(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to % have different sizes");
    if a.size == 1 then
      return (a(1)%b(1),);
    else
      return (a(1)%b(1), (...chpl__tupleRest(a)%chpl__tupleRest(b)));
  }

  inline proc **(a: _tuple, b: _tuple) where chpl_TwoHomogTuples(a,b) {
    if a.size != b.size then
      compilerError("tuple operands to ** have different sizes");

    var result: a.size*(a(1) ** b(1)).type;
    for param d in 1..a.size do
      result(d) = a(d) ** b(d);

    return result;
  }

  inline proc **(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to ** have different sizes");
    if a.size == 1 then
      return (a(1)**b(1),);
    else
      return (a(1)**b(1), (...chpl__tupleRest(a)**chpl__tupleRest(b)));
  }

  inline proc &(a: _tuple, b: _tuple) where chpl_TwoHomogTuples(a,b) {
    if a.size != b.size then
      compilerError("tuple operands to & have different sizes");

    var result: a.size*(a(1) & b(1)).type;
    for param d in 1..a.size do
      result(d) = a(d) & b(d);

    return result;
  }

  inline proc &(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to & have different sizes");
    if a.size == 1 then
      return (a(1)&b(1),);
    else
      return (a(1)&b(1), (...chpl__tupleRest(a)&chpl__tupleRest(b)));
  }

  inline proc |(a: _tuple, b: _tuple) where chpl_TwoHomogTuples(a,b) {
    if a.size != b.size then
      compilerError("tuple operands to | have different sizes");

    var result: a.size*(a(1) | b(1)).type;
    for param d in 1..a.size do
      result(d) = a(d) | b(d);

    return result;
  }

  inline proc |(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to | have different sizes");
    if a.size == 1 then
      return (a(1)|b(1),);
    else
      return (a(1)|b(1), (...chpl__tupleRest(a)|chpl__tupleRest(b)));
  }

  inline proc ^(a: _tuple, b: _tuple) where chpl_TwoHomogTuples(a,b) {
    if a.size != b.size then
      compilerError("tuple operands to ^ have different sizes");

    var result: a.size*(a(1) ^ b(1)).type;
    for param d in 1..a.size do
      result(d) = a(d) ^ b(d);

    return result;
  }

  inline proc ^(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to ^ have different sizes");
    if a.size == 1 then
      return (a(1)^b(1),);
    else
      return (a(1)^b(1), (...chpl__tupleRest(a)^chpl__tupleRest(b)));
  }

  inline proc <<(a: _tuple, b: _tuple) where chpl_TwoHomogTuples(a,b) {
    if a.size != b.size then
      compilerError("tuple operands to << have different sizes");

    var result: a.size*(a(1) << b(1)).type;
    for param d in 1..a.size do
      result(d) = a(d) << b(d);

    return result;
  }

  inline proc <<(a: _tuple, b: _tuple) {
    if a.size != b.size then
      compilerError("tuple operands to << have different sizes");
    if a.size == 1 then
      return (a(1)<<b(1),);
    else
      return (a(1)<<b(1), (...chpl__tupleRest(a)<<chpl__tupleRest(b)));
  }

  inline proc >>(a: _tuple, b: _tuple) where chpl_TwoHomogTuples(a,b) {
    if a.size != b.size then
      compilerError("tuple operands to >> have different sizes");

    var result: a.size*(a(1) >> b(1)).type;
    for param d in 1..a.size do
      result(d) = a(d) >> b(d);

    return result;
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

  //
  // standard overloaded binary operators on homog tuple / scalar
  // pairs.
  //
  inline proc +(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    var result: x.size * x(1).type;
    for param d in 1..x.size do
      result(d) = x(d) + y;
    return result;
  }

  inline proc +(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                       t: (y(1).type)  {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x + y(d);
    return result;
  }

  inline proc -(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    var result: x.size * x(1).type;
    for param d in 1..x.size do
      result(d) = x(d) - y;
    return result;
  }

  inline proc -(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                       t: (y(1).type)  {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x - y(d);
    return result;
  }

  inline proc *(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    var result: x.size * x(1).type;
    for param d in 1..x.size do
      result(d) = x(d) * y;
    return result;
  }

  inline proc *(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                       x: (y(1).type)  {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x * y(d);
    return result;
  }

  inline proc /(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    var result: x.size * x(1).type;
    for param d in 1..x.size do
      result(d) = x(d) / y;
    return result;
  }

  inline proc /(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                       t: (y(1).type)  {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x / y(d);
    return result;
  }

  inline proc %(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    var result: x.size * x(1).type;
    for param d in 1..x.size do
      result(d) = x(d) % y;
    return result;
  }

  inline proc %(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                       t: (y(1).type)  {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x % y(d);
    return result;
  }

  inline proc **(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    var result: x.size * x(1).type;
    for param d in 1..x.size do
      result(d) = x(d) ** y;
    return result;
  }

  inline proc **(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                       t: (y(1).type)  {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x ** y(d);
    return result;
  }

  inline proc &(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    var result: x.size * x(1).type;
    for param d in 1..x.size do
      result(d) = x(d) & y;
    return result;
  }

  inline proc &(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                       t: (y(1).type)  {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x & y(d);
    return result;
  }

  inline proc |(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    var result: x.size * x(1).type;
    for param d in 1..x.size do
      result(d) = x(d) | y;
    return result;
  }

  inline proc |(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                       t: (y(1).type)  {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x | y(d);
    return result;
  }

  inline proc ^(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    var result: x.size * x(1).type;
    for param d in 1..x.size do
      result(d) = x(d) ^ y;
    return result;
  }

  inline proc ^(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                       t: (y(1).type)  {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x ^ y(d);
    return result;
  }

  inline proc <<(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    var result: x.size * x(1).type;
    for param d in 1..x.size do
      result(d) = x(d) << y;
    return result;
  }

  inline proc <<(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                       t: (y(1).type)  {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x << y(d);
    return result;
  }

  inline proc >>(x: _tuple, y: x(1).type) where isHomogeneousTuple(x) {
    var result: x.size * x(1).type;
    for param d in 1..x.size do
      result(d) = x(d) >> y;
    return result;
  }

  inline proc >>(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                       t: (y(1).type)  {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x >> y(d);
    return result;
  }



}
