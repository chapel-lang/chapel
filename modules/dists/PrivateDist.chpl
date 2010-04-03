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

  def dsiSerialWrite(x: Writer) { x.write("Private Domain"); }

  def dsiBuildArray(type eltType)
    return new PrivateArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);

  def dsiNumIndices return numLocales;
  def dsiLow return 0;
  def dsiHigh return numLocales-1;
  def dsiSetIndices(x: domain) { compilerError("cannot reassign private domain"); }
  def dsiGetIndices() { return [0..numLocales-1]; }

  def dsiRequiresPrivatization() param return true;
  def linksDistribution() param return false;

  def dsiGetPrivatizeData() return 0;

  def dsiPrivatize(privatizeData)
    return new PrivateDom(rank=rank, idxType=idxType, stridable=stridable, dist=dist);

  def dsiGetReprivatizeData() return 0;

  def dsiReprivatize(other, reprivatizeData) { }

  def dsiMember(i) return 0 <= i && i <= numLocales-1;
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

def PrivateArr.dsiGetBaseDom() return dom;

def PrivateArr.dsiRequiresPrivatization() param return true;

def PrivateArr.dsiGetPrivatizeData() return 0;

def PrivateArr.dsiPrivatize(privatizeData) {
  var dompid = dom.pid;
  var thisdom = dom;
  var privdom = __primitive("chpl_getPrivatizedClass", thisdom, dompid);
  return new PrivateArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
}

def PrivateArr.dsiAccess(i: idxType) var {
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

def PrivateArr.dsiAccess(i: 1*idxType) var
  return dsiAccess(i(1));

def PrivateArr.these() var {
  for i in dom do
    yield dsiAccess(i);
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
    yield dsiAccess(i);
}

def PrivateArr.dsiSerialWrite(x: Writer) {
  var first: bool = true;
  for i in dom {
    if first then first = !first; else write(" ");
    write(dsiAccess(i));
  }
}

const PrivateSpace: domain(1) dmapped new dmap(new Private());
