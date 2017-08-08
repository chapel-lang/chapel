/*
 * Copyright 2004-2017 Cray Inc.
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

/* Debug flag */
config param debugCS = false;
use RangeChunk only ;

/* For backwards-compatibility with LayoutCSR */
type CSR = CS;

pragma "no doc"
/* Comparator used for sorting by columns */
record _ColumnComparator {
  proc init() { }
  proc key(idx: _tuple) { return (idx(2), idx(1));}
}

const _columnComparator: _ColumnComparator;

/*
This CS layout provides a Compressed Sparse Row and Compressed Sparse Column
implementation for Chapel's sparse domains and arrays.

To declare a CS domain, invoke the ``CS`` constructor without arguments
in a `dmapped` clause. For example:

  .. code-block:: chapel

    use LayoutCS;
    var D = {1..n, 1..m};  // a default-distributed domain
    var CS_Domain: sparse subdomain(D) dmapped CS();

To declare a CS array, use a CS domain, for example:

  .. code-block:: chapel

    // assume the above declarations
    var CS_Array: [CS_Domain] real;

This domain map is a layout, i.e. it maps all indices to the current locale.
All elements of a CS-distributed array are stored
on the locale where the array variable is declared.
*/
class CS: BaseDist {
  const row: bool = true;

  proc dsiNewSparseDom(param rank: int, type idxType, dom: domain) {
    return new CSDom(rank=rank, row=row, idxType=idxType, dist=this, parentDom=dom);
  }

  proc dsiClone() return new CS(row=this.row);

  proc dsiEqualDMaps(that: CS) param {
    return true;
  }

  proc dsiEqualDMaps(that) param {
    return false;
  }
} // CS


class CSDom: BaseSparseDomImpl {
  const row: bool;                  // would like param

  var dist: CS;

  var rowRange: range(idxType);
  var colRange: range(idxType);

  /* (row|col) dom */
  const dom: domain(1, idxType);

  /* (row|col) start */
  var startIdx: [dom] idxType;      // would like index(nnzDom)
  /* (row|col) idx */
  var idx: [nnzDom] idxType;        // would like index(parentDom.dim(1))

  /* Initializer */
  proc CSDom(param rank, type idxType, const row=true,
             dist: CS, parentDom: domain(rank, idxType)) {
    if (rank != 2) then
      compilerError("Only 2D sparse domains are supported by the CS distribution");
    this.row = row;
    this.dist = dist;
    this.parentDom = parentDom;
    rowRange = parentDom.dim(1);
    colRange = parentDom.dim(2);
    dom = if row then {rowRange.low..rowRange.high+1} else {colRange.low..colRange.high+1};
    nnzDom = {1..nnz};
    dsiClear();
  }

  proc dsiMyDist() return dist;

  // TODO: Open question -- How should slicing on CS domains and arrays work?
  proc dsiGetIndices() { compilerError('Slicing is not supported on CS domains'); }

  proc dsiSetIndices(x) { compilerError('Slicing is not supported on CS domains'); }

  proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
    chpl_assignDomainWithIndsIterSafeForRemoving(this, rhs);
  }

  proc dsiBuildArray(type eltType)
    return new CSArr(eltType=eltType, rank=rank, idxType=idxType, dom=this);

  iter dsiIndsIterSafeForRemoving() {
    var cursor = if this.row then rowRange.high else colRange.high;
    for i in 1..nnz by -1 {
      while (startIdx(cursor) > i) {
        cursor -= 1;
      }
      if this.row {
        yield (cursor, idx(i));
      } else {
        yield (idx(i), cursor);
      }
    }
  }

  iter these() {
    // TODO: Is it faster to start at _private_findStart(1) ?
    var cursor = if this.row then rowRange.low else colRange.low;
    for i in 1..nnz {
      while (startIdx(cursor+1) <= i) {
        cursor+= 1;
      }
      if this.row then
        yield (cursor, idx(i));
      else
        yield (idx(i), cursor);

    }
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    // same as DefaultSparseDom's leader
    const numElems = nnz;
    const numChunks = _computeNumChunks(numElems);
    if debugCS then
      writeln("CSDom leader: ", numChunks, " chunks, ", numElems, " elems");

    // split our numElems elements over numChunks tasks
    if numChunks == 1 then
      yield (this, 1, numElems);
    else
      coforall chunk in chunks(1..numElems, numChunks) do
        yield (this, chunk.first, chunk.last);
    // TODO: to handle large numElems and numChunks faster, it would be great
    // to run the binary search in _private_findStart smarter, e.g.
    // pass to the tasks created in 'coforall' smaller ranges to search over.
  }

  iter these(param tag: iterKind, followThis: (?,?,?)) where tag == iterKind.follower {
    var (followThisDom, startIx, endIx) = followThis;
    if boundsChecking then
      assert(startIx <= endIx, "CSDom follower - got nothing to iterate over");

    // TODO: Allow zippering sparse domains of equal length some day...
    if (followThisDom != this) then
      halt("Sparse domains can't be zippered with anything other than themselves and their arrays (CS layout)");

    // This loop is identical to the serial iterator, except for the iteration
    // space and finding the initial 'cursorRow'.
    var cursor = _private_findStart(startIx);
    if debugCS then
      writeln("CSDom follower: ", startIx, "..", endIx,
              "  startIdx(", cursor, ")=", startIdx(cursor));

    for i in startIx..endIx {
      while (startIdx(cursor+1) <= i) do cursor+= 1;
      if this.row then
        yield (cursor, idx(i));
      else
        yield (idx(i), cursor);
    }
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    compilerError("Sparse iterators can't yet be zippered with others (CS layout)");
    yield 0;    // Dummy.
  }

  // Helper: find 'ix' s.t. startIdx(ix) <= startIx < startIdx(ix+1)
  // or a number at most 'approx' smaller than that.
  // There MUST exist a solution within low..high.
  proc _private_findStart(startIx) {
    return _private_findStart(startIx, dom.low, dom.high);
  }
  proc _private_findStart(startIx, low, high) {
    var approx = 2; // Indicates when to switch to linear search.
                    // This number could be tuned for performance.
    // simple binary search (should be fewer comparisons than binarySearch())
    var l = low, h = high;
    while h > l + approx {
      var m = (h + l) / 2;
      if startIdx(m) <= startIx then l = m; else h = m;
    }
    var hh = min(l+approx, high);

    if boundsChecking then {
      assert(startIdx(l) <= startIx, "CSDom.findStart-1");
      assert(startIx < startIdx(hh), "CSDom.findStart-2");
      // The second assertion will fail if there is NO solution
      // within low..high.  For performance,
      // ensuring existence of a solution is caller responsibility.
    }
    return l;
  }

  proc stopIdx(i) {
    return startIdx(i+1)-1;
  }

  proc find(ind: rank*idxType) {
    use Search;

    const (row, col) = ind;

    var ret: (bool, idxType);
    if this.row then
      ret = binarySearch(idx, col, lo=startIdx(row), hi=stopIdx(row));
    else
      ret = binarySearch(idx, row, lo=startIdx(col), hi=stopIdx(col));

    return ret;
  }

  proc dsiMember(ind: rank*idxType) {
    if parentDom.member(ind) {
      const (found, loc) = find(ind);
      return found;
    }
    return false;
  }

  proc dsiFirst {
    if nnz == 0 then return (parentDom.low) - (1,1);
    const _low = nnzDom.low;
    for i in dom do
      if startIdx[i] > _low then
        return (i-1, idx[idx.domain.low]);

    halt("Something went wrong in dsiFirst");
    return (0, 0);
  }

  proc dsiLast {
    if nnz == 0 then return (parentDom.low) - (1,1);
    const _low = nnzDom.low;
    var _lastRow = parentDom.low[1] - 1;
    for i in dom do
      if startIdx[i] > _low then
        _lastRow = i;

    return (_lastRow-1, idx[nnz]);
  }

  proc dsiAdd(ind: rank*idxType) {
    boundsCheck(ind);

    // find position in nnzDom to insert new index
    const (found, insertPt) = find(ind);

    // if the index already existed, then return
    if found then return 0;

    // increment number of nonzeroes
    nnz += 1;

    // double nnzDom if we've outgrown it; grab current size otherwise
    var oldNNZDomSize = nnzDom.size;
    _grow(nnz);

    const (row,col) = ind;

    // shift column indices up
    for i in insertPt..nnz-1 by -1 {
      idx(i+1) = idx(i);
    }

    if this.row then
      idx(insertPt) = col;
    else
      idx(insertPt) = row;

    // bump the startIdx counts
    var start = if this.row then row else col;

    for rc in start+1..dom.high {  // want dom[row+1..]
      startIdx(rc) += 1;
    }

    // shift all of the arrays up and initialize nonzeroes if
    // necessary
    //
    // BLC: Note: if rectangular arrays had a user-settable
    // initialization value, we could set it to be the IRV and skip
    // this second initialization of any new values in the array.
    // we could also eliminate the oldNNZDomSize variable
    for a in _arrs {
      a.sparseShiftArray(insertPt..nnz-1, oldNNZDomSize+1..nnzDom.size);
    }
    return 1;
  }

  proc bulkAdd_help(inds: [?indsDom] rank*idxType, dataSorted=false,
                    isUnique=false) {

    if this.row then
      bulkAdd_prepareInds(inds, dataSorted, isUnique, cmp=Sort.defaultComparator);
    else {
      bulkAdd_prepareInds(inds, dataSorted, isUnique, cmp=_columnComparator);
    }

    if nnz == 0 {

      const dupCount = if isUnique then 0 else _countDuplicates(inds);

      nnz += inds.size-dupCount;
      _bulkGrow();

      var idxIdx = 1;
      var prevIdx = parentDom.low - (1,1);
      var current: idxType;

      if this.row then
        current = parentDom.dim(1).low;
      else
        current = parentDom.dim(2).low;

      // Update startIdx && idx
      for (i,j) in inds {
        if !isUnique && (i,j) == prevIdx then continue;
        else prevIdx = (i,j);

        if this.row {
          while i != current {
            current += 1;
            startIdx[current+1] = startIdx[current];
          }
          startIdx[i+1] += 1;
          idx[idxIdx] = j;
          idxIdx += 1;
        } else {
          while j != current {
            current += 1;
            startIdx[current+1] = startIdx[current];
          }
          startIdx[j+1] += 1;
          idx[idxIdx] = i;
          idxIdx += 1;
        }
      }


      // make sure startIdx[i]>startIdx[j] for i>j for possibly
      // untouched part of startIdx
      const startIdxHigh = startIdx[current+1];
      for r in current+2..startIdx.domain.high {
        startIdx[r] = startIdxHigh;
      }

      return idxIdx-1;
    } // if nnz == 0

    const (actualInsertPts, actualAddCnt) =
      __getActualInsertPts(this, inds, isUnique);

    const oldnnz = nnz;
    nnz += actualAddCnt;

    // Grow nnzDom if necessary
    _bulkGrow();

    // Linearly fill the new idx from backwards
    var newIndIdx = indsDom.high; //index into new indices
    var oldIndIdx = oldnnz; //index into old indices
    var newLoc = actualInsertPts[newIndIdx]; // its position-to-be in new dom
    while newLoc == -1 {
      newIndIdx -= 1;
      if newIndIdx == indsDom.low-1 then break; // there were duplicates -- now done
      newLoc = actualInsertPts[newIndIdx];
    }

    var arrShiftMap: [{1..oldnnz}] int; //to map where data goes

    for i in 1..nnz by -1 {
      if oldIndIdx >= 1 && i > newLoc {
        // Shift from old values
        idx[i] = idx[oldIndIdx];
        arrShiftMap[oldIndIdx] = i;
        oldIndIdx -= 1;
      }
      else if newIndIdx >= indsDom.low && i == newLoc {
        // Put the new guy in
        if this.row {
          idx[i] = inds[newIndIdx][2];
        } else {
          idx[i] = inds[newIndIdx][1];
        }
        newIndIdx -= 1;
        if newIndIdx >= indsDom.low then
          newLoc = actualInsertPts[newIndIdx];
        else
          newLoc = -2; // Finished new set
        while newLoc == -1 {
          newIndIdx -= 1;
          if newIndIdx == indsDom.low-1 then break; // There were duplicates -- now done
          newLoc = actualInsertPts[newIndIdx];
        }
      }
      else halt("Something went wrong");
    }

    // Aggregated row || col shift
    var prevCursor = if this.row then parentDom.dim(1).low else parentDom.dim(2).low;
    var cursor: int;
    var cursorCnt = 0;
    for (ind, p) in zip(inds, actualInsertPts)  {
      if p == -1 then continue;
      if this.row {
        cursor = ind[1];
      } else {
        cursor = ind[2];
      }
      if cursor == prevCursor then cursorCnt += 1;
      else {
        startIdx[prevCursor+1] += cursorCnt;
        if cursor - prevCursor > 1 {
          for i in prevCursor+2..cursor {
            startIdx[i] += cursorCnt;
          }
        }
        cursorCnt += 1;
        prevCursor = cursor;
      }
    }
    for i in prevCursor+1..dom.high {
      startIdx[i] += cursorCnt;
    }
    for a in _arrs do
      a.sparseBulkShiftArray(arrShiftMap, oldnnz);

    return actualAddCnt;
  }

  proc dsiRemove(ind: rank*idxType) {
    // find position in nnzDom to remove old index
    const (found, insertPt) = find(ind);

    // if the index doesn't already exist, then return
    if (!found) then return 0;

    // increment number of nonzeroes
    nnz -= 1;

    _shrink(nnz);

    const (row,col) = ind;

    // shift column indices down
    for i in insertPt..nnz {
      idx(i) = idx(i+1);
    }

    // bump the startIdx counts
    if this.row {
      for r in row+1..dom.high {  // want dom[row+1..]
        startIdx(r) -= 1;
      }
    } else {
      for r in col+1..dom.high {  // want dom[row+1..]
        startIdx(r) -= 1;
      }
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

    return 1;
  }

  proc dsiClear() {
    nnz = 0;
    startIdx = 1;
  }

  iter dimIter(param d, ind) {
    if (d != 2) {
      compilerError("dimIter(1, ...) not supported on CS domains");
    }
    for i in startIdx[ind]..stopIdx[ind] do
      yield idx[i];
  }

  proc dsiSerialWrite(f) {
    f.writeln("{");
    if this.row {
      for r in rowRange {
        const lo = startIdx(r),
              hi = stopIdx(r);
        for c in lo..hi {
          f.write(" (", r, ", ", idx(c), ")", if (c==hi) then "\n" else "");
        }
      }
    } else {
      // Note: Prints differently than CSR
      for c in colRange {
        const lo = startIdx(c),
              hi = stopIdx(c);
        for r in lo..hi {
          f.write(" (", idx(r), ", ", c, ")", if (r==hi) then "\n" else "");
        }
      }
    }
    f.writeln("}");
  }

} // CSDom


class CSArr: BaseSparseArrImpl {

  proc dsiAccess(ind: rank*idxType) ref {
    // make sure we're in the dense bounding box
    dom.boundsCheck(ind);

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
    dom.boundsCheck(ind);

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
    dom.boundsCheck(ind);

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

  iter these(param tag: iterKind) where tag == iterKind.leader {
    // forward to the leader iterator on our domain
    // Note: this is so that arrays can be zippered with domains;
    // otherwise just chunk up data[1..dom.nnz] a-la DefaultSparseArr.
    for followThis in dom.these(tag) do
      yield followThis;
  }

  iter these(param tag: iterKind, followThis: (?,?,?)) ref where tag == iterKind.follower {
    // simpler than CSDom's follower - no need to deal with rows (or columns)
    var (followThisDom, startIx, endIx) = followThis;

    if (followThisDom != this.dom) then
      halt("Sparse arrays can't be zippered with anything other than their domains and sibling arrays (CS layout)");
    if debugCS then
      writeln("CSArr follower: ", startIx, "..", endIx);

    for i in startIx..endIx do yield data[i];
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    compilerError("Sparse iterators can't yet be zippered with others (CS layout)");
    yield 0;    // Dummy.
  }

  proc dsiSerialWrite(f) {
    if dom.row {
      for r in dom.rowRange {
        const lo = dom.startIdx(r);
        const hi = dom.stopIdx(r);
        for c in lo..hi {
          f.write(data(c), if (c==hi) then "\n" else " ");
        }
      }
    } else {
      for c in dom.colRange {
        const lo = dom.startIdx(c);
        const hi = dom.stopIdx(c);
        for r in lo..hi {
          f.write(data(r), if (r==hi) then "\n" else " ");
        }
      }
    }
  }
} // CSArr

// Currently this is not optimized for addition of a sparse
/* Overload for CS, necessary b/c domains are sorted by rows, not columns */
proc +=(ref sd: domain, d: domain)
where sd._value: CSDom && d.rank==sd.rank && sd.idxType==d.idxType {

  if d.size == 0 then return;

  type _idxType = if sd.rank==1 then int else sd.rank*int;
  const indCount = d.numIndices;
  const arr: [{0..#indCount}] _idxType;

  //this could be a parallel loop. but ranks don't match -- doesn't compile
  for (a,i) in zip(arr,d) do a=i;

  sd._value.dsiBulkAdd(arr, sd._value.row, true, false);
}
