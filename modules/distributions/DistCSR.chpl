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
    var cursorRow = rowRange.low;
    for i in 1..nnz {
      while (rowStart(cursorRow+1) <= i) {
        cursorRow += 1;
      }
      yield (cursorRow, colIdx(i));
    }
  }

  def these(param tag: iterator) where tag == iterator.leader {
    yield true;
  }

  def these(param tag: iterator, follower: bool) where tag == iterator.follower {
    var cursorRow = rowRange.low;
    for i in 1..nnz {
      while (rowStart(cursorRow+1) <= i) {
        cursorRow += 1;
      }
      yield (cursorRow, colIdx(i));
    }
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    compilerError("Sparse iterators can't yet be zippered with others");
  }

  def dsiDim(d : int) {
    if (d == 1) {
      return rowRange;
    } else {
      return colRange;
    }
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
    if boundsChecking then
      if !((dom.parentDom).member(ind)) then
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

  def these() var {
    for e in data[1..dom.nnz] do yield e;
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
