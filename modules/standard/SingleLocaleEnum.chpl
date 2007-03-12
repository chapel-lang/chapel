class SingleLocaleEnumDomain: BaseDomain {
  type ind_type;
  var _arrs2: seq(BaseArray);
  var adomain: SingleLocaleAssociativeDomain(ind_type=ind_type);

  def initialize() {
    adomain = SingleLocaleAssociativeDomain(ind_type=ind_type);
    for i in _enum_enumerate(ind_type) do
      adomain.add(i);
  }

  def getIndices() return this;

  def setIndices(b: SingleLocaleEnumDomain) {
    adomain.setIndices(b.adomain);
  }

  def getHeadCursor() {
    return adomain.getHeadCursor();
  }

  def getNextCursor(c) {
    return adomain.getNextCursor(c);
  }

  def getValue(c) {
    return adomain.getValue(c);
  }

  def isValidCursor?(c) {
    return adomain.isValidCursor?(c);
  }

  def member?(ind: ind_type) {
    adomain.member?(ind);
  }

  def numIndicies {
    return adomain.numIndicies;
  }

  def buildEmptyDomain()
    return SingleLocaleEnumDomain(ind_type=ind_type);

  def buildArray(type eltType) {
    var ia = SingleLocaleEnumArray(eltType, ind_type, dom=this);
    _arrs2 #= ia;
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
  type dim_type;

  var dom: SingleLocaleEnumDomain(ind_type=dim_type);
  var anarray = SingleLocaleAssociativeArray(eltType, dim_type, dom.adomain);

  def this(ind : dim_type) var : eltType
    return anarray(ind);

  def getHeadCursor() {
    return anarray.getHeadCursor();
  }

  def getNextCursor(c)
    return anarray.getNextCursor(c);

  def getValue(c)
    return anarray.data(c);

  def isValidCursor?(c)
    return anarray.isValidCursor?(c);

  def numElements {
    return anarray.numElements;
  }
}

