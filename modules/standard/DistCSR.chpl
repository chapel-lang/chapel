class CSR {
  param stridable: bool = false;

  def buildDomain(param rank: int, type idxType) {
    halt("The CSR distribution does not support dense domains");
  }

  def buildEnumDomain(type idxType) {
    halt("The CSR distribution does not support enumerated domains");
  }

  def buildDomain(type idxType) {
    halt("The CSR distribution does not support associative domains");
  }

  def buildSparseDomain(param rank:int, type idxType, 
                        parentDom: BaseArithmeticDomain) {
    return new CSRDomain(rank=rank, idxType=idxType, 
                                    parentDom=parentDom);
  }
}

use Search;

class CSRDomain: BaseSparseArithmeticDomain {
  param rank : int;
  type idxType;
  var parentDom: BaseArithmeticDomain;
  var nnz = 0;  // intention is that user might specify this to avoid reallocs
  //  type idxType = rank*idxType;

  var rowRange = parentDom.bbox(1);
  var colRange = parentDom.bbox(2);

  const rowDom = [rowRange.low..rowRange.high+1];
  var nnzDomSize = nnz;
  var nnzDom = [1..nnzDomSize];

  var rowStart: [rowDom] idxType;      // would like index(nnzDom)
  var colIdx: [nnzDom] idxType;        // would like index(parentDom.bbox(1))

  def initialize() {
    if (rank != 2) then
      compilerError("Only 2D sparse domains are supported presently");
    for i in rowDom do rowStart(i) = 1;
    nnz = 0;
  }

  def numIndices return nnz;

  def getIndices() return 0;
  def setIndices(x) { }

  def buildArray(type eltType)
    return new CSRArray(eltType, rank, idxType, dom=this);

  def buildEmptyDomain()
    return new CSRDomain(rank=rank, idxType=idxType, parentDom=parentDom);

  def these() {
    var cursorRow = rowRange.low;
    for i in 1..nnz {
      while (rowStart(cursorRow+1) <= i) {
        cursorRow += 1;
      }
      yield (cursorRow, colIdx(i));
    }
  }

  def dim(d : int) {
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
    const (row, col) = ind;

    return BinarySearch(colIdx, col, rowStart(row), rowStop(row));
  }

  def member(ind: rank*idxType) {
    const (found, loc) = find(ind);
    return found;
  }

  def add(ind: rank*idxType) {
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

  def clear() {
    nnz = 0;
  }

  def dimIter(param d, ind) {
    if (d != 2) {
      compilerError("dimIter(1, ...) not supported on CSR domains");
    }
    for c in colIdx[rowStart(ind)..rowStop(ind)] do
      yield c;
  }
}


class CSRArray: BaseArray {
  type eltType;
  param rank : int;
  type idxType;

  var dom : CSRDomain(rank=rank, idxType=idxType);
  var data: [dom.nnzDom] eltType;
  var irv: eltType;

  //  def this(ind: idxType ... 1) var where rank == 1
  //    return this(ind);

  def this(ind: rank*idxType) var {
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
}


def CSRDomain.writeThis(f: Writer) {
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


def CSRArray.writeThis(f: Writer) {
  for r in dom.rowRange {
    const lo = dom.rowStart(r);
    const hi = dom.rowStop(r);
    for c in lo..hi {
      f.write(data(c), if (c==hi) then "\n" else " ");
    }
  }
}
