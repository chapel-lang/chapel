config param debugCSR = false;
var _haltLockSparseCSR: sync int; // ensure halt() happens at most once

class CSR: BaseDist {
  def dsiNewSparseDom(param rank: int, type idxType, dom: domain) {
    return new CSRDom(rank=rank, idxType=idxType, dist=this, parentDom=dom);
  }

  def dsiClone() return new CSR();
}

class CSRDom: BaseSparseDom {
  param rank : int;
  type idxType;
  var dist: CSR;
  var parentDom: domain(rank, idxType);
  var nnz: idxType;  // intention is that user might specify this to avoid reallocs
  //  type idxType = rank*idxType;

  var rowRange: range(idxType);
  var colRange: range(idxType);

  const rowDom: domain(1, idxType);
  var nnzDomSize: idxType;
  var nnzDom: domain(1, nnz.type);

  var rowStart: [rowDom] idxType;      // would like index(nnzDom)
  var colIdx: [nnzDom] idxType;        // would like index(parentDom.dim(1))

  def getBaseDist() return dist;

  def CSRDom(param rank, type idxType, 
                               dist: CSR,
                               parentDom: domain(rank, idxType)) {
    if (rank != 2) then
      compilerError("Only 2D sparse domains are supported by the CSR distribution");
    this.dist = dist;
    this.parentDom = parentDom;
    nnz = 0;
    rowRange = parentDom.dim(1);
    colRange = parentDom.dim(2);
    rowDom = [rowRange.low..rowRange.high+1];
    nnzDomSize = nnz;
    nnzDom = [1..nnzDomSize];
    dsiClear();
  }

  def dsiNumIndices return nnz;

  def dsiGetIndices() return 0;
  def dsiSetIndices(x) { }

  def dsiBuildArray(type eltType)
    return new CSRArr(eltType=eltType, rank=rank, idxType=idxType, dom=this);

  def dsiIndsIterSafeForRemoving() {
    var cursorRow = rowRange.high;
    for i in 1..nnz by -1 {
      while (rowStart(cursorRow) > i) {
        cursorRow -= 1;
      }
      yield (cursorRow, colIdx(i));
    }
  }

  def these() {
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

  def these(param tag: iterator) where tag == iterator.leader {
    // same as DefaultSparseDom's leader
    const numElems = nnz;
    if numElems <= 0 then {
      if debugCSR then writeln("CSRDom leader: nothing to do (", numElems,
                               " elements)");
    } else {
      const numChunks = _computeNumChunks(numElems);
      //writeln("leader- rowRange=", rowRange, " colRange=", colRange, "\n",
      //        "        rowStart=", rowStart, " colIdx=", colIdx);
      if debugCSR then
        writeln("CSRDom leader: ", numChunks, " chunks, ", numElems, " elems");

      // split our numElems elements over numChunks tasks
      if numChunks <= 1 then
        yield (this, 1, numElems);
      else
        coforall chunk in 1..numChunks do
          yield (this, (..._computeChunkStartEnd(numElems, numChunks, chunk)));
      // TODO: to handle large numElems and numChunks faster, it would be great
      // to run the binary search in _private_findStartRow smarter, e.g.
      // pass to the tasks created in 'coforall' smaller ranges to search over.
    }  // if numElems
  }

  def these(param tag: iterator, follower: (?,?,?)) where tag == iterator.follower {
    var (followerDom, startIx, endIx) = follower;
    if boundsChecking then
      assert(startIx <= endIx, "CSRDom follower - got nothing to iterate over");

    if (followerDom != this) then {
      _haltLockSparseCSR = 1;
      halt("Sparse domains can't be zippered with anything other than themselves and their arrays (CSR layout)");
    }

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

  def these(param tag: iterator, follower) where tag == iterator.follower {
    compilerError("Sparse iterators can't yet be zippered with others (CSR layout)");
  }

  // Helper: find 'ix' s.t. rowStart(ix) <= startIx < rowStart(ix+1)
  // or a number at most 'approx' smaller than that.
  // There MUST exist a solution within low..high.
  def _private_findStartRow(startIx) {
    return _private_findStartRow(startIx, rowDom.low, rowDom.high);
  }
  def _private_findStartRow(startIx, low, high) {
    var approx = 2; // Indicates when to switch to linear search.
                    // This number could be tuned for performance.
    // simple binary search (should be fewer comparisons than BinarySearch())
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

  def dsiDim(d : int) {
    if (d == 1) {
      return rowRange;
    } else {
      return colRange;
    }
  }

  def boundsCheck(ind: rank*idxType):void {
    if boundsChecking then
      if !(parentDom.member(ind)) then
        halt("CSR domain/array index out of bounds: ", ind,
             " (expected to be within ", parentDom, ")");
  }

  def rowStop(row) {
    return rowStart(row+1)-1;
  }

  def find(ind: rank*idxType) {
    use Search;

    const (row, col) = ind;

    return BinarySearch(colIdx, col, rowStart(row), rowStop(row));
  }

  def dsiMember(ind: rank*idxType) {
    const (found, loc) = find(ind);
    return found;
  }

  def dsiAdd(ind: rank*idxType) {
    boundsCheck(ind);

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

      nnzDom = [1..nnzDomSize];
    }

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
    // BLC: Note: if arithmetic arrays had a user-settable
    // initialization value, we could set it to be the IRV and skip
    // this second initialization of any new values in the array.
    // we could also eliminate the oldNNZDomSize variable
    for a in _arrs {
      a.sparseShiftArray(insertPt..nnz-1, oldNNZDomSize+1..nnzDomSize);
    }
  }

  def dsiRemove(ind: rank*idxType) {
    // find position in nnzDom to remove old index
    const (found, insertPt) = find(ind);

    // if the index doesn't already exist, then return
    if (!found) then return;

    // increment number of nonzeroes
    nnz -= 1;

    // TODO: should halve nnzDom if we've outgrown it...
    /*
    var oldNNZDomSize = nnzDomSize;
    if (nnz > nnzDomSize) {
      nnzDomSize = if (nnzDomSize) then 2*nnzDomSize else 1;

      nnzDom = [1..nnzDomSize];
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
    // BLC: Note: if arithmetic arrays had a user-settable
    // initialization value, we could set it to be the IRV and skip
    // this second initialization of any new values in the array.
    // we could also eliminate the oldNNZDomSize variable
    for a in _arrs {
      a.sparseShiftArrayBack(insertPt..nnz-1);
    }
  }

  def dsiClear() {
    nnz = 0;
    rowStart = 1;
  }

  def dimIter(param d, ind) {
    if (d != 2) {
      compilerError("dimIter(1, ...) not supported on CSR domains");
    }
    for c in colIdx[rowStart(ind)..rowStop(ind)] do
      yield c;
  }
}


class CSRArr: BaseArr {
  type eltType;
  param rank : int;
  type idxType;

  var dom : CSRDom(rank=rank, idxType=idxType);
  var data: [dom.nnzDom] eltType;
  var irv: eltType;

  def dsiGetBaseDom() return dom;

  //  def this(ind: idxType ... 1) var where rank == 1
  //    return this(ind);

  def dsiAccess(ind: rank*idxType) var {
    // make sure we're in the dense bounding box
    dom.boundsCheck(ind);

    // lookup the index and return the data or IRV
    const (found, loc) = dom.find(ind);
    if setter && !found then
      halt("attempting to assign a 'zero' value in a sparse array: ", ind);
    if found then
      return data(loc);
    else
      return irv;
  }

  def these() var {
    for e in data[1..dom.nnz] do yield e;
  }

  def these(param tag: iterator) where tag == iterator.leader {
    // forward to the leader iterator on our domain
    // Note: this is so that arrays can be zippered with domains;
    // otherwise just chunk up data[1..dom.nnz] a-la DefaultSparseArr.
    for follower in dom.these(tag) do
      yield follower;
  }

  def these(param tag: iterator, follower: (?,?,?)) var where tag == iterator.follower {
    // simpler than CSRDom's follower - no need to deal with rows (or columns)
    var (followerDom, startIx, endIx) = follower;

    if (followerDom != this.dom) then {
      _haltLockSparseCSR = 1;
      halt("Sparse arrays can't be zippered with anything other than their domains and sibling arrays (CSR layout)");
    }
    if debugCSR then
      writeln("CSRArr follower: ", startIx, "..", endIx);

    for e in data[startIx..endIx] do yield e;
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    compilerError("Sparse iterators can't yet be zippered with others (CSR layout)");
  }

  def IRV var {
    return irv;
  }

  def sparseShiftArray(shiftrange, initrange) {
    for i in initrange {
      data(i) = irv;
    }
    for i in shiftrange by -1 {
      data(i+1) = data(i);
    }
    data(shiftrange.low) = irv;
  }

  def sparseShiftArrayBack(shiftrange) {
    for i in shiftrange {
      data(i) = data(i+1);
    }
  }
}


def CSRDom.dsiSerialWrite(f: Writer) {
  f.writeln("[");
  for r in rowRange {
    const lo = rowStart(r);
    const hi = rowStop(r);
    for c in lo..hi {
      f.write(" (", r, ", ", colIdx(c), ")", if (c==hi) then "\n" else "");
    }
  }
  f.writeln("]");
}


def CSRArr.dsiSerialWrite(f: Writer) {
  for r in dom.rowRange {
    const lo = dom.rowStart(r);
    const hi = dom.rowStop(r);
    for c in lo..hi {
      f.write(data(c), if (c==hi) then "\n" else " ");
    }
  }
}
