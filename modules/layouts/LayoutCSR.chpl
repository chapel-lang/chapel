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

config param debugCSR = false;
use RangeChunk only ;

// I have not seen us test a non-"sub" CSR domain
// and I do not want untested code in the docs.
// TODO: change to 'sparse domain' and add that code to the test suite.
/*
This CSR layout provides a Compressed Sparse Row implementation
for Chapel's sparse domains and arrays.

To declare a CSR domain, invoke the ``CSR`` constructor without arguments
in a `dmapped` clause. For example:

  .. code-block:: chapel

    use LayoutCSR;
    var D = {1..n, 1..m};  // a default-distributed domain
    var CSR_Domain: sparse subdomain(D) dmapped CSR();

To declare a CSR array, use a CSR domain, for example:

  .. code-block:: chapel

    // assume the above declarations
    var CSR_Array: [CSR_Domain] real;

This domain map is a layout, i.e. it maps all indices to the current locale.
All elements of a CSR-distributed array are stored
on the locale where the array variable is declared.
*/
class CSR: BaseDist {
  proc dsiNewSparseDom(param rank: int, type idxType, dom: domain) {
    return new CSRDom(rank=rank, idxType=idxType, dist=this, parentDom=dom);
  }

  proc dsiClone() return new CSR();

  proc dsiEqualDMaps(that: CSR) param {
    return true;
  }

  proc dsiEqualDMaps(that) param {
    return false;
  }
}

class CSRDom: BaseSparseDomImpl {
  var dist: CSR;

  var rowRange: range(idxType);
  var colRange: range(idxType);

  const rowDom: domain(1, idxType);

  var rowStart: [rowDom] idxType;      // would like index(nnzDom)
  var colIdx: [nnzDom] idxType;        // would like index(parentDom.dim(1))

  proc CSRDom(param rank, type idxType, 
                               dist: CSR,
                               parentDom: domain(rank, idxType)) {
    if (rank != 2) then
      compilerError("Only 2D sparse domains are supported by the CSR distribution");
    this.dist = dist;
    this.parentDom = parentDom;
    rowRange = parentDom.dim(1);
    colRange = parentDom.dim(2);
    rowDom = {rowRange.low..rowRange.high+1};
    nnzDom = {1..nnz};
    dsiClear();
  }

  proc dsiMyDist() return dist;

  proc dsiGetIndices() return 0;
  proc dsiSetIndices(x) { }

  proc dsiBuildArray(type eltType)
    return new CSRArr(eltType=eltType, rank=rank, idxType=idxType, dom=this);

  iter dsiIndsIterSafeForRemoving() {
    var cursorRow = rowRange.high;
    for i in 1..nnz by -1 {
      while (rowStart(cursorRow) > i) {
        cursorRow -= 1;
      }
      yield (cursorRow, colIdx(i));
    }
  }

  iter these() {
    //writeln("serial- rowRange=", rowRange, " colRange=", colRange, "\n",
    //        "        rowStart=", rowStart, " colIdx=", colIdx);

    // faster to start at _private_findStartRow(1) ?
    var cursorRow = rowRange.low;
    for i in 1..nnz {
      while (rowStart(cursorRow+1) <= i) {
        cursorRow += 1;
      }
      yield (cursorRow, colIdx(i));
    }
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    // same as DefaultSparseDom's leader
    const numElems = nnz;
    const numChunks = _computeNumChunks(numElems);
    //writeln("leader- rowRange=", rowRange, " colRange=", colRange, "\n",
    //        "        rowStart=", rowStart, " colIdx=", colIdx);
    if debugCSR then
      writeln("CSRDom leader: ", numChunks, " chunks, ", numElems, " elems");

    // split our numElems elements over numChunks tasks
    if numChunks == 1 then
      yield (this, 1, numElems);
    else
      coforall chunk in chunks(1..numElems, numChunks) do
        yield (this, chunk.first, chunk.last);
    // TODO: to handle large numElems and numChunks faster, it would be great
    // to run the binary search in _private_findStartRow smarter, e.g.
    // pass to the tasks created in 'coforall' smaller ranges to search over.
  }

  iter these(param tag: iterKind, followThis: (?,?,?)) where tag == iterKind.follower {
    var (followThisDom, startIx, endIx) = followThis;
    if boundsChecking then
      assert(startIx <= endIx, "CSRDom follower - got nothing to iterate over");

    if (followThisDom != this) then
      halt("Sparse domains can't be zippered with anything other than themselves and their arrays (CSR layout)");

    // This loop is identical to the serial iterator, except for the iteration
    // space and finding the initial 'cursorRow'.
    var cursorRow = _private_findStartRow(startIx);
    if debugCSR then
      writeln("CSRDom follower: ", startIx, "..", endIx,
              "  rowStart(", cursorRow, ")=", rowStart(cursorRow));

    for i in startIx..endIx {
      while (rowStart(cursorRow+1) <= i) do cursorRow += 1;
      yield (cursorRow, colIdx(i));
    }
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    compilerError("Sparse iterators can't yet be zippered with others (CSR layout)");
    yield 0;    // Dummy.
  }

  // Helper: find 'ix' s.t. rowStart(ix) <= startIx < rowStart(ix+1)
  // or a number at most 'approx' smaller than that.
  // There MUST exist a solution within low..high.
  proc _private_findStartRow(startIx) {
    return _private_findStartRow(startIx, rowDom.low, rowDom.high);
  }
  proc _private_findStartRow(startIx, low, high) {
    var approx = 2; // Indicates when to switch to linear search.
                    // This number could be tuned for performance.
    // simple binary search (should be fewer comparisons than binarySearch())
    var l = low, h = high;
    while h > l + approx {
      var m = (h + l) / 2;
      if rowStart(m) <= startIx then l = m; else h = m;
    }
    var hh = min(l+approx, high);

    //writeln("findStartRow: startIx=", startIx, " l=", l, "(", low,
    //        ") h=", h, "(", high, ")\n",
    //        "              rowStart(", l, ")=", rowStart(l),
    //        " rowStart(", hh, ")=", rowStart(hh));

    if boundsChecking then {
      assert(rowStart(l) <= startIx, "CSRDom.findStartRow-1");
      assert(startIx < rowStart(hh), "CSRDom.findStartRow-2");
      // The second assertion will fail if there is NO solution
      // within low..high.  For performance,
      // ensuring existence of a solution is caller responsibility.
    }
    return l;
  }

  proc rowStop(row) {
    return rowStart(row+1)-1;
  }

  proc find(ind: rank*idxType) {
    use Search;

    const (row, col) = ind;

    return binarySearch(colIdx, col, lo=rowStart(row), hi=rowStop(row));
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
    for i in rowDom do
      if rowStart[i] > _low then
        return (i-1, colIdx[colIdx.domain.low]);

    halt("Something went wrong in dsiFirst");
    return (0, 0);
  }

  proc dsiLast {
    if nnz == 0 then return (parentDom.low) - (1,1);
    const _low = nnzDom.low;
    var _lastRow = parentDom.low[1] - 1;
    for i in rowDom do
      if rowStart[i] > _low then
        _lastRow = i;

    return (_lastRow-1, colIdx[nnz]);
  }

  proc dsiAdd(ind: rank*idxType) {
    boundsCheck(ind);

    // find position in nnzDom to insert new index
    const (found, insertPt) = find(ind);

    // if the index already existed, then return
    if (found) then return 0;

    // increment number of nonzeroes
    nnz += 1;

    // double nnzDom if we've outgrown it; grab current size otherwise
    var oldNNZDomSize = nnzDom.size;
    _grow(nnz);

    const (row,col) = ind;

    // shift column indices up
    for i in insertPt..nnz-1 by -1 {
      colIdx(i+1) = colIdx(i);
    }

    colIdx(insertPt) = col;

    // bump the rowStart counts
    for r in row+1..rowDom.high {  // want rowDom[row+1..]
      rowStart(r) += 1;
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
      isUnique=false){

    const (actualInsertPts, actualAddCnt) =
      __getActualInsertPts(this, inds, dataSorted, isUnique);

    const oldnnz = nnz;
    nnz += actualAddCnt;

    //grow nnzDom if necessary
    _bulkGrow();

    //linearly fill the new colIdx from backwards
    var newIndIdx = indsDom.high; //index into new indices
    var oldIndIdx = oldnnz; //index into old indices
    var newLoc = actualInsertPts[newIndIdx]; //its position-to-be in new dom
    while newLoc == -1 {
      newIndIdx -= 1;
      if newIndIdx == indsDom.low-1 then break; //there were duplicates -- now done
      newLoc = actualInsertPts[newIndIdx];
    }

    var arrShiftMap: [{1..oldnnz}] int; //to map where data goes

    for i in 1..nnz by -1 {
      if oldIndIdx >= 1 && i > newLoc {
        //shift from old values
        colIdx[i] = colIdx[oldIndIdx];
        arrShiftMap[oldIndIdx] = i;
        oldIndIdx -= 1;
      }
      else if newIndIdx >= indsDom.low && i == newLoc {
        //put the new guy in
        colIdx[i] = inds[newIndIdx][2];
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

    //aggregated row shift
    var prevRow = parentDom.dim(1).low;
    var row: int;
    var rowCnt = 0;
    for (ind, p) in zip(inds, actualInsertPts)  {
      if p == -1 then continue;
      row = ind[1];
      if row == prevRow then rowCnt += 1;
      else {
        /*writeln(rowCnt, " nnz in row ", prevRow);*/
        rowStart[prevRow+1] += rowCnt;
        if row - prevRow > 1 {
          for i in prevRow+2..row{
            rowStart[i] += rowCnt;
          }
        }
        rowCnt += 1;
        prevRow = row;
      }
    }
    for i in prevRow+1..rowDom.high{
        rowStart[i] += rowCnt;
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

    // TODO: should halve nnzDom if we've outgrown it...
    /*
    var oldNNZDomSize = nnzDomSize;
    if (nnz > nnzDomSize) {
      nnzDomSize = if (nnzDomSize) then 2*nnzDomSize else 1;

      nnzDom = {1..nnzDomSize};
    }
    */

    const (row,col) = ind;

    // shift column indices down
    for i in insertPt..nnz {
      colIdx(i) = colIdx(i+1);
    }

    // bump the rowStart counts
    for r in row+1..rowDom.high {  // want rowDom[row+1..]
      rowStart(r) -= 1;
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
    rowStart = 1;
  }

  iter dimIter(param d, ind) {
    if (d != 2) {
      compilerError("dimIter(1, ...) not supported on CSR domains");
    }
    for i in rowStart[ind]..rowStop[ind] do
      yield colIdx[i];
  }
}


class CSRArr: BaseSparseArrImpl {

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
    // simpler than CSRDom's follower - no need to deal with rows (or columns)
    var (followThisDom, startIx, endIx) = followThis;

    if (followThisDom != this.dom) then
      halt("Sparse arrays can't be zippered with anything other than their domains and sibling arrays (CSR layout)");
    if debugCSR then
      writeln("CSRArr follower: ", startIx, "..", endIx);

    for i in startIx..endIx do yield data[i];
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    compilerError("Sparse iterators can't yet be zippered with others (CSR layout)");
    yield 0;    // Dummy.
  }
}


proc CSRDom.dsiSerialWrite(f) {
  f.writeln("{");
  for r in rowRange {
    const lo = rowStart(r);
    const hi = rowStop(r);
    for c in lo..hi {
      f.write(" (", r, ", ", colIdx(c), ")", if (c==hi) then "\n" else "");
    }
  }
  f.writeln("}");
}


proc CSRArr.dsiSerialWrite(f) {
  for r in dom.rowRange {
    const lo = dom.rowStart(r);
    const hi = dom.rowStop(r);
    for c in lo..hi {
      f.write(data(c), if (c==hi) then "\n" else " ");
    }
  }
}
