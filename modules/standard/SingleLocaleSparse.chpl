class SingleLocaleSparseDomain: BaseDomain {
  param rank : int;
  type dim_type;
  var parentDom: BaseDomain;  // want this to be BaseArithmeticDomain
  //  type ind_type = rank*dim_type;
  var nnz = 0;

  // const rowRange = parentDom.bbox(1); // this is what I want
  const rowRange = -1..2000;

  // I'd like both of these things to go away over time
  var actualColRange = max(int)..min(int);
  var actualRowRange = max(int)..min(int);


  const rowDom = [rowRange.low..rowRange.high+1];
  var nnzDom = [1..nnz];

  var rowStart: [rowDom] dim_type;      // would like index(nnzDom)
  var colIdx: [nnzDom] dim_type;        // would like index(parentDom.bbox(1))

  def initialize() {
    if (rank != 2) then
      compilerError("Only 2D sparse domains are supported presently");
    for i in rowDom do rowStart(i) = 1;
  }

  def getIndices() return 0;
  def setIndices(x);

  def buildArray(type eltType)
    return SingleLocaleSparseArray(eltType, rank, dim_type, dom=this);

  def buildEmptyDomain()
    return SingleLocaleSparseDomain(rank=rank, dim_type=dim_type);

  def getHeadCursor() {
    var c: rank*dim_type;
    return c;
  }

  def getValue(c) {
    if rank == 1 then
      return c(1);
    else
      return c;
  }

  def this(dim : int) {
    if (dim == 1) {
      return actualRowRange; // BLC: return parentDom.this(dim)?
    } else {
      return actualColRange;
    }
  }

  def rowStop(row) {
    return rowStart(row+1)-1;
  }

  def find(ind: rank*dim_type) {
    const (row, col) = ind;

    // BLC: seems like if I ran this backward the common case of
    // inserting in sorted order would run more quickly

    // search indices in this row
    for i in rowStart(row)..rowStop(row) {
      if (colIdx(i) == col) {
        // hit -- found index
        return (true, i);
      } else if (colIdx(i) > col) {
        // miss -- found index greater than the one we're looking for
        return (false, i);
      }
    }
    // miss -- fell off end of row
    return (false, rowStop(row)+1);
  }

  def member?(ind: rank*dim_type) {
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
    var nnzDomSize = nnzDom.numIndices;
    if (nnz > nnzDomSize) {
      nnzDomSize = if (nnzDomSize) then 2*nnzDomSize else 1;

      nnzDom = [1..nnzDomSize];
    }

    const (row,col) = ind;

    // expand col range
    if (col < actualColRange.low) {
      actualColRange = col..actualColRange.high;
    }
    if (col > actualColRange.high) {
      actualColRange = actualColRange.low..col;
    }
    if (row < actualRowRange.low) {
      actualRowRange = row..actualRowRange.high;
    }
    if (row > actualRowRange.high) {
      actualRowRange = actualRowRange.low..row;
    }

    // shift column indices and array data up
    for i in [insertPt+1..nnz) by -1 {
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
    for r in row+1..rowRange.high {  // want rowDom[row+1..]
      rowStart(r) += 1;
    }

    // still need to re-allocate the arrays
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

  def this(ind: rank*dim_type) var {
    // make sure we're in the dense bounding box
    if boundsChecking then
      if !((dom.parentDom).member?(ind)) then
        halt("array index out of bounds: ", ind);

    // lookup the index and return the data or IRV
    const (found, loc) = dom.find(ind);
    return if (found) then data(loc) else irv;
  }
}


def SingleLocaleSparseArray.writeThis(f: Writer) {
  halt("Sparse arrays cannot currently be written");
}
