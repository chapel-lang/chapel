/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
  private use ChapelStandard;

  pragma "tuple" record _tuple {
    param size : int;
  }

  pragma "tuple init fn"
  inline proc chpl__init_tuple(param size : int) {
    // body inserted during generic instantiation
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
  pragma "suppress lvalue error"
  // suppressing lvalue errors -- workaround forall exprs yielding owned
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

  pragma "no doc"
  proc *(param p: uint, type t) type {
    if p > max(int) then
      compilerError("Tuples of size >" + max(int):string + " are not currently supported");
    param pAsInt = p: int;
    return pAsInt*t;
  }

  pragma "no doc"
  pragma "last resort"
  proc *(param p: bool, type t) type {
    compilerError("Tuple types cannot be defined using boolean sizes");
  }

  pragma "no doc"
  pragma "last resort"
  proc *(p: bool, type t) type {
    compilerError("Tuple types cannot be defined using boolean sizes");
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
  proc *(p: integral, type t) type {
    compilerError("tuple size must be known at compile-time");
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

  pragma "no doc"
  proc _check_tuple_var_decl(x: _tuple, param p) param {
    if p == x.size {
      return true;
    } else {
      compilerError("tuple size must match the number of grouped variables");
      return false;
    }
  }
  pragma "no doc"
  proc _check_tuple_var_decl(x, param p) param {
    compilerError("illegal tuple variable declaration with non-tuple initializer");
    return false;
  }


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
  pragma "reference to const when const this"
  iter _tuple.these() ref
  {

    // If we hit this error, it generally means that the compiler wasn't
    // successful at unrolling a loop over a heterogeneous tuple, either
    // because the IR changed (in the event of a regression) or because
    // it's a pattern that isn't handled yet (such as zippered iteration
    // or a forall loop over a heterogeneous tuple).  See preFold.cpp,
    // specifically unrollHetTupleLoop().
    //
    if !isHomogeneousTuple(this) then
      compilerError("Heterogeneous tuples don't support this style of loop yet");

    if CHPL_WARN_TUPLE_ITERATION == "true" then
      compilerWarning("Iterating over tuples. If you intended to use zippered iteration, add the new keyword 'zip' before the tuple of iteratable expressions.");

    for i in 1..this.size {
      yield(this(i));
    }
  }

  pragma "no doc"
  pragma "reference to const when const this"
  iter _tuple.these(param tag:iterKind) ref
      where tag == iterKind.leader
  {

    const numTasks = if dataParTasksPerLocale==0 then here.maxTaskPar
                     else dataParTasksPerLocale;
    const ignoreRunning = dataParIgnoreRunningTasks;
    const minIndicesPerTask = dataParMinGranularity;
    const length = this.size;
    const myRange = (0..#length,);
    var (numChunks, _) = _computeChunkStuff(numTasks, ignoreRunning,
                                            minIndicesPerTask,
                                            myRange);

    if numChunks == 1 {
      yield myRange;
    } else {
      coforall chunk in 0..#numChunks {
        // _computeBlock assumes 0-based ranges
        const (lo,hi) = _computeBlock(length, numChunks, chunk, length-1);
        yield (lo..hi,);
      }
    }
  }

  pragma "no doc"
  pragma "reference to const when const this"
  iter _tuple.these(param tag:iterKind, followThis: _tuple) ref
      where tag == iterKind.follower
  {
    if followThis.size != 1 then
      compilerError('Tuple zipped with incompatible iterator expression.');

    var fThis = followThis(1).translate(1);

    for i in fThis {
      yield this(i);
    }
  }

  //
  // tuple methods
  //

  //
  // tuple casts to complex(64) and complex(128)
  //
  // TODO: These could instead use 'noinit' and manually assign the fields.
  //
  // Note: statically inlining the _chpl_complex runtime functions is necessary
  // for good performance
  //
  inline proc _cast(type t, x: (?,?)) where t == complex(64) {
    extern proc _chpl_complex64(re:real(32),im:real(32)) : complex(64);
    return _chpl_complex64(x(1):real(32),x(2):real(32));
  }

  inline proc _cast(type t, x: (?,?)) where t == complex(128) {
    extern proc _chpl_complex128(re:real(64),im:real(64)):complex(128);
    return _chpl_complex128(x(1):real(64),x(2):real(64));
  }

  //
  // General tuple cast function
  //
  pragma "tuple cast fn"
  pragma "unsafe"
  inline proc _cast(type t:_tuple, x: _tuple) {
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
    if a.size != b.size {
      return false;
    } else {
      for param i in 1..a.size do
        if a(i) != b(i) then
          return false;
    }
    return true;
  }

  inline proc !=(a: _tuple, b: _tuple) {
    if a.size != b.size {
      return true;
    } else {
      for param i in 1..a.size do
        if a(i) != b(i) then
          return true;
    }
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
                                        isSubtype(t, (y(1).type)) {
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
                                        isSubtype(t, (y(1).type)) {
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
                                        isSubtype(t, (y(1).type)) {
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
                                        isSubtype(t, (y(1).type)) {
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
                                        isSubtype(t, (y(1).type)) {
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
                                         isSubtype(t, (y(1).type)) {
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
                                        isSubtype(t, (y(1).type)) {
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
                                        isSubtype(t, (y(1).type)) {
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
                                        isSubtype(t, (y(1).type)) {
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
                                         isSubtype(t, (y(1).type)) {
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
                                         isSubtype(t, (y(1).type)) {
    var result: y.size * y(1).type;
    for param d in 1..y.size do
      result(d) = x >> y(d);
    return result;
  }


}
