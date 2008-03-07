use List;
class SingleLocaleOpaqueDomain: BaseDomain {
  param rank: int;
  type idxType = _OpaqueIndex;
  var adomain: SingleLocaleAssociativeDomain(rank=rank, idxType=_OpaqueIndex);

  def initialize() {
    adomain = new SingleLocaleAssociativeDomain(rank=rank, idxType=_OpaqueIndex);
  }

  def create() {
    var i = new _OpaqueIndex();
    adomain.add(i);
    return i;
  }

  def getIndices() return adomain;

  def setIndices(b: SingleLocaleAssociativeDomain) {
    adomain.setIndices(b);
  }

  def these() {
    for i in adomain do
      yield i;
  }

  def member(ind: idxType) {
    adomain.member(ind);
  }

  def numIndices {
    return adomain.numIndices;
  }

  def buildEmptyDomain()
    return new SingleLocaleOpaqueDomain(rank=rank, idxType=idxType);

  def buildArray(type eltType) {
    var ia = new SingleLocaleOpaqueArray(eltType, idxType, dom=this);
    return ia;
  }
}

def SingleLocaleOpaqueDomain.writeThis(f: Writer) {
  adomain.writeThis(f);
}

def SingleLocaleOpaqueArray.writeThis(f: Writer) {
  anarray.writeThis(f);
}

class SingleLocaleOpaqueArray: BaseArray {
  type eltType;
  type idxType;

  var dom: SingleLocaleOpaqueDomain(rank=1, idxType=idxType);
  var anarray = new SingleLocaleAssociativeArray(eltType, idxType, dom.adomain);

  def this(ind : idxType) var : eltType
    return anarray(ind);

  def these() {
    for e in anarray do
      yield e;
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
