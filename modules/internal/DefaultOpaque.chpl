class DefaultOpaqueDom: BaseOpaqueDom {
  type idxType = _OpaqueIndex;
  var dist: DefaultDist;
  var adomain: DefaultAssociativeDom(idxType=_OpaqueIndex);

  def linksDistribution() param return false;

  def DefaultOpaqueDom(dist: DefaultDist) {
    this.dist = dist;
    adomain = new DefaultAssociativeDom(_OpaqueIndex, dist);
  }

  def ~DefaultOpaqueDom() {
    for i in adomain do delete i;
    delete adomain;
  }

  def dsiCreate() {
    var i = new _OpaqueIndex();
    adomain.dsiAdd(i);
    return i;
  }

  def dsiGetIndices() return adomain;

  def dsiSetIndices(b: DefaultAssociativeDom) {
    adomain.dsiSetIndices(b);
  }

  def these() {
    for i in adomain do
      yield i;
  }

  def these(param tag: iterator) where tag == iterator.leader {
    for block in adomain.these(tag=iterator.leader) do
      yield block;
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    for i in adomain.these(tag=iterator.follower, follower) do
      yield i;
  }

  def dsiMember(ind: idxType) {
    return adomain.dsiMember(ind);
  }

  def dsiNumIndices {
    return adomain.dsiNumIndices;
  }

  def dsiBuildArray(type eltType) {
    var ia = new DefaultOpaqueArr(eltType=eltType, idxType=idxType, dom=this);
    return ia;
  }
}

def DefaultOpaqueDom.dsiSerialWrite(f: Writer) {
  adomain.dsiSerialWrite(f);
}

def DefaultOpaqueArr.dsiSerialWrite(f: Writer) {
  anarray.dsiSerialWrite(f);
}

class DefaultOpaqueArr: BaseArr {
  type eltType;
  type idxType;

  var dom: DefaultOpaqueDom(idxType=idxType);
  var anarray = new DefaultAssociativeArr(eltType=eltType, idxType=idxType, dom=dom.adomain);

  def ~DefaultOpaqueArr() {
    delete anarray;
  }

  def dsiGetBaseDom() return dom;

  def dsiAccess(ind : idxType) var : eltType
    return anarray.dsiAccess(ind);

  def these() var {
    for e in anarray do
      yield e;
  }

  def these(param tag: iterator) where tag == iterator.leader {
    for block in dom.these(tag=iterator.leader) do
      yield block;
  }

  def these(param tag: iterator, follower) var where tag == iterator.follower {
    for i in dom.these(tag=iterator.follower, follower) do
      yield dsiAccess(i);
  }


  def dsiSorted() {
    for e in anarray.dsiSorted() do
      yield e;
  }

  def tupleInit(b: _tuple) {
    anarray.tupleInit(b);
  }
}

def DefaultOpaqueDom.dsiRemove(idx: idxType) {
  adomain.dsiRemove(idx);
}
