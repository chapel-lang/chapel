use List;
class SingleLocaleEnumDomain: BaseEnumDomain {
  param rank = 1;
  type idxType;
  var adomain: SingleLocaleAssociativeDomain(idxType);

  def SingleLocaleEnumDomain(type idxType) {
    adomain = new SingleLocaleAssociativeDomain(idxType);
    var enumTuple = _enum_enumerate(idxType);
    for param i in 1..enumTuple.size do
      adomain.add(enumTuple(i));
  }

  def getIndices() return adomain;

  def setIndices(b: SingleLocaleAssociativeDomain) {
    adomain.setIndices(b);
  }

  def these() {
    var enumTuple = _enum_enumerate(idxType);
    for param i in 1..enumTuple.size do
      yield enumTuple(i);
  }

  def member(ind: idxType) {
    adomain.member(ind);
  }

  def numIndices {
    return adomain.numIndices;
  }

  def buildEmptyDomain()
    return new SingleLocaleEnumDomain(idxType);

  def buildArray(type eltType) {
    var ia = new SingleLocaleEnumArray(eltType, idxType, dom=this);
    return ia;
  }
}

def SingleLocaleEnumDomain.writeThis(f: Writer) {
  var enumTuple = _enum_enumerate(idxType);
  f.write("[");
  if (enumTuple.size > 1) {
    f.write(enumTuple(1));
    for param i in 2..enumTuple.size do
      f.write(", ", enumTuple(i));
  }
  f.write("]");
}

def SingleLocaleEnumArray.writeThis(f: Writer) {
  var enumTuple = _enum_enumerate(idxType);
  if (enumTuple.size > 1) {
    f.write(anarray(enumTuple(1)));
    for param i in 2..enumTuple.size do
      f.write(" ", anarray(enumTuple(i)));
  }
}

class SingleLocaleEnumArray: BaseArray {
  type eltType;
  type idxType;

  var dom: SingleLocaleEnumDomain(idxType=idxType);
  var anarray = new SingleLocaleAssociativeArray(eltType, idxType, dom.adomain);

  def this(ind : idxType) var : eltType
    return anarray(ind);

  def these() var {
    var enumTuple = _enum_enumerate(idxType);
    for param i in 1..enumTuple.size do
      yield anarray(enumTuple(i));
  }

  def numElements {
    return anarray.numElements;
  }

  def tupleInit(b: _tuple) {
    var enumTuple = _enum_enumerate(idxType);
    for param i in 1..enumTuple.size do
      anarray.this(enumTuple(i)) = b(i);
  }
}
