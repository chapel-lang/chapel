use List;
class DefaultOpaqueDom: BaseOpaqueDom {
  type idxType = _OpaqueIndex;
  var dist: DefaultDist;
  var adomain: DefaultAssociativeDom(idxType=_OpaqueIndex);

  def DefaultOpaqueDom(dist: DefaultDist) {
    this.dist = dist;
    adomain = new DefaultAssociativeDom(_OpaqueIndex, dist);
  }

  def create() {
    var i = new _OpaqueIndex();
    adomain.add(i);
    return i;
  }

  def getIndices() return adomain;

  def setIndices(b: DefaultAssociativeDom) {
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
    var ia = new DefaultOpaqueArr(eltType, idxType, dom=this);
    return ia;
  }
}

def DefaultOpaqueDom.writeThis(f: Writer) {
  adomain.writeThis(f);
}

def DefaultOpaqueArr.writeThis(f: Writer) {
  anarray.writeThis(f);
}

class DefaultOpaqueArr: BaseArr {
  type eltType;
  type idxType;

  var dom: DefaultOpaqueDom(idxType=idxType);
  var anarray = new DefaultAssociativeArr(eltType, idxType, dom.adomain);

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

def DefaultOpaqueDom.remove(idx: idxType) {
  adomain.remove(idx);
}
