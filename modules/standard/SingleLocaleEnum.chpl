use List;
class SingleLocaleEnumDomain: BaseDomain {
  param rank: int;
  type idxType;
  var adomain: SingleLocaleAssociativeDomain(rank=rank, idxType=idxType);

  def initialize() {
    adomain = new SingleLocaleAssociativeDomain(rank=rank, idxType=idxType);
    var enumTuple = _enum_enumerate(idxType);
    for param i in 1..enumTuple.size do
      adomain.add(enumTuple(i));
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
    return new SingleLocaleEnumDomain(rank=rank, idxType=idxType);

  def buildArray(type eltType) {
    var ia = new SingleLocaleEnumArray(eltType, idxType, dom=this);
    return ia;
  }
}

def SingleLocaleEnumDomain.writeThis(f: Writer) {
  adomain.writeThis(f);
}

def SingleLocaleEnumArray.writeThis(f: Writer) {
  anarray.writeThis(f);
}

class SingleLocaleEnumArray: BaseArray {
  type eltType;
  type idxType;

  var dom: SingleLocaleEnumDomain(rank=1, idxType=idxType);
  var anarray = new SingleLocaleAssociativeArray(eltType, idxType, dom.adomain);

  def this(ind : idxType) var : eltType
    return anarray(ind);

  def these() {
    for e in anarray do
      yield e;
  }

  def numElements {
    return anarray.numElements;
  }

  def tupleInit(b: _tuple) {
    anarray.tupleInit(b);
  }
}
