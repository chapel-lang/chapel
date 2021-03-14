/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

// DefaultSparse.chpl
//
// COO (coordinate) storage where the indices/coordinates are stored in lexicographically
// sorted order (first by row and then by column).  The indices/coordinates are 0-based.
//
module DefaultSparse {
  use ChapelStandard;
  import RangeChunk;
  use DSIUtil;

  config param debugDefaultSparse = false;

  class DefaultSparseDom: BaseSparseDomImpl {
    var dist: unmanaged DefaultDist;
    var _nnz = 0;

    pragma "local field"
    var _indices: [nnzDom] index(rank, idxType);

    override proc linksDistribution() param return false;
    override proc dsiLinksDistribution() return false;

    proc init(param rank, type idxType, dist: unmanaged DefaultDist,
        parentDom: domain) {
      super.init(rank, idxType, parentDom);

      this.dist = dist;
    }

    override proc getNNZ(): int{
      return _nnz;
    }

    proc dsiBuildArray(type eltType, param initElts:bool) {
      return new unmanaged DefaultSparseArr(eltType=eltType,
                                            rank=rank, idxType=idxType,
                                            dom=_to_unmanaged(this),
                                            initElts=initElts);
    }

    // ?FromMMS: this doesn't appear to be an order independent yielding loop, why
    // is it marked as such
    pragma "order independent yielding loops"
    iter dsiIndsIterSafeForRemoving() {
      for i in 0..#_nnz by -1 {
        yield _indices(i);
      }
    }

    pragma "order independent yielding loops"
    iter these() {
      for i in 0..#_nnz {
        yield _indices(i);
      }
    }

    pragma "order independent yielding loops"
    iter these(param tag: iterKind) where tag == iterKind.standalone {
      const numElems = _nnz;
      const numChunks = _computeNumChunks(numElems): numElems.type;
      if debugDefaultSparse {
        writeln("DefaultSparseDom standalone: ", numChunks, " chunks, ",
                numElems, " elems");
      }

      // split our numElems elements over numChunks tasks
      if numChunks <= 1 {
        // ... except if 1, just use the current thread
        for i in 0..#numElems {
          yield _indices(i);
        }
      } else {
        coforall chunk in RangeChunk.chunks(0..#numElems, numChunks) {
          for i in chunk do
            yield _indices(i);
        }
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      const numElems = _nnz;
      const numChunks = _computeNumChunks(numElems): numElems.type;
      if debugDefaultSparse then
        writeln("DefaultSparseDom leader: ", numChunks, " chunks, ",
                numElems, " elems");

      // split our numElems elements over numChunks tasks
      if numChunks <= 1 then
        // ... except if 1, just use the current thread
        yield (this, 0, numElems-1);
      else
        coforall chunk in RangeChunk.chunks(0..#numElems, numChunks) do
          yield (this, chunk.first, chunk.last);
    }

    pragma "order independent yielding loops"
    iter these(param tag: iterKind, followThis:(?,?,?)) where tag == iterKind.follower {
      var (followThisDom, startIx, endIx) = followThis;

      if (followThisDom != this) then
        halt("Sparse domains can't be zippered with anything other than themselves and their arrays");
      if debugDefaultSparse then
        writeln("DefaultSparseDom follower: ", startIx, "..", endIx);

      for i in startIx..endIx do
        yield _indices(i);
    }

    iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
      compilerError("Sparse iterators can't yet be zippered with others");
      var dummy: rank * idxType;
      yield dummy;
    }

    // private
    proc find(ind) {
      use Search;
      //
      // sjd: unfortunate specialization for rank == 1
      //
      if rank == 1 && isTuple(ind) && ind.size == 1 then
        return binarySearch(_indices, ind(0), lo=0, hi=_nnz-1);
      else
        return binarySearch(_indices, ind, lo=0, hi=_nnz-1);
    }

    proc dsiMember(ind) { // ind should be verified to be index type
      const (found, loc) = find(ind);
      return found;
    }

    proc dsiFirst {
      return _indices[_indices.domain.first];
    }

    proc dsiLast {
      return _indices[_nnz-1];
    }

    proc add_help(ind) {
      // find position in nnzDom to insert new index
      const (found, insertPt) = find(ind);

      // if the index already existed, then return
      if (found) then return 0;  // 0 additions because already in there

      if boundsChecking then
        this.boundsCheck(ind);

      // increment number of nonzeroes
      _nnz += 1;

      const oldNNZDomSize = nnzDom.size;
      // double nnzDom if we've outgrown it; grab current size otherwise
      _grow(_nnz);

      // shift indices up
      for i in insertPt.._nnz-2 by -1 {
        _indices(i+1) = _indices(i);
      }

      _indices(insertPt) = ind;

      // shift all of the arrays up and initialize nonzeroes if
      // necessary
      //
      // BLC: Note: if rectangular arrays had a user-settable
      // initialization value, we could set it to be the IRV and skip
      // this second initialization of any new values in the array.
      // we could also eliminate the oldNNZDomSize variable
      for a in _arrs {
        a.sparseShiftArray(insertPt.._nnz-2, oldNNZDomSize..nnzDom.size-1);
      }

      return 1;
    }

    proc rem_help(ind) {
      // find position in nnzDom to insert new index
      const (found, insertPt) = find(ind);

      // if the index does not exist, then halt
      // why halt? - Engin, ?fromMMS: was this ever answered?
      if (!found) then
        halt("index not in domain: ", ind);

      // decrement number of nonzeroes
      _nnz -= 1;

      // TODO: should halve nnzDom if we've outgrown it; grab current
      // size otherwise
      /*
      var oldNNZDomSize = nnzDomSize;
      if (_nnz > nnzDomSize) {
        nnzDomSize = if (nnzDomSize) then 2*nnzDomSize else 1;
        nnzDom = [1..nnzDomSize];
      }
      */
      // shift indices up
      for i in insertPt.._nnz-1 {
        _indices(i) = _indices(i+1);
      }

      // shift all of the arrays up and initialize nonzeroes if
      // necessary
      //
      // BLC: Note: if rectangular arrays had a user-settable
      // initialization value, we could set it to be the IRV and skip
      // this second initialization of any new values in the array.
      // we could also eliminate the oldNNZDomSize variable
      for a in _arrs {
        a.sparseShiftArrayBack(insertPt.._nnz-1);
      }

      return 1; // number of items removed
    }

    proc dsiAdd(ind: idxType) where rank == 1 {
      return add_help(ind);
    }

    proc dsiRemove(ind: idxType) where rank == 1 {
      return rem_help(ind);
    }

    proc dsiAdd(ind: rank*idxType) {
      if (rank == 1) {
        return add_help(ind(0));
      } else {
        return add_help(ind);
      }
    }

    proc dsiRemove(ind: rank*idxType) {
      if (rank == 1) {
        return rem_help(ind(0));
      } else {
        return rem_help(ind);
      }
    }

    // this returns the position for the last sparse index added
    override proc bulkAdd_help(inds: [?indsDom] index(rank, idxType),
        dataSorted=false, isUnique=false, addOn=nilLocale){
      import Sort;

      if addOn != nilLocale {
        if addOn != this.locale {
          halt("Bulk index addition is only possible on the locale where the\
              sparse domain is created");
        }
      }

      bulkAdd_prepareInds(inds, dataSorted, isUnique, Sort.defaultComparator);

      if _nnz == 0 {

        const dupCount = if isUnique then 0 else _countDuplicates(inds);

        _nnz += inds.size-dupCount;
        _bulkGrow();

        var indIdx = _indices.domain.low;
        var prevIdx = parentDom.low-1;

        if isUnique {
          _indices[_indices.domain.low..#inds.size]=inds;
          return inds.size;
        }
        else {
          for i in inds {
            if i == prevIdx then continue;
            else prevIdx = i;

            _indices[indIdx] = i;
            indIdx += 1;
          }
          return indIdx-1;
        }
      }

      const (actualInsertPts, actualAddCnt) =
        __getActualInsertPts(this, inds, isUnique);

      const oldnnz = _nnz;
      _nnz += actualAddCnt;

      //grow nnzDom if necessary
      _bulkGrow();

      //linearly fill the new colIdx from backwards
      var newIndIdx = indsDom.high; //index into new indices
      var oldIndIdx = oldnnz-1; //index into old indices
      var newLoc = actualInsertPts[newIndIdx]; //its position-to-be in new dom
      while newLoc == -1 {
        newIndIdx -= 1;
        if newIndIdx == indsDom.low-1 then break; //there were duplicates -- now done
        newLoc = actualInsertPts[newIndIdx];
      }
      var arrShiftMap: [0..#oldnnz] int; //to map where data goes

      for i in 0..#_nnz by -1 {
        if oldIndIdx >= 0 && i > newLoc {
          //shift from old values
          _indices[i] = _indices[oldIndIdx];
          arrShiftMap[oldIndIdx] = i;
          oldIndIdx -= 1;
        }
        else if newIndIdx >= indsDom.low && i == newLoc {
          //put the new guy in
          _indices[i] = inds[newIndIdx];
          newIndIdx -= 1;
          if newIndIdx >= indsDom.low then
            newLoc = actualInsertPts[newIndIdx];
          else
            newLoc = -2; //finished new set
          while newLoc == -1 {
            newIndIdx -= 1;
            if newIndIdx == indsDom.low-1 then break; //there were duplicates -- now done
            newLoc = actualInsertPts[newIndIdx];
          }
        }
        else halt("Something went wrong");
      }

      for a in _arrs do
        a.sparseBulkShiftArray(arrShiftMap, oldnnz);

      return actualAddCnt;
    }

    override proc dsiMyDist() : unmanaged BaseDist {
      return dist;
    }

    override proc dsiClear() {
      _nnz = 0;
      // should we empty the domain too ?
      // nnzDom = {1..0};
    }

    iter dimIter(param d, ind) {
      if (d != rank-1) {
        compilerError("dimIter() not supported on sparse domains for dimensions other than the last");
      }
      halt("dimIter() not yet implemented for sparse domains");
      yield _indices(0);
    }

    proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
      if _to_borrowed(rhs._instance.type) == this.type &&
         canDoDirectAssignment(rhs) {

        // ENGIN: We cannot use bulkGrow here, because rhs might be grown using
        // grow, which has a different heuristic to grow the internal arrays.
        // That may result in size mismatch in the following internal array
        // assignments
        // ?fromMMS: what is this about?
        this._nnz = rhs._nnz;
        this.nnzDom = rhs.nnzDom;

        this._indices = rhs._indices;
      }
      else {
        chpl_assignDomainWithIndsIterSafeForRemoving(this, rhs);
      }
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain(loc: locale) {
      if this.locale == loc {
        return _getDomain(_to_unmanaged(this));
      } else {
        const copy = new unmanaged DefaultSparseDom(rank, idxType, dist, parentDom);
        return new _domain(copy);
      }
    }
  }


  class DefaultSparseArr: BaseSparseArrImpl {

    proc init(type eltType,
              param rank : int,
              type idxType,
              dom,
              param initElts:bool) {
      super.init(eltType, rank, idxType, dom, initElts);
    }

    // dsiDestroyArr is defined in BaseSparseArrImpl

    // ref version
    proc dsiAccess(ind: idxType) ref where rank == 1 {
      // make sure we're in the dense bounding box
      if boundsChecking then
        if !(dom.parentDom.contains(ind)) {
          if debugDefaultSparse {
            writeln("On locale ", here.id);
            writeln("In dsiAccess, got index ", ind);
            writeln("dom.parentDom = ", dom.parentDom);
          }

          halt("array index out of bounds: ", ind);
        }


      // lookup the index and return the data or IRV
      const (found, loc) = dom.find(ind);
      if found then
        return data(loc);
      else // ?fromMMS: is this error message correct? Not actually looking at value.
        halt("attempting to assign a 'zero' value in a sparse array: ", ind);
    }
    // value version
    proc dsiAccess(ind: idxType) const ref where rank == 1 {
      // make sure we're in the dense bounding box
      if boundsChecking then
        if !(dom.parentDom.contains(ind)) then
          halt("array index out of bounds: ", ind);

      // lookup the index and return the data or IRV
      const (found, loc) = dom.find(ind);
      if found then
        return data(loc);
      else
        return irv;
    }


    // ref version
    proc dsiAccess(ind: rank*idxType) ref {
      // make sure we're in the dense bounding box
      if boundsChecking then
        if !(dom.parentDom.contains(ind)) then
          halt("array index out of bounds: ", ind);

      // lookup the index and return the data or IRV
      const (found, loc) = dom.find(ind);
      if found then
        return data(loc);
      else
        halt("attempting to assign a 'zero' value in a sparse array: ", ind);
    }
    // value version for POD types
    proc dsiAccess(ind: rank*idxType)
    where shouldReturnRvalueByValue(eltType) {
      // make sure we're in the dense bounding box
      if boundsChecking then
        if !(dom.parentDom.contains(ind)) then
          halt("array index out of bounds: ", ind);

      // lookup the index and return the data or IRV
      const (found, loc) = dom.find(ind);
      if found then
        return data(loc);
      else
        return irv;
    }
    // const ref version for types with copy ctors
    proc dsiAccess(ind: rank*idxType) const ref
    where shouldReturnRvalueByConstRef(eltType) {
      // make sure we're in the dense bounding box
      if boundsChecking then
        if !(dom.parentDom.contains(ind)) then
          halt("array index out of bounds: ", ind);

      // lookup the index and return the data or IRV
      const (found, loc) = dom.find(ind);
      if found then
        return data(loc);
      else
        return irv;
    }

    pragma "order independent yielding loops"
    iter these() ref {
      for i in 0..#dom._nnz do yield data[i];
    }

    pragma "order independent yielding loops"
    iter these(param tag: iterKind) ref where tag == iterKind.standalone {
      const numElems = dom._nnz;
      const numChunks = _computeNumChunks(numElems): numElems.type;
      if debugDefaultSparse {
        writeln("DefaultSparseArr standalone: ", numChunks, " chunks, ",
                numElems, " elems");
      }
      if numChunks <= 1 {
        for i in 0..#numElems {
          yield data[i];
        }
      } else {
        coforall chunk in RangeChunk.chunks(0..#numElems, numChunks) {
          for i in chunk do
            yield data[i];
        }
      }
    }


    iter these(param tag: iterKind) where tag == iterKind.leader {
      // forward to the leader iterator on our domain
      for followThis in dom.these(tag) do
        yield followThis;
    }

    // same as DefaultSparseDom's follower, except here we index into 'data'
    pragma "order independent yielding loops"
    iter these(param tag: iterKind, followThis:(?,?,?)) ref where tag == iterKind.follower {
      var (followThisDom, startIx, endIx) = followThis;

      if (followThisDom != this.dom) then
        halt("Sparse arrays can't be zippered with anything other than their domains and sibling arrays");
      if debugDefaultSparse then
        writeln("DefaultSparseArr follower: ", startIx, "..", endIx);

      for i in startIx..endIx do yield data[i];
    }

    iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
      compilerError("Sparse iterators can't yet be zippered with others");
      yield 0;  // dummy
    }

    proc dsiTargetLocales() {
      compilerError("targetLocales is unsupported by sparse domains");
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain(loc: locale) {
      if this.locale == loc {
        return _getDomain(dom);
      } else {
        return dom.dsiLocalSubdomain(loc);
      }
    }
  }


  proc DefaultSparseDom.dsiSerialWrite(f, printBrackets=true) throws {
    if (rank == 1) {
      if printBrackets then f <~> "{";
      if (_nnz >= 1) {
        f <~> _indices(0);
        for i in 1.._nnz-1 {
          f <~> " " <~> _indices(i);
        }
      }
      if printBrackets then f <~> "}";
    } else {
      if printBrackets then f <~> "{\n";
      if (_nnz >= 1) {
        var prevInd = _indices(0);
        f <~> " " <~> prevInd;
        for i in 1.._nnz-1 {
          if (prevInd(0) != _indices(i)(0)) {
            f <~> "\n";
          }
          prevInd = _indices(i);
          f <~> " " <~> prevInd;
        }
        f <~> "\n";
      }
      if printBrackets then f <~> "}\n";
    }
  }


  proc DefaultSparseArr.dsiSerialWrite(f) throws {
    if (rank == 1) {
      if (dom._nnz >= 1) {
        f <~> data(0);
        for i in 1..dom._nnz-1 {
          f <~> " " <~> data(i);
        }
      }
    } else {
      if (dom._nnz >= 1) {
        var prevInd = dom._indices(0);
        f <~> data(0);
        for i in 1..dom._nnz-1 {
          if (prevInd(0) != dom._indices(i)(0)) {
            f <~> "\n";
          } else {
            f <~> " ";
          }
          prevInd = dom._indices(i);
          f <~> data(i);
        }
        f <~> "\n";
      }
    }
  }

}
