class SingleLocaleSparseDomain: BaseDomain {
  param rank : int;
  type dim_type;
  param stridable: bool;
  //  type ind_type = rank*dim_type;
  var adomain : SingleLocaleArithmeticDomain(rank=rank, dim_type=dim_type, stridable=stridable);
  //  var inds: seq(rank*dim_type);
  var inds: rank*seq(dim_type);
  var numinds = 0;

  def getIndices() return 0;
  def setIndices(x);

  def buildArray(type eltType)
    return SingleLocaleSparseArray(eltType, rank, dim_type, stridable, dom=this);

  def buildEmptyDomain()
    return SingleLocaleSparseDomain(rank=rank, dim_type=dim_type, stridable=stridable, adomain=adomain);

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

  def add(ind : rank*dim_type) {
    for param d in 1..rank {
      inds(d) #= ind(d);
    }
    numinds += 1;
  }
}


class SingleLocaleSparseArray: BaseArray {
  type eltType;
  param rank : int;
  type dim_type;
  param stridable: bool;

  var dom : SingleLocaleSparseDomain(rank=rank, dim_type=dim_type, stridable=stridable);
  var data: [1..30] eltType;
  var irv: eltType;

  def this(ind: dim_type ... 1) var where rank == 1
    return this(ind);

  var x: eltType;

  def this(ind: rank*dim_type) var {
    for i in 1..dom.numinds {
      var match = true;
      for param d in 1..rank {
        if (dom.inds(d)(i) != ind(d)) then match = false;
      }
      if (match) {
        return data[i];
      }
    }
    return irv;
  }
}


def SingleLocaleSparseArray.writeThis(f: Writer) {
  halt("Sparse arrays cannot currently be written");
}
