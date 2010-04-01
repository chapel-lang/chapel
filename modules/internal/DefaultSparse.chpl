use Search;

class DefaultSparseDom: BaseSparseDom {
  param rank : int;
  type idxType;
  var parentDom: domain(rank, idxType);
  var dist: DefaultDist;
  var nnz = 0;  // intention is that user might specify this to avoid reallocs

  var nnzDomSize = nnz;
  var nnzDom = [1..nnzDomSize];

  var indices: [nnzDom] index(rank);

  def linksDistribution() param return false;

  def DefaultSparseDom(param rank, type idxType, 
                               dist: DefaultDist,
                               parentDom: domain(rank, idxType)) {
    this.parentDom = parentDom;
    this.dist = dist;
    nnz = 0;
  }

  def dsiNumIndices return nnz;

  def dsiBuildArray(type eltType)
    return new DefaultSparseArr(eltType=eltType, rank=rank, idxType=idxType,
                                dom=this);

  def dsiIndsIterSafeForRemoving() {
    for i in 1..nnz by -1 {
      yield indices(i);
    }
  }

  def these() {
    for i in 1..nnz {
      yield indices(i);
    }
  }

  def these(param tag: iterator) where tag == iterator.leader {
    compilerWarning("parallel iteration over sparse domain has been serialized (see note in $CHPL_HOME/STATUS)");
    yield true;
  }

  def these(param tag: iterator, follower: bool) where tag == iterator.follower {
    for i in 1..nnz do
      yield indices(i);
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    compilerError("Sparse iterators can't yet be zippered with others");
  }

  def dsiDim(d : int) {
    return parentDom.dim(d);
  }

  // private
  def find(ind) {
    //
    // sjd: unfortunate specialization for rank == 1
    // sjd: would it be better if indices were an array of rank*idxType?
    // sjd: isn't it a bug as is because the idxType may not match index(rank)?
    //
    if rank == 1 && isTuple(ind) && ind.size == 1 then
      return BinarySearch(indices, ind(1), 1, nnz);
    else
      return BinarySearch(indices, ind, 1, nnz);
  }

  def dsiMember(ind) { // ind should be verified to be index type
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
    for i in insertPt..nnz-1 by -1 {
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

  def rem_help(ind) {
    // find position in nnzDom to insert new index
    const (found, insertPt) = find(ind);

    // if the index already existed, then return
    if (!found) then return;

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
    // BLC: Note: if arithmetic arrays had a user-settable
    // initialization value, we could set it to be the IRV and skip
    // this second initialization of any new values in the array.
    // we could also eliminate the oldNNZDomSize variable
    for a in _arrs {
      a.sparseShiftArrayBack(insertPt..nnz-1);
    }
  }

  def dsiAdd(ind: idxType) where rank == 1 {
    add_help(ind);
  }

  def dsiRemove(ind: idxType) where rank == 1 {
    rem_help(ind);
  }

  def dsiAdd(ind: rank*idxType) {
    add_help(ind);
  }

  def dsiRemove(ind: rank*idxType) {
    rem_help(ind);
  }

  def dsiClear() {
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


class DefaultSparseArr: BaseArr {
  type eltType;
  param rank : int;
  type idxType;

  var dom : DefaultSparseDom(rank, idxType);
  var data: [dom.nnzDom] eltType;
  var irv: eltType;

  def dsiGetBaseDom() return dom;

  def dsiAccess(ind: idxType) var where rank == 1 {
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

  def dsiAccess(ind: rank*idxType) var {
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


def DefaultSparseDom.dsiSerialWrite(f: Writer) {
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
      f.writeln();
    }
    f.writeln("]");
  }
}


def DefaultSparseArr.dsiSerialWrite(f: Writer) {
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
