/*
 * Copyright 2004-2016 Cray Inc.
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
module DefaultSparse {

  use Search;
  config param debugDefaultSparse = false;

  class DefaultSparseDom: BaseSparseDom {
    param rank : int;
    type idxType;
    var parentDom;
    var dist: DefaultDist;
    var nnz = 0;  // intention is that user might specify this to avoid reallocs

    var nnzDomSize = nnz;
    var nnzDom = {1..nnzDomSize};

    var indices: [nnzDom] index(rank, idxType);

    proc linksDistribution() param return false;
    proc dsiLinksDistribution()     return false;

    proc DefaultSparseDom(param rank, type idxType,
                                 dist: DefaultDist,
                                 parentDom: domain) {
      this.parentDom = parentDom;
      this.dist = dist;
      nnz = 0;
    }

    proc dsiNumIndices return nnz;

    proc dsiBuildArray(type eltType)
      return new DefaultSparseArr(eltType=eltType, rank=rank, idxType=idxType,
                                  dom=this);

    iter dsiIndsIterSafeForRemoving() {
      for i in 1..nnz by -1 {
        yield indices(i);
      }
    }

    iter these() {
      for i in 1..nnz {
        yield indices(i);
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone {
      const numElems = nnz;
      const numChunks = _computeNumChunks(numElems): numElems.type;
      if debugDefaultSparse {
        writeln("DefaultSparseDom standalone: ", numChunks, " chunks, ",
                numElems, " elems");
      }

      // split our numElems elements over numChunks tasks
      if numChunks <= 1 {
        // ... except if 1, just use the current thread
        for i in 1..numElems {
          yield indices(i);
        }
      } else {
        coforall chunk in 1..numChunks {
          const (startIx, endIx) =
            _computeChunkStartEnd(numElems, numChunks, chunk);
          for i in startIx..endIx {
            yield indices(i);
          }
        }
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      const numElems = nnz;
      const numChunks = _computeNumChunks(numElems): numElems.type;
      if debugDefaultSparse then
        writeln("DefaultSparseDom leader: ", numChunks, " chunks, ",
                numElems, " elems");

      // split our numElems elements over numChunks tasks
      if numChunks <= 1 then
        // ... except if 1, just use the current thread
        yield (this, 1, numElems);
      else
        coforall chunk in 1..numChunks do
          yield (this, (..._computeChunkStartEnd(numElems, numChunks, chunk)));
    }

    iter these(param tag: iterKind, followThis:(?,?,?)) where tag == iterKind.follower {
      var (followThisDom, startIx, endIx) = followThis;

      if (followThisDom != this) then
        halt("Sparse domains can't be zippered with anything other than themselves and their arrays");
      if debugDefaultSparse then
        writeln("DefaultSparseDom follower: ", startIx, "..", endIx);

      for i in startIx..endIx do
        yield indices(i);
    }

    iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
      compilerError("Sparse iterators can't yet be zippered with others");
      var dummy: rank * idxType;
      yield dummy;
    }

    proc dsiDim(d : int) {
      return parentDom.dim(d);
    }

    proc dsiDims() {
      return parentDom.dims();
    }

    // private
    proc find(ind) {
      //
      // sjd: unfortunate specialization for rank == 1
      //
      if rank == 1 && isTuple(ind) && ind.size == 1 then
        return BinarySearch(indices, ind(1), 1, nnz);
      else
        return BinarySearch(indices, ind, 1, nnz);
    }

    proc dsiMember(ind) { // ind should be verified to be index type
      const (found, loc) = find(ind);
      return found;
    }

    proc boundsCheck(ind: index(rank, idxType)):void {
      if boundsChecking then
        if !(parentDom.member(ind)) then
          halt("DefaultSparse domain/array index out of bounds: ", ind,
              " (expected to be within ", parentDom, ")");
    }

    proc add_help(ind) {
      // find position in nnzDom to insert new index
      const (found, insertPt) = find(ind);

      // if the index already existed, then return
      if (found) then return;

      // increment number of nonzeroes
      nnz += 1;

      // double nnzDom if we've outgrown it; grab current size otherwise
      var oldNNZDomSize = nnzDomSize;
      if (nnz > nnzDomSize) {
        nnzDomSize = if (nnzDomSize) then 2*nnzDomSize else 1;
        nnzDom = {1..nnzDomSize};
      }
      // shift indices up
      for i in insertPt..nnz-1 by -1 {
        indices(i+1) = indices(i);
      }

      indices(insertPt) = ind;

      // shift all of the arrays up and initialize nonzeroes if
      // necessary
      //
      // BLC: Note: if rectangular arrays had a user-settable
      // initialization value, we could set it to be the IRV and skip
      // this second initialization of any new values in the array.
      // we could also eliminate the oldNNZDomSize variable
      for a in _arrs {
        a.sparseShiftArray(insertPt..nnz-1, oldNNZDomSize+1..nnzDomSize);
      }
    }

    proc rem_help(ind) {
      // find position in nnzDom to insert new index
      const (found, insertPt) = find(ind);

      // if the index does not exist, then halt
      if (!found) then
        halt("index not in domain: ", ind);

      // increment number of nonzeroes
      nnz -= 1;

      // TODO: should halve nnzDom if we've outgrown it; grab current
      // size otherwise
      /*
      var oldNNZDomSize = nnzDomSize;
      if (nnz > nnzDomSize) {
        nnzDomSize = if (nnzDomSize) then 2*nnzDomSize else 1;
        nnzDom = [1..nnzDomSize];
      }
      */
      // shift indices up
      for i in insertPt..nnz {
        indices(i) = indices(i+1);
      }

      // shift all of the arrays up and initialize nonzeroes if
      // necessary
      //
      // BLC: Note: if rectangular arrays had a user-settable
      // initialization value, we could set it to be the IRV and skip
      // this second initialization of any new values in the array.
      // we could also eliminate the oldNNZDomSize variable
      for a in _arrs {
        a.sparseShiftArrayBack(insertPt..nnz-1);
      }
    }

    proc dsiAdd(ind: idxType) where rank == 1 {
      add_help(ind);
    }

    proc dsiRemove(ind: idxType) where rank == 1 {
      rem_help(ind);
    }

    proc dsiAdd(ind: rank*idxType) {
      if (rank == 1) {
        add_help(ind(1));
      } else {
        add_help(ind);
      }
    }

    proc bulkAdd(inds: [] index(rank, idxType), sorted, noDuplicate,
        actualInsertPts, actualAddCnt){

      const oldnnz = nnz;
      nnz += actualAddCnt;

      //grow nnzDom if necessary
      if (nnz > nnzDomSize) {
        nnzDomSize = (exp2(log2(nnz)+1.0)):int;

        nnzDom = {1..nnzDomSize};
      }

      //linearly fill the new colIdx from backwards
      var newIndIdx = actualInsertPts.size-1; //index into new indices
      var oldIndIdx = oldnnz; //index into old indices
      var newLoc = actualInsertPts[newIndIdx]; //its position-to-be in new dom
      while newLoc == -1 {
        newIndIdx -= 1;
        newLoc = actualInsertPts[newIndIdx];
      }

      var arrShiftMap: [{1..oldnnz}] int; //to map where data goes

      for i in 1..nnz by -1 {
        if oldIndIdx >= 1 && i > newLoc {
          //shift from old values
          indices[i] = indices[oldIndIdx];
          arrShiftMap[oldIndIdx] = i;
          oldIndIdx -= 1;
        }
        else if newIndIdx >= 0 && i == newLoc {
          //put the new guy in
          indices[i] = inds[newIndIdx];
          newIndIdx -= 1;
          if newIndIdx >= 0 then 
            newLoc = actualInsertPts[newIndIdx];
          else
            newLoc = -2; //finished new set
          while newLoc == -1 {
            newIndIdx -= 1;
            if newIndIdx == -1 then break; //there were duplicates -- now done
            newLoc = actualInsertPts[newIndIdx];
          }
        }
        else halt("Something went wrong");
      }

      for a in _arrs do 
        a.sparseBulkShiftArray(arrShiftMap, oldnnz);
    }

    proc dsiRemove(ind: rank*idxType) {
      if (rank == 1) {
        rem_help(ind(1));
      } else {
        rem_help(ind);
      }
    }

    proc dsiClear() {
      nnz = 0;
    }

    iter dimIter(param d, ind) {
      if (d != rank-1) {
        compilerError("dimIter() not supported on sparse domains for dimensions other than the last");
      }
      halt("dimIter() not yet implemented for sparse domains");
      yield indices(1);
    }
  }


  class DefaultSparseArr: BaseArr {
    type eltType;
    param rank : int;
    type idxType;

    var dom; /* : DefaultSparseDom(?); */
    var data: [dom.nnzDom] eltType;
    var irv: eltType;

    proc dsiGetBaseDom() return dom;

    // ref version
    proc dsiAccess(ind: idxType) ref where rank == 1 {
      // make sure we're in the dense bounding box
      if boundsChecking then
        if !(dom.parentDom.member(ind)) {
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
      else
        halt("attempting to assign a 'zero' value in a sparse array: ", ind);
    }
    // value version
    proc dsiAccess(ind: idxType) const ref where rank == 1 {
      // make sure we're in the dense bounding box
      if boundsChecking then
        if !(dom.parentDom.member(ind)) then
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
        if !(dom.parentDom.member(ind)) then
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
    where !shouldReturnRvalueByConstRef(eltType) {
      // make sure we're in the dense bounding box
      if boundsChecking then
        if !(dom.parentDom.member(ind)) then
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
        if !(dom.parentDom.member(ind)) then
          halt("array index out of bounds: ", ind);

      // lookup the index and return the data or IRV
      const (found, loc) = dom.find(ind);
      if found then
        return data(loc);
      else
        return irv;
    }

    iter these() ref {
      for i in 1..dom.nnz do yield data[i];
    }

    iter these(param tag: iterKind) ref where tag == iterKind.standalone {
      const numElems = dom.nnz;
      const numChunks = _computeNumChunks(numElems): numElems.type;
      if debugDefaultSparse {
        writeln("DefaultSparseArr standalone: ", numChunks, " chunks, ",
                numElems, " elems");
      }
      if numChunks <= 1 {
        for i in 1..numElems {
          yield data[i];
        }
      } else {
        coforall chunk in 1..numChunks {
          const (startIx, endIx) =
            _computeChunkStartEnd(numElems, numChunks, chunk);
          for i in startIx..endIx {
            yield data[i];
          }
        }
      }
    }


    iter these(param tag: iterKind) where tag == iterKind.leader {
      // forward to the leader iterator on our domain
      for followThis in dom.these(tag) do
        yield followThis;
    }

    // same as DefaultSparseDom's follower, except here we index into 'data'
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

    proc IRV ref {
      return irv;
    }

    // shifts data array according to shiftMap where shiftMap[i] is the new index 
    // of the ith element of the array. Called at the end of bulkAdd to move the
    // existing items in data array and initialize new indices with irv.
    // oldnnz is the number of elements in the array. As the function is called 
    // at the end of bulkAdd, it is almost certain that oldnnz!=data.size
    proc sparseBulkShiftArray(shiftMap, oldnnz){
      var newIdx: int;
      var prevNewIdx = 1;
      for (i, _newIdx) in zip(1..oldnnz by -1, shiftMap.domain.dim(1) by -1) {
        newIdx = shiftMap[_newIdx];
        data[newIdx] = data[i];
        
        //fill IRV up to previously added nnz
        for emptyIndex in newIdx+1..prevNewIdx-1 do data[emptyIndex] = irv;
        prevNewIdx = newIdx;
      }
      //fill the initial added space with IRV
      for i in 1..prevNewIdx-1 do data[i] = irv;
    }

    // shift data array after single index addition. Fills the new index with irv
    proc sparseShiftArray(shiftrange, initrange) {
      for i in initrange {
        data(i) = irv;
      }
      for i in shiftrange by -1 {
        data(i+1) = data(i);
      }
      data(shiftrange.low) = irv;
    }

    proc sparseShiftArrayBack(shiftrange) {
      for i in shiftrange {
        data(i) = data(i+1);
      }
    }

    proc dsiTargetLocales() {
      compilerError("targetLocales is unsupported by sparse domains");
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain() {
      return _newDomain(dom);
    }
  }


  proc DefaultSparseDom.dsiSerialWrite(f, printBrackets=true) {
    if (rank == 1) {
      if printBrackets then f.write("{");
      if (nnz >= 1) {
        f.write(indices(1));
        for i in 2..nnz {
          f.write(" ", indices(i));
        }
      }
      if printBrackets then f.write("}");
    } else {
      if printBrackets then f.writeln("{");
      if (nnz >= 1) {
        var prevInd = indices(1);
        f.write(" ", prevInd);
        for i in 2..nnz {
          if (prevInd(1) != indices(i)(1)) {
            f.writeln();
          }
          prevInd = indices(i);
          f.write(" ", prevInd);
        }
        f.writeln();
      }
      if printBrackets then f.writeln("}");
    }
  }


  proc DefaultSparseArr.dsiSerialWrite(f) {
    if (rank == 1) {
      if (dom.nnz >= 1) {
        f.write(data(1));
        for i in 2..dom.nnz {
          f.write(" ", data(i));
        }
      }
    } else {
      if (dom.nnz >= 1) {
        var prevInd = dom.indices(1);
        f.write(data(1));
        for i in 2..dom.nnz {
          if (prevInd(1) != dom.indices(i)(1)) {
            f.writeln();
          } else {
            f.write(" ");
          }
          prevInd = dom.indices(i);
          f.write(data(i));
        }
        f.writeln();
      }
    }
  }

}
