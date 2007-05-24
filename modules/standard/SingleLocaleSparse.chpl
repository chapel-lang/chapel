use Search;

class SingleLocaleSparseDomain: BaseArithmeticDomain {
  param rank : int;
  type dim_type;
  var parentDom: BaseArithmeticDomain;
  var nnz = 0;  // intention is that user might specify this to avoid reallocs
  //  type ind_type = rank*dim_type;

  var rowRange = parentDom.bbox(1);
  var colRange = parentDom.bbox(2);

  const rowDom = [rowRange.low..rowRange.high+1];
  var nnzDomSize = nnz;
  var nnzDom = [1..nnzDomSize];

  var rowStart: [rowDom] dim_type;      // would like index(nnzDom)
  var colIdx: [nnzDom] dim_type;        // would like index(parentDom.bbox(1))

  def initialize() {
    if (rank != 2) then
      compilerError("Only 2D sparse domains are supported presently");
    for i in rowDom do rowStart(i) = 1;
    nnz = 0;
  }

  def numIndices return nnz;

  def getIndices() return 0;
  def setIndices(x);

  def buildArray(type eltType)
    return SingleLocaleSparseArray(eltType, rank, dim_type, dom=this);

  def buildEmptyDomain()
    return SingleLocaleSparseDomain(rank=rank, dim_type=dim_type, 
                                    parentDom = BaseArithmeticDomain());

  // really, really would like to replace the following with a true
  // iterator like:
  //
  // iterator this() {
  //   for i in 1..nnz {
  //     while (rowStart(cursorRow+1) < i) {
  //       cursorRow += 1;
  //     }
  //     yield (cursorRow, colIdx(i));
  //   }
  // }

  iterator ault() {
    var cursorRow = rowRange.low;
    for i in 1..nnz {
      while (rowStart(cursorRow+1) <= i) {
        cursorRow += 1;
      }
      yield (cursorRow, colIdx(i));
    }
  }
  /*
  var cursorRow: index(rowDom);
  var cursorColInd: index(nnzDom);

  def getHeadCursor() {
    cursorRow = rowRange.low;
    cursorColInd = 1;
    //    writeln("rowStart = ", rowStart);
    while (rowStart(cursorRow+1) <= cursorColInd) {
      cursorRow += 1;
    }
    return (cursorRow, colIdx(cursorColInd));
  }

  def getNextCursor(c) {
    cursorColInd += 1;
    if (cursorColInd <= nnz) {
      while (rowStart(cursorRow+1) <= cursorColInd) {
        cursorRow += 1;
      }
      return (cursorRow, colIdx(cursorColInd));
    }
    return (-1,-1);
  }

  def getValue(c) {
    return c;
  }

  def isValidCursor?(c) {
    return cursorColInd <= nnz;
  }
  */
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

  def find(ind: rank*dim_type) {
    const (row, col) = ind;

    return BinarySearch(colIdx, col, rowStart(row), rowStop(row));
  }

  def member(ind: rank*dim_type) {
    const (found, loc) = find(ind);
    return found;
  }

  def add(ind: rank*dim_type) {
    // find position in nnzDom to insert new index
    const (found, insertPt) = find(ind);

    // if the index already existed, then return
    if (found) then return;

    // increment number of nonzeroes
    nnz += 1;

    // double nnzDom if we've outgrown it
    if (nnz > nnzDomSize) {
      nnzDomSize = if (nnzDomSize) then 2*nnzDomSize else 1;

      nnzDom = [1..nnzDomSize];
    }

    const (row,col) = ind;

    // shift column indices and array data up
    for i in [insertPt..nnz) by -1 {
      colIdx(i+1) = colIdx(i);
      // need to shift arrays as well
      /*
      for a in _arrs {
        const arr = a:SingleLocaleSparseArray(?et, ?rank, ?dt);
        arr.data(i+1) = arr.data(i);
      }
      */
    }

    colIdx(insertPt) = col;

    // bump the rowStart counts
    for r in row+1..rowDom.high {  // want rowDom[row+1..]
      rowStart(r) += 1;
    }

    // still need to re-allocate the arrays
  }

  iterator dimIter(param d, ind) {
    if (d != 2) {
      compilerError("dimIter(1, ...) not supported on CRS domains");
    }
    for c in colIdx[rowStart(ind)..rowStop(ind)] do
      yield c;
  }
}


class SingleLocaleSparseArray: BaseArray {
  type eltType;
  param rank : int;
  type dim_type;

  var dom : SingleLocaleSparseDomain(rank=rank, dim_type=dim_type);
  var data: [dom.nnzDom] eltType;
  var irv: eltType;

  //  def this(ind: dim_type ... 1) var where rank == 1
  //    return this(ind);

  def this(ind: rank*dim_type) {
    // make sure we're in the dense bounding box
    if boundsChecking then
      if !((dom.parentDom).member(ind)) then
        halt("array index out of bounds: ", ind);

    // lookup the index and return the data or IRV
    const (found, loc) = dom.find(ind);
    return if (found) then data(loc) else irv;
  }


  def =this(ind: rank*dim_type, val:eltType) {
    // make sure we're in the dense bounding box
    if boundsChecking then
      if !((dom.parentDom).member(ind)) then
        halt("array index out of bounds: ", ind);

    // lookup the index and return the data or IRV
    const (found, loc) = dom.find(ind);
    if found then
      data(loc) = val;
    else
      halt("attempting to assign a 'zero' value in a sparse array: ", ind);
  }

  def IRV var {
    return irv;
  }
}


def SingleLocaleSparseDomain.writeThis(f: Writer) {
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


def SingleLocaleSparseArray.writeThis(f: Writer) {
  for r in dom.rowRange {
    const lo = dom.rowStart(r);
    const hi = dom.rowStop(r);
    for c in lo..hi {
      f.write(data(c), if (c==hi) then "\n" else " ");
    }
  }
}
