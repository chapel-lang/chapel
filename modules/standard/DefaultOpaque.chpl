use List;
class DefaultOpaqueDomain: BaseOpaqueDomain {
  type idxType = _OpaqueIndex;
  var dist: DefaultDistribution;
  var adomain: DefaultAssociativeDomain(idxType=_OpaqueIndex);

  def DefaultOpaqueDomain(dist: DefaultDistribution) {
    this.dist = dist;
    adomain = new DefaultAssociativeDomain(_OpaqueIndex, dist);
  }

  def create() {
    var i = new _OpaqueIndex();
    adomain.add(i);
    return i;
  }

  def getIndices() return adomain;

  def setIndices(b: DefaultAssociativeDomain) {
    adomain.setIndices(b);
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

  def member(ind: idxType) {
    return adomain.member(ind);
  }

  def numIndices {
    return adomain.numIndices;
  }

  def buildArray(type eltType) {
    var ia = new DefaultOpaqueArray(eltType, idxType, dom=this);
    return ia;
  }
}

def DefaultOpaqueDomain.writeThis(f: Writer) {
  adomain.writeThis(f);
}

def DefaultOpaqueArray.writeThis(f: Writer) {
  anarray.writeThis(f);
}

class DefaultOpaqueArray: BaseArray {
  type eltType;
  type idxType;

  var dom: DefaultOpaqueDomain(idxType=idxType);
  var anarray = new DefaultAssociativeArray(eltType, idxType, dom.adomain);

  def this(ind : idxType) var : eltType
    return anarray(ind);

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
      yield this(i);
  }


  def sorted() {
    for e in anarray.sorted() do
      yield e;
  }

  def numElements {
    return anarray.numElements;
  }

  def tupleInit(b: _tuple) {
    anarray.tupleInit(b);
  }
}

def DefaultOpaqueDomain.remove(idx: idxType) {
  adomain.remove(idx);
}
