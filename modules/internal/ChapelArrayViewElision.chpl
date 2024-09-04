/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

module ChapelArrayViewElision {
  use ChapelBase only iterKind;
  use ChapelRange;
  use DefaultRectangular;
  use CTypes;
  use ChapelArray only _validRankChangeArgs;

  //
  // compiler interface
  //
  // Calls to these functions are inserted by the compiler. See
  // compiler/optimizations/arrayViewElision.cpp for how the compiler makes use
  // of these functions.

  proc chpl__createProtoSlice(ref Arr, slicingExprs ...)
      where chpl__createProtoSliceArgCheck(Arr, slicingExprs) {

    if slicingExprs.size == 1 then
      return new chpl__protoSlice(isConst=false, c_addrOf(Arr),
                                  slicingExprs[0]);
    else
      return new chpl__protoSlice(isConst=false, c_addrOf(Arr), slicingExprs);
  }

  proc chpl__createConstProtoSlice(const ref Arr, slicingExprs ...)
      where chpl__createProtoSliceArgCheck(Arr, slicingExprs) {

    if slicingExprs.size == 1 {
      return new chpl__protoSlice(isConst=true, c_addrOfConst(Arr),
                                  slicingExprs[0]);
    }
    else
      return new chpl__protoSlice(isConst=true, c_addrOfConst(Arr),
                                  slicingExprs);
  }

  // catch-all: nothing here is supported, just pretend creating a proto slice.
  // The branch where this call is will be dropped during resolution. We just
  // want to avoid resolution errors before that happens
  proc chpl__createProtoSlice(x, slicingExprs... ) {
    return new chpl__protoSlice();
  }

  // catch-all: nothing here is supported, just pretend creating a proto slice.
  // The branch where this call is will be dropped during resolution. We just
  // want to avoid resolution errors before that happens
  proc chpl__createConstProtoSlice(x, slicingExprs... ) {
    return new chpl__protoSlice();
  }

  proc chpl__ave_exprCanBeProtoSlice(base, idxExprs...) param: bool {
    return chpl__ave_baseTypeSupports(base) &&
           chpl__ave_idxExprsSupport(base.idxType, (...idxExprs));
  }

  proc chpl__ave_protoSlicesSupportAssignment(a: chpl__protoSlice,
                                              b: chpl__protoSlice) param: bool {
    if a.isRankChange != b.isRankChange then return false; //or assert?

    if !a.isRankChange then return true; // nothing else to check

    // we want to check that if there are integrals in the original slicing
    // expressions, they are at the same rank. In other words, if we are working
    // with rank-changes, we want to make sure that the collapsed dims on both
    // sides match
    type aType = a.slicingExprType;
    type bType = b.slicingExprType;
    compilerAssert(a.slicingExprType.size == b.slicingExprType.size);
    for param i in 0..<a.slicingExprType.size {
      if ( ( isRangeType(aType[i]) && !isRangeType(bType[i])) ||
           (!isRangeType(aType[i]) &&  isRangeType(bType[i])) ) {
        return false;
      }
    }

    return true;
  }

  //
  // proto slice type
  //
  // This is the type we create in lieu of full-blown array views. The key
  // functionality for this record are:
  //
  // 1. Supporting assignment with `=`
  // 2. Supporting bulk transfer
  // 3. Supporting serial and parallel iterations for non-bulk array transfer.
  //
  // Gotchas/caveats for the implementation:
  //
  // 1. We don't have `ref` fields yet. So, `ptrToArr` is used, and it stores
  //    the `c_addrOf` of the array in question. So it points to the _array
  //    record.
  // 2. `var ranges` is tricky. We want to avoid creating domains unless given
  //    by the user. In addition, I also wanted to avoid creating tuples, again,
  //    unless given by the user. An important context for this optimization is
  //    that it is for small array transfers, so small overheads like that could
  //    add up. In order to support that, `var ranges` could have 3 different
  //    types. It could be
  //      a. a range,
  //      b. a tuple of ranges,
  //      c. a domain (only if that's what the user gives us)
  //    There's some conditionals to handle that in the code, so heads up.
  // 3. Rank-change views are represented by slices. Right now, this
  //    optimization only fires for rank-changes if:
  //      a. both sides of the assignment are rank changes
  //      b. both rank-changes have the same number of arguments
  //      c. argument types match pair-wise (int to int, range to range)
  //    This implies that such rank-changes can be represented by slices where
  //    the collapsed dimensions could be ranges of size 1. `param isRankChange`
  //    field tells you whether a particular protoSlice is actually a
  //    rank-change logically.

  record chpl__protoSlice {
    param rank;
    param isConst;
    var ptrToArr; // I want this to be a `forwarding ref` to the array
    type slicingExprType;
    var ranges;
    param isRankChange = false;

    proc init() {
      // this constructor is called to create dummy protoSlices that will never
      // be used and removed from the AST.
      this.rank = 1;
      this.isConst = true;

      var dummyArr = [1,];
      this.ptrToArr = c_addrOf(dummyArr);

      var dummyRange = 1..0;
      this.slicingExprType = dummyRange.type;
      this.ranges = dummyRange;
    }

    proc init(param isConst, ptrToArr, slicingExprs) {
      this.rank = ptrToArr.deref().rank;
      this.isConst = isConst;
      this.ptrToArr = ptrToArr;
      this.slicingExprType = slicingExprs.type;
      if isDomain(slicingExprs) {
        this.ranges = slicingExprs;
      }
      else if isRange(slicingExprs) {
        if allBounded(slicingExprs) {
          this.ranges = slicingExprs;
        }
        else {
          this.ranges = tupleOfRangesSlice(ptrToArr.deref().dims(),
                                           (slicingExprs,))[0];
          // [0] at the end makes it a range instead of tuple of ranges
        }
      }
      else if chpl__isTupleOfRanges(slicingExprs) {
        this.ranges = tupleOfRangesSlice(ptrToArr.deref().dims(), slicingExprs);
      }
      else if _validRankChangeArgs(slicingExprs, ptrToArr.deref().idxType) {
        this.ranges = tupleOfRangesSlice(ptrToArr.deref().dims(),
                                         rangify(slicingExprs));
        this.isRankChange = true;
      }
      else {
        this.ranges = 1..0; // needed to keep the compiler happy
        compilerError("Unexpected slicing expr in chpl__protoSlice.init");
      }
    }

    proc init=(other: chpl__protoSlice) {
      this.rank = other.rank;
      this.isConst = other.isConst;
      this.ptrToArr = other.ptrToArr;
      this.slicingExprType = other.slicingExprType;
      this.ranges = other.ranges;
      this.isRankChange = other.isRankChange;
      init this;
      halt("protoSlice copy initializer should never be called");
    }

    // 1D always returns a range
    inline proc domOrRange where rank==1 {
      if isDomain(ranges) then
        return ranges.dim[0];
      else
        return ranges;
    }

    inline proc domOrRange where rank>1 {
      if isDomain(ranges) then
        return ranges;
      else
        return {(...ranges)};
    }

    inline proc dims() {
      if chpl__isTupleOfRanges(this.ranges) {
        return ranges;
      }
      else if isDomain(this.ranges) {
        return ranges.dims();
      }
      else if isRange(this.ranges) {
        return (ranges,);
      }
      else {
        compilerError("Unhandled case in chpl__protoSlice.dims()");
      }
    }

    inline proc rank param { return ptrToArr.deref().rank; }
    inline proc eltType type { return ptrToArr.deref().eltType; }
    inline proc _value { return ptrToArr.deref()._value; }

    inline proc sizeAs(type t) where rank==1 {
      return ranges.sizeAs(t);
    }

    inline proc sizeAs(type t) {
      if isDomain(this.ranges) {
        return ranges.sizeAs(t);
      }
      else {
        var size = 1:t;
        for param r in 0..<rank {
          size *= ranges[r].sizeAs(t);
        }
        return size;
      }
    }

    inline proc isRectangular() param {
      return ptrToArr.deref().isRectangular();
    }

    // NOTE: the iterators below are not fun to look at because of all the
    // repetition. We want to have `ref` yield intent if the protoSlice was
    // based on a non-const array and `const` yield intent if it was a const
    // array. The pragma `reference to const when const this` doesn't work here,
    // nor we have a good support for yield intent overloading.
    // https://github.com/chapel-lang/chapel/issues/7000 is related.
    iter these() ref where !isConst {
      if rank == 1 then {
        foreach elem in chpl__serialViewIter1D(ptrToArr.deref()._instance,
                                               domOrRange) {
          yield elem;
        }
      }
      else {

        /*
          Storing `inst` here and iterating over `inst` doesn't seem to work.
          Check the arrays primer for how that causes issues. Potentially an
          iterator inlining issue, or memory cleanup going sideways.

          const inst = domOrRange._instance;
        */
        foreach elem in chpl__serialViewIter(ptrToArr.deref()._instance,
                                             domOrRange._instance) {
          yield elem;
        }
      }
    }

    iter these() const ref where isConst {
      if rank == 1 then {
        foreach elem in chpl__serialViewIter1D(ptrToArr.deref()._instance,
                                               domOrRange) {
          yield elem;
        }
      }
      else {

        /*
          Storing `inst` here and iterating over `inst` doesn't seem to work.
          Check the arrays primer for how that causes issues. Potentially an
          iterator inlining issue, or memory cleanup going sideways.

          const inst = domOrRange._instance;
        */
        foreach elem in chpl__serialViewIter(ptrToArr.deref()._instance,
                                             domOrRange._instance) {
          yield elem;
        }
      }
    }

    iter these(param tag: iterKind) where tag==iterKind.leader {
      for followThis in domOrRange.these(iterKind.leader) {
        yield followThis;
      }
    }

    iter these(param tag: iterKind, followThis) ref
        where tag==iterKind.follower && !isConst {
      ref arr = ptrToArr.deref();
      foreach i in domOrRange.these(iterKind.follower, followThis) {
        yield arr[i];
      }
    }
    iter these(param tag: iterKind, followThis) const ref
        where tag==iterKind.follower && isConst{
      const ref arr = ptrToArr.deref();
      foreach i in domOrRange.these(iterKind.follower, followThis) {
        yield arr[i];
      }
    }

    proc supportsShortArrayTransfer() param {
      return ptrToArr.deref().domain.supportsShortArrayTransfer();
    }
  }

  // we need this as otherwise compiler-generated equality operator requires
  // both sides to have the exact same type. For the most part, we want that for
  // optimization, however being able to resolve this operator for different
  // types keeps the compiler happy. IOW, the cases where we need this are
  // typically cases where the optimization is not supported in the first place.
  operator ==(const ref lhs: chpl__protoSlice(?),
              const ref rhs: chpl__protoSlice(?)) {
    // we have to compare types. Otherwise, comparing ranges fields can result
    // in confusing errors if one was a range and the other was a domain as that
    // comparison would be a promoted comparison.
    return lhs.type == rhs.type &&
           lhs.rank == rhs.rank &&
           lhs.ptrToArr == rhs.ptrToArr &&
           lhs.ranges == rhs.ranges;
  }


  // we need this because we support assignment between two proto slices
  inline operator :(ref a: chpl__protoSlice, type b: chpl__protoSlice) {
    compilerError("Should never cast proto slices");
  }

  proc isProtoSlice(a) param { return isSubtype(a.type, chpl__protoSlice); }

  //
  // private interface
  //

  private proc chpl__createProtoSliceArgCheck(Arr, slicingExprs) param: bool {
    compilerAssert(isTuple(slicingExprs));

    return chpl__ave_baseTypeSupports(Arr) &&
           (chpl__isTupleOfRanges(slicingExprs) ||
            (slicingExprs.size == 1 && isDomain(slicingExprs[0])) ||
            _validRankChangeArgs(slicingExprs, Arr.idxType));
  }

  private proc chpl__ave_baseTypeSupports(base) param: bool {
    import Reflection;
    return isArray(base) && // also could be a view?
           base.domain.supportsArrayViewElision() &&
           Reflection.canResolve("c_addrOf", base);
  }

  private proc chpl__ave_idxExprsSupport(type idxType,
                                         idxExprs...) param: bool {
    for param tid in 0..<idxExprs.size {
      if !isRange(idxExprs[tid]) {
        // should we also check for homogeneous tuples as we don't have support
        // for rank-change just yet?
        return false;
      }
      else if !(idxExprs[tid].strides == strideKind.positive ||
                idxExprs[tid].strides == strideKind.one) {
        // negative strided slices are not supported and generate a warning.
        // Instead of trying to generate the warning, just avoid covering
        // unsupported things here
        return false;
      }
    }
    return true;
  }

  private proc chpl__ave_idxExprsSupport(type idxType,
                                         idxExprs: domain) param: bool {
    if !(idxExprs.strides == strideKind.positive ||
         idxExprs.strides == strideKind.one) {
      // negative strided slices are not supported and generate a warning.
      // Instead of trying to generate the warning, just avoid covering
      // unsupported things here
      return false;
    }
    return true;
  }

  private proc chpl__ave_idxExprsSupport(type idxType,
                                         idxExprs...) param: bool
      where _validRankChangeArgs(idxExprs, idxType) {
    return true;
  }

  private proc allBounded(ranges: range(?)) param {
    return ranges.bounds == boundKind.both;
  }

  private proc allBounded(ranges) param {
    if chpl__isTupleOfRanges(ranges) {
      for param i in 0..<ranges.size {
        if ranges[i].bounds != boundKind.both {
          return false;
        }
      }
      return true;
    }
    compilerError("Unexpected type to allBounded");
    return false;
  }

  private proc rangify(rcTup) {
    compilerAssert(isTuple(rcTup));

    proc createRangifiedType(type rcTupType) type {
      proc recurse(type curType, param dim) type {
        if dim == rcTupType.size {
          return curType;
        }
        else if isRangeType(rcTupType[dim]) {
          if dim == 0 {
            return recurse((rcTupType[dim],), dim+1);
          }
          else {
            return recurse(((...curType), rcTupType[dim]), dim+1);
          }
        }
        else {
          const dummy: rcTupType[dim];
          const dummyRange = dummy..dummy;
          if dim == 0 {
            return recurse((dummyRange.type,), dim+1);
          }
          else {
            return recurse(((...curType), dummyRange.type), dim+1);
          }
        }
      }

      return recurse(nothing, 0);
    }

    var ret: createRangifiedType(rcTup.type);

    for param i in 0..<ret.size {
      if isRange(rcTup[i]) {
        ret[i] = rcTup[i];
      }
      else {
        ret[i] = rcTup[i]..rcTup[i];
      }
    }

    return ret;
  }
}
