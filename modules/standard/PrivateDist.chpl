//
// Private Distribution, Domain, and Array
//  Defines PrivateSpace, an instance of PrivateDom
//
class Private: BaseDist {
  def dsiNewArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    return new PrivateDom(rank=rank, idxType=idxType, stridable=stridable);
  }

  def writeThis(x: Writer) {
    x.writeln("Private Distribution");
  }
}

class PrivateDom: BaseArithmeticDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  var pid: int = -1;
  var dist: Private;

  def getDist() return dist;
  def these() { for i in 0..numLocales-1 do yield i; }

  def these(param tag: iterator) where tag == iterator.leader {
    coforall loc in Locales do on loc {
      var t: 1*range(idxType);
      t(1) = here.id..here.id;
      yield t;
    }
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    for i in follower(1) do
      yield i;
  }

  def writeThis(x: Writer) { x.write("Private Domain"); }

  def buildArray(type eltType)
    return new PrivateArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);

  def numIndices return numLocales;
  def low return 0;
  def high return numLocales-1;
  def setIndices(x: domain) { compilerError("cannot reassign private domain"); }
  def getIndices() { return [0..numLocales-1]; }

  def requiresPrivatization() param return true;
  def linksDistribution() param return false;

  def getPrivatizeData() return 0;

  def privatize(privatizeData)
    return new PrivateDom(rank=rank, idxType=idxType, stridable=stridable, dist=dist);

  def getReprivatizeData() return 0;

  def reprivatize(other, reprivatizeData) { }

  def member(i) return 0 <= i && i <= numLocales-1;
}

class PrivateArr: BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  var dom: PrivateDom(rank, idxType, stridable);
  var data: eltType;
  var pid: int = -1;
}

def PrivateArr.getBaseDom() return dom;

def PrivateArr.requiresPrivatization() param return true;

def PrivateArr.getPrivatizeData() return 0;

def PrivateArr.privatize(privatizeData) {
  var dompid = dom.pid;
  var thisdom = dom;
  var privdom = __primitive("chpl_getPrivatizedClass", thisdom, dompid);
  return new PrivateArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
}

def PrivateArr.this(i: idxType) var {
  if _local then
    return data;
  else if i == here.id then
    return data;
  else {
    if boundsChecking then
      if i < 0 || i >= numLocales then
        halt("array index out of bounds: ", i);
    var arrpid = this.pid;
    var privarr = this;
    on Locales(i) {
      privarr = __primitive("chpl_getPrivatizedClass", privarr, arrpid);
    }
    return privarr.data;
  }
}

def PrivateArr.these() var {
  for i in dom do
    yield this(i);
}

def PrivateArr.these(param tag: iterator) where tag == iterator.leader {
  coforall loc in Locales do on loc {
    var t: 1*range(idxType);
    t(1) = here.id..here.id;
    yield t;
  }
}

def PrivateArr.these(param tag: iterator, follower) var where tag == iterator.follower {
  for i in follower(1) do
    yield this(i);
}

def PrivateArr.writeThis(x: Writer) {
  var first: bool = true;
  for i in dom {
    if first then first = !first; else write(" ");
    write(this(i));
  }
}

const PrivateSpace: domain(1) distributed new dist(new Private());
