/*
 * Copyright 2004-2015 Cray Inc.
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
      yield 0;  // dummy.
    }

    proc dsiDim(d : int) {
      return parentDom.dim(d);
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
      add_help(ind);
    }

    proc dsiRemove(ind: rank*idxType) {
      rem_help(ind);
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

    proc dsiAccess(ind: idxType) ref where rank == 1 {
      // make sure we're in the dense bounding box
      if boundsChecking then
        if !(dom.parentDom.member(ind)) then
          halt("array index out of bounds: ", ind);

      // lookup the index and return the data or IRV
      const (found, loc) = dom.find(ind);
      if setter && !found then
        halt("attempting to assign a 'zero' value in a sparse array: ", ind);
      if found then
        return data(loc);
      else
        return irv;
    }

    proc dsiAccess(ind: rank*idxType) ref {
      // make sure we're in the dense bounding box
      if boundsChecking then
        if !(dom.parentDom.member(ind)) then
          halt("array index out of bounds: ", ind);

      // lookup the index and return the data or IRV
      const (found, loc) = dom.find(ind);
      if setter && !found then
        halt("attempting to assign a 'zero' value in a sparse array: ", ind);
      if found then
        return data(loc);
      else
        return irv;
    }

    iter these() ref {
      for e in data[1..dom.nnz] do yield e;
    }

    iter these(param tag: iterKind) ref where tag == iterKind.standalone {
      const numElems = dom.nnz;
      const numChunks = _computeNumChunks(numElems): numElems.type;
      if debugDefaultSparse {
        writeln("DefaultSparseArr standalone: ", numChunks, " chunks, ",
                numElems, " elems");
      }
      if numChunks <= 1 {
        for e in data[1..numElems] {
          yield e;
        }
      } else {
        coforall chunk in 1..numChunks {
          const (startIx, endIx) =
            _computeChunkStartEnd(numElems, numChunks, chunk);
          for e in data[startIx..endIx] {
            yield e;
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

      for e in data[startIx..endIx] do yield e;
    }

    iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
      compilerError("Sparse iterators can't yet be zippered with others");
      yield 0;  // dummy
    }

    proc IRV ref {
      return irv;
    }

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
      compilerError("targetLocales is unsuppported by sparse domains");
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain() {
      return _newDomain(dom);
    }
  }


  proc DefaultSparseDom.dsiSerialWrite(f: Writer) {
    if (rank == 1) {
      f.write("{");
      if (nnz >= 1) {
        f.write(indices(1));
        for i in 2..nnz {
          f.write(" ", indices(i));
        }
      }
      f.write("}");
    } else {
      f.writeln("{");
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
      f.writeln("}");
    }
  }


  proc DefaultSparseArr.dsiSerialWrite(f: Writer) {
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
