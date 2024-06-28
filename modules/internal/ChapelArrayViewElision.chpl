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

  // TODO the following can be removed?
  proc isProtoSlice(a) param { return isSubtype(a.type, chpl__protoSlice); }

  proc isArrayOrProtoSlice(a) param {
    return isArray(a) || isProtoSlice(a);
  }

  proc areBothArraysOrProtoSlices(a, b) {
    return isArrayOrProtoSlice(a) && isArrayOrProtoSlice(b);
  }

  proc rangify(rcTup) {
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


  record chpl__protoSlice {
    param rank;
    param isConst;
    var ptrToArr; // I want this to be a `forwarding ref` to the array
    var ranges;
    type slicingExprType;

    proc init() {
      // this constructor is called to create dummy protoSlices that will never
      // be used and removed from the AST. 
      this.rank = 1;
      this.isConst = true;

      var dummyArr = [1,];
      this.ptrToArr = c_addrOf(dummyArr);
      this.ranges = 1..0;
      this.slicingExprType = this.ranges.type;
    }

    proc init(param isConst, ptrToArr, slicingExprs) {
      this.rank = ptrToArr.deref().rank;
      this.isConst = isConst;
      this.ptrToArr = ptrToArr;
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
        this.ranges = rangify(slicingExprs);
      }
      else {
        this.ranges = 1..0; // needed to keep the compiler happy
        compilerError("Unexpected slicing expr in chpl__protoSlice.init");
      }
      this.slicingExprType = slicingExprs.type;
    }

    proc init=(other: chpl__protoSlice) {
      this.rank = other.rank;
      this.isConst = other.isConst;
      this.ptrToArr = other.ptrToArr;
      this.ranges = other.ranges;
      this.slicingExprType = other.slicingExprType;
      init this;
      halt("protoSlice copy initializer should never be called");
    }

    inline proc domOrRange where rank==1 {
      return ranges; // doesn't matter whether it is a domain or a range
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
    inline proc isRectangular() param { return ptrToArr.deref().isRectangular(); }

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
  }

  operator ==(const ref lhs: chpl__protoSlice(?),
              const ref rhs: chpl__protoSlice(?)) {
    return lhs.rank == rhs.rank &&
           lhs.ptrToArr == rhs.ptrToArr &&
           lhs.ranges == rhs.ranges;
  }

  proc chpl__createProtoSliceArgCheck(Arr, slicingExprs) param: bool {
    compilerAssert(isTuple(slicingExprs));

    return chpl__baseTypeSupportAVE(Arr) &&
           (chpl__isTupleOfRanges(slicingExprs) ||
            (slicingExprs.size == 1 && isDomain(slicingExprs[0])) ||
            _validRankChangeArgs(slicingExprs, Arr.idxType));
  }

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
      return new chpl__protoSlice(isConst=true, c_addrOfConst(Arr), slicingExprs);
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


  proc chpl__baseTypeSupportAVE(base) param: bool {
    import Reflection;
    return isArray(base) && // also could be a view?
           isSubtype(base._instance.type, DefaultRectangularArr) &&
           Reflection.canResolve("c_addrOf", base);
  }

  proc chpl__indexingExprsSupportAVE(type idxType, indexingExprs...) param: bool {
    for param tid in 0..<indexingExprs.size {
      if !isRange(indexingExprs[tid]) {
        // should we also check for homogeneous tuples as we don't have support
        // for rank-change just yet?
        return false;
      }
      else if !(indexingExprs[tid].strides == strideKind.positive ||
                indexingExprs[tid].strides == strideKind.one) {
        // negative strided slices are not supported and generate a warning.
        // Instead of trying to generate the warning, just avoid covering
        // unsupported things here
        return false;
      }
    }
    return true;
  }

  proc chpl__indexingExprsSupportAVE(type idxType, indexingExprs: domain) param: bool {
    if !(indexingExprs.strides == strideKind.positive ||
         indexingExprs.strides == strideKind.one) {
      // negative strided slices are not supported and generate a warning.
      // Instead of trying to generate the warning, just avoid covering
      // unsupported things here
      return false;
    }
    return true;
  }

  proc chpl__indexingExprsSupportAVE(type idxType, indexingExprs...) param: bool 
      where _validRankChangeArgs(indexingExprs, idxType) {
    return true;
  }

  inline proc chpl__bothLocal(const ref a, const ref b) {
    extern proc chpl_equals_localeID(const ref x, const ref y): bool;

    const aLoc = __primitive("_wide_get_locale", a._value);
    const bLoc = __primitive("_wide_get_locale", b._value);

    return chpl_equals_localeID(aLoc, bLoc) &&
           chpl_equals_localeID(aLoc, here_id);
  }

  proc chpl__typesSupportArrayViewElision(base,
                                          indexingExprs...) param: bool {
    return chpl__baseTypeSupportAVE(base) &&
           chpl__indexingExprsSupportAVE(base.idxType, (...indexingExprs));
  }

  proc chpl__ave_typesMatch(a: chpl__protoSlice, b: chpl__protoSlice) param: bool {
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

  inline operator :(ref a: chpl__protoSlice, type b: chpl__protoSlice) {
    compilerError("Should never cast proto slices");
  }
}
