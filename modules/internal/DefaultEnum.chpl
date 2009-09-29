class DefaultEnumDom: BaseEnumDom {
  type idxType;
  var dist: DefaultDist;
  var adomain: DefaultAssociativeDom(idxType);

  def linksDistribution() param return false;

  def DefaultEnumDom(type idxType, dist: DefaultDist) {
    this.dist = dist;
    adomain = new DefaultAssociativeDom(idxType, dist);
    var enumTuple = _enum_enumerate(idxType);
    for param i in 1..enumTuple.size do
      adomain.add(enumTuple(i));
  }

  def ~DefaultEnumDom() {
    delete adomain;
  }

  def getIndices() return adomain;

  def setIndices(b: DefaultAssociativeDom) {
    adomain.setIndices(b);
  }

  def these() {
    var enumTuple = _enum_enumerate(idxType);
    for param i in 1..enumTuple.size do
      yield enumTuple(i);
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
    adomain.member(ind);
  }

  def numIndices {
    return adomain.numIndices;
  }

  def buildArray(type eltType) {
    var ia = new DefaultEnumArr(eltType=eltType, idxType=idxType, dom=this);
    return ia;
  }
}

def DefaultEnumDom.writeThis(f: Writer) {
  var enumTuple = _enum_enumerate(idxType);
  f.write("[");
  if (enumTuple.size > 1) {
    f.write(enumTuple(1));
    for param i in 2..enumTuple.size do
      f.write(", ", enumTuple(i));
  }
  f.write("]");
}

def DefaultEnumArr.writeThis(f: Writer) {
  var enumTuple = _enum_enumerate(idxType);
  if (enumTuple.size > 1) {
    f.write(anarray(enumTuple(1)));
    for param i in 2..enumTuple.size do
      f.write(" ", anarray(enumTuple(i)));
  }
}

class DefaultEnumArr: BaseArr {
  type eltType;
  type idxType;

  var dom: DefaultEnumDom(idxType=idxType);
  var anarray = new DefaultAssociativeArr(eltType=eltType, idxType=idxType, dom=dom.adomain);

  def ~DefaultEnumArr() {
    delete anarray;
  }

  def getBaseDom() return dom;

  def this(ind : idxType) var : eltType
    return anarray(ind);

  def these() var {
    var enumTuple = _enum_enumerate(idxType);
    for param i in 1..enumTuple.size do
      yield anarray(enumTuple(i));
  }

  def these(param tag: iterator) where tag == iterator.leader {
    for block in dom.these(tag=iterator.leader) do
      yield block;
  }

  def these(param tag: iterator, follower) var where tag == iterator.follower {
    for i in dom.these(tag=iterator.follower, follower) do
      yield this(i);
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
