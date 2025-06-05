/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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



@chpldoc.nodoc
module ShallowCopy {
  private use CTypes;
  private use OS.POSIX;

  // The shallowCopy / shallowSwap code needs to be able to copy/swap
  // _array records. But c_ptrTo on an _array will return a pointer to
  // the first element, which messes up the shallowCopy/shallowSwap code
  //
  // As a workaround, this function just returns a pointer to the argument,
  // whether or not it is an array.
  //
  // TODO: these should be replaced with the appropriate c_addrOf[Const] calls
  private inline proc ptrTo(ref x) {
    return c_pointer_return(x);
  }
  private inline proc ptrToConst(const ref x) {
    return c_pointer_return_const(x);
  }

  // These shallow copy functions "move" a record around
  // (i.e. they neither swap nor call a copy initializer).
  //
  // TODO: move these out of the Sort module and/or consider
  // language support for it. See issue #14576.

  inline proc shallowCopy(ref dst, ref src) {
    type st = __primitive("static typeof", dst);
    if isPODType(st) {
      dst = src;
    } else {
      var size = c_sizeof(st);
      memcpy(ptrTo(dst), ptrTo(src), size);
      if boundsChecking {
        // The version moved from should never be used again,
        // but we clear it out just in case.
        memset(ptrTo(src), 0, size);
      }
    }
  }

  // returns the result of shallow copying src
  pragma "unsafe"
  pragma "no copy return"
  inline proc shallowCopyInit(ref src) {
    type st = __primitive("static typeof", src);
    pragma "no init"
    pragma "no auto destroy"
    var dst: st;
    shallowCopy(dst, src);
    return dst;
  }

  pragma "unsafe"
  inline proc shallowSwap(ref lhs:?t, ref rhs:t) {
    type st = __primitive("static typeof", lhs);
    pragma "no init"
    pragma "no auto destroy"
    var tmp: st;
    if isPODType(st) {
      tmp = lhs;
      lhs = rhs;
      rhs = tmp;
    } else {
      var size = c_sizeof(st);
      // tmp = lhs
      memcpy(ptrTo(tmp), ptrTo(lhs), size);
      // lhs = rhs
      memcpy(ptrTo(lhs), ptrTo(rhs), size);
      // rhs = tmp
      memcpy(ptrTo(rhs), ptrTo(tmp), size);
    }
  }

  inline proc shallowCopy(ref A, dst, src, nElts) {

    // Ideally this would just be
    //A[dst..#nElts] = A[src..#nElts];

    type idxType = A.idxType;
    const dst_idx = dst:idxType;
    const src_idx = src:idxType;
    const nElts_idx = nElts:idxType;

    if boundsChecking {
      assert(nElts > 0);
      // check that the bounds can be safely converted to array indices
      dst.safeCast(idxType);
      src.safeCast(idxType);
      nElts.safeCast(idxType);
      // check that the domain contains the indices
      assert(A.domain.contains(dst_idx..#nElts_idx));
      assert(A.domain.contains(src_idx..#nElts_idx));
    }

    if A._instance.isDefaultRectangular() {
      type st = __primitive("static field type", A._value, "eltType");
      var size = (nElts:c_size_t)*c_sizeof(st);
      memcpy(ptrTo(A[dst_idx]), ptrTo(A[src_idx]), size);
    } else {
      var ok = chpl__bulkTransferArray(/*dst*/ A,
                                       {dst_idx..#nElts_idx},
                                       /*src*/ A,
                                       {src_idx..#nElts_idx});
      if !ok {
        // fall back on PRIM_ASSIGN to move the elements
        // at present, this is needed for Cyclic and Replicated.
        foreach i in 0..#nElts {
          __primitive("=", A[dst_idx+i:idxType], A[src_idx+i:idxType]);
        }
      }
    }
  }
  inline proc shallowCopy(ref DstA, dst, SrcA, src, nElts) {

    // Ideally this would just be
    //DstA[dst..#nElts] = SrcA[src..#nElts];

    const dst_idx = dst:DstA.idxType;
    const src_idx = src:SrcA.idxType;
    const nElts_dst_idx = nElts:DstA.idxType;
    const nElts_src_idx = nElts:SrcA.idxType;

    if boundsChecking {
      assert(nElts > 0);
      // check that the bounds can be safely converted to array indices
      dst.safeCast(DstA.idxType);
      src.safeCast(SrcA.idxType);
      nElts.safeCast(DstA.idxType);
      nElts.safeCast(SrcA.idxType);
      // check that the domain contains the indices
      assert(DstA.domain.contains(dst_idx..#nElts_dst_idx));
      assert(SrcA.domain.contains(src_idx..#nElts_src_idx));
    }


    if DstA._instance.isDefaultRectangular() &&
       SrcA._instance.isDefaultRectangular() {
      type st = __primitive("static field type", DstA._value, "eltType");
      var size = (nElts:c_size_t)*c_sizeof(st);
      memcpy(ptrTo(DstA[dst_idx]), ptrToConst(SrcA[src_idx]), size);
    } else {
      var ok = chpl__bulkTransferArray(/*dst*/ DstA,
                                       {dst_idx..#nElts_dst_idx},
                                       /*src*/ SrcA,
                                       {src_idx..#nElts_src_idx});
      if !ok {
        // fall back on PRIM_ASSIGN to move the elements
        // at present, this is needed for Cyclic and Replicated.
        foreach i in 0..#nElts_dst_idx {
          __primitive("=", DstA[dst_idx+i], SrcA[src_idx+i:SrcA.idxType]);
        }
      }
    }
  }
  proc shallowCopyPutGetRefs(ref dst, const ref src, numBytes: c_size_t) {
    if dst.locale.id == here.id {
      __primitive("chpl_comm_get", dst, src.locale.id, src, numBytes);
    } else if src.locale.id == here.id {
      __primitive("chpl_comm_put", src, dst.locale.id, dst, numBytes);
    } else {
      halt("Remote src and remote dst not yet supported");
    }
  }

  // For the case in which we know that the source and dest regions
  // are contiguous within a locale
  proc shallowCopyPutGet(ref DstA, dst, const ref SrcA, src, nElts) {
    var size = (nElts:c_size_t)*c_sizeof(DstA.eltType);
    shallowCopyPutGetRefs(DstA[dst], SrcA[src], size);
  }
}

