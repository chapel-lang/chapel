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

  // TODO the following can be removed?
  proc isProtoSlice(a) param { return isSubtype(a.type, chpl__protoSlice); }

  proc isArrayOrProtoSlice(a) param {
    return isArray(a) || isProtoSlice(a);
  }

  proc areBothArraysOrProtoSlices(a, b) {
    return isArrayOrProtoSlice(a) && isArrayOrProtoSlice(b);
  }

  record chpl__protoSlice {
    param rank;
    param isConst;
    var ptrToArr; // I want this to be a `forwarding ref` to the array
    var ranges;

    proc init() {
      // this constructor is called to create dummy protoSlices that will never
      // be used and removed from the AST. 
      this.rank = 1;
      this.isConst = true;

      var dummyArr = [1,];
      this.ptrToArr = c_addrOf(dummyArr);
      this.ranges = 1..0;
    }

    proc init(param isConst, ptrToArr, slicingExprs) {
      this.rank = ptrToArr.deref().rank;
      this.isConst = isConst;
      this.ptrToArr = ptrToArr;
      if allBounded(slicingExprs) {
        this.ranges = slicingExprs;
      }
      else if chpl__isTupleOfRanges(slicingExprs) {
        this.ranges = tupleOfRangesSlice(ptrToArr.deref().dims(), slicingExprs);
      }
      else {
        this.ranges = tupleOfRangesSlice(ptrToArr.deref().dims(),
                                         (slicingExprs,))[0];
        // [0] at the end makes it a range instead of tuple of ranges
      }
    }

    proc init=(other: chpl__protoSlice) {
      this.rank = other.rank;
      this.isConst = other.isConst;
      this.ptrToArr = other.ptrToArr;
      this.ranges = other.ranges;
      init this;
      extern proc printf(s...);
      printf("this is probably not what you want\n");
    }

    inline proc domOrRange where rank==1 {
      return ranges;
    }

    inline proc domOrRange where rank>1 {
      return {(...ranges)};
    }

    inline proc dims() where chpl__isTupleOfRanges(this.ranges) {
      return ranges;
    }

    inline proc dims() {
      return (ranges,);
    }

    inline proc rank param { return ptrToArr.deref().rank; }
    inline proc eltType type { return ptrToArr.deref().eltType; }
    inline proc _value { return ptrToArr.deref()._value; }

    inline proc sizeAs(type t) where rank==1 {
      return ranges.sizeAs(t);
    }

    inline proc sizeAs(type t) {
      var size = 1:t;
      for param r in 0..<rank {
        size *= ranges[r].sizeAs(t);
      }
      return size;
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

  proc chpl__createProtoSlice(ref Arr, slicingExprs ...)
      where chpl__baseTypeSupportAVE(Arr) &&
            chpl__isTupleOfRanges(slicingExprs) {
    if slicingExprs.size == 1 then
      return new chpl__protoSlice(isConst=false, c_addrOf(Arr),
                                  slicingExprs[0]);
    else
      return new chpl__protoSlice(isConst=false, c_addrOf(Arr), slicingExprs);
  }

  proc chpl__createConstProtoSlice(const ref Arr, slicingExprs ...)
      where chpl__baseTypeSupportAVE(Arr) &&
            chpl__isTupleOfRanges(slicingExprs) {
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

  extern proc printf(s...);

  proc chpl__indexingExprsSupportAVE(indexingExprs...) param: bool {
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
           chpl__indexingExprsSupportAVE((...indexingExprs));
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
