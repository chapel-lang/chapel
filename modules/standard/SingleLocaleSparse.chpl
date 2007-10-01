use Search;
use List;

class SingleLocaleSparseDomain: BaseSparseArithmeticDomain {
  param rank : int;
  type idxType;
  var parentDom: BaseArithmeticDomain;
  var nnz = 0;  // intention is that user might specify this to avoid reallocs

  var nnzDomSize = nnz;
  var nnzDom = [1..nnzDomSize];

  var indices: [nnzDom] index(rank);

  def initialize() {
    nnz = 0;
  }

  def numIndices return nnz;

  def getIndices() return 0;
  def setIndices(x);

  def buildArray(type eltType)
    return SingleLocaleSparseArray(eltType, rank, idxType, dom=this);

  def buildEmptyDomain()
    return SingleLocaleSparseDomain(rank=rank, idxType=idxType, parentDom=parentDom);

  def these() {
    for i in 1..nnz {
      yield indices(i);
    }
  }

  def dim(d : int) {
    return parentDom.bbox(d);
  }

  // private
  def find(ind) {
    return BinarySearch(indices, ind, 1, nnz);
  }

  def member(ind) { // ind should be verified to be index type
    const (found, loc) = find(ind);
    return found;
  }

  def add_help(ind) {
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
    // shift indices up
    for i in [insertPt..nnz) by -1 {
      indices(i+1) = indices(i);
    }

    indices(insertPt) = ind;

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

  def add(ind: idxType) where rank == 1 {
    add_help(ind);
  }

  def add(ind: rank*idxType) {
    add_help(ind);
  }

  def clear() {
    nnz = 0;
  }

  def dimIter(param d, ind) {
    if (d != rank-1) {
      compilerError("dimIter() not supported on sparse domains for dimensions other than the last");
    }
    halt("dimIter() not yet implemented for sparse domains");
    yield indices(1);
  }
}


class SingleLocaleSparseArray: BaseArray {
  type eltType;
  param rank : int;
  type idxType;

  var dom : SingleLocaleSparseDomain(rank=rank, idxType=idxType);
  var data: [dom.nnzDom] eltType;
  var irv: eltType;

  def this(ind: idxType) var where rank == 1 {
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

  def these() var {
    for e in data[1..dom.nnz] yield e;
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


def SingleLocaleSparseDomain.writeThis(f: Writer) {
  if (rank == 1) {
    f.write("[");
    if (nnz >= 1) {
      f.write(indices(1));
      for i in 2..nnz {
        f.write(" ", indices(i));
      }
    }
    f.write("]");
  } else {
    f.writeln("[");
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
    }
    f.writeln("\n]");
  }
}


def SingleLocaleSparseArray.writeThis(f: Writer) {
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
