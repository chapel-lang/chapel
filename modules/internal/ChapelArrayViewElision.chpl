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

  proc isProtoSlice(a) param { return isSubtype(a.type, chpl__protoSlice); }

  proc isArrayOrProtoSlice(a) param {
    return isArray(a) || isProtoSlice(a);
  }

  proc areBothArraysOrProtoSlices(a, b) {
    return isArrayOrProtoSlice(a) && isArrayOrProtoSlice(b);
  }

  record chpl__protoSlice {
    param rank;
    type idxType;
    var ptrToArr; // I want this to be a `forwarding ref` to the array
    var ranges;

    proc init() {
      // this constructor is called to create dummy protoSlices that will never
      // be used and removed from the AST. 
      this.rank = 1;
      this.idxType = int;

      var dummyArr = [1,];
      this.ptrToArr = c_addrOf(dummyArr);
      this.ranges = (1..0,);
    }

    proc init(ptrToArr, slicingExprs) {
      this.rank = ptrToArr.deref().rank;
      this.idxType = ptrToArr.deref().idxType;
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
      this.idxType = other.idxType;
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

    iter these() ref {
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

    iter these(param tag: iterKind, followThis) ref where tag==iterKind.follower {
      ref arr = ptrToArr.deref();
      foreach i in domOrRange.these(iterKind.follower, followThis) {
        yield arr[i];
      }
    }
  }

  // TODO can we allow const arrs to be passed here without breaking constness
  // guarantees?
  // TODO we can also accept domains and ints (rank-change)
  proc chpl__createProtoSlice(ref Arr, slicingExprs: range(?))
      where chpl__baseTypeSupportAVE(Arr) {
    return new chpl__protoSlice(c_addrOf(Arr), slicingExprs);
  }

  pragma "last resort"
  proc chpl__createProtoSlice(ref Arr, slicingExprs:range(?) ...)
      where chpl__baseTypeSupportAVE(Arr) {
    return new chpl__protoSlice(c_addrOf(Arr), slicingExprs);
  }

  pragma "last resort"
  proc chpl__createProtoSlice(const ref Arr, slicingExprs: range(?))
      where chpl__baseTypeSupportAVE(Arr) {
    return new chpl__protoSlice(c_addrOfConst(Arr), slicingExprs);
  }

  pragma "last resort"
  proc chpl__createProtoSlice(const ref Arr, slicingExprs:range(?) ...)
      where chpl__baseTypeSupportAVE(Arr) {
    return new chpl__protoSlice(c_addrOfConst(Arr), slicingExprs);
  }

  pragma "last resort"
  proc chpl__createProtoSlice(const ref Arr, slicingExprs... ) {
    // this is an array access. This call will be eliminated later in
    // resolution, but we want it to live for a bit for easier resolution
    return new chpl__protoSlice();
  }


  proc chpl__exprSupportsViewTransfer(base, exprs...) param {
    if base.isDefaultRectangular() {



    }

    return false;
  }

  proc chpl__baseTypeSupportAVE(base) param: bool {
    import Reflection;
    return isArray(base) && // also could be a view?
           isSubtype(base._instance.type, DefaultRectangularArr) &&
           Reflection.canResolve("c_addrOf", base);
  }

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
}
