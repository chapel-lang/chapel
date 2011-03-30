class DefaultOpaqueDom: BaseOpaqueDom {
  type idxType = _OpaqueIndex;
  var dist: DefaultDist;
  var adomain: DefaultAssociativeDom(idxType=_OpaqueIndex);

  proc linksDistribution() param return false;
  proc dsiLinksDistribution()     return false;

  proc DefaultOpaqueDom(dist: DefaultDist) {
    this.dist = dist;
    adomain = new DefaultAssociativeDom(_OpaqueIndex, dist);
  }

  proc ~DefaultOpaqueDom() {
    for i in adomain do delete i;
    delete adomain;
  }

  proc dsiCreate() {
    var i = new _OpaqueIndex();
    adomain.dsiAdd(i);
    return i;
  }

  proc dsiGetIndices() return adomain;

  proc dsiSetIndices(b: DefaultAssociativeDom) {
    adomain.dsiSetIndices(b);
  }

  iter these() {
    for i in adomain do
      yield i;
  }

  iter these(param tag: iterator) where tag == iterator.leader {
    for block in adomain.these(tag=iterator.leader) do
      yield block;
  }

  iter these(param tag: iterator, follower) where tag == iterator.follower {
    for i in adomain.these(tag=iterator.follower, follower) do
      yield i;
  }

  proc dsiMember(ind: idxType) {
    return adomain.dsiMember(ind);
  }

  proc dsiNumIndices {
    return adomain.dsiNumIndices;
  }

  proc dsiBuildArray(type eltType) {
    var ia = new DefaultOpaqueArr(eltType=eltType, idxType=idxType, dom=this);
    return ia;
  }
}

proc DefaultOpaqueDom.dsiSerialWrite(f: Writer) {
  adomain.dsiSerialWrite(f);
}

proc DefaultOpaqueArr.dsiSerialWrite(f: Writer) {
  anarray.dsiSerialWrite(f);
}

class DefaultOpaqueArr: BaseArr {
  type eltType;
  type idxType;

  var dom: DefaultOpaqueDom(idxType=idxType);
  var anarray = new DefaultAssociativeArr(eltType=eltType, idxType=idxType, dom=dom.adomain);

  proc ~DefaultOpaqueArr() {
    delete anarray;
  }

  proc dsiGetBaseDom() return dom;

  proc dsiAccess(ind : idxType) var : eltType
    return anarray.dsiAccess(ind);

  iter these() var {
    for e in anarray do
      yield e;
  }

  iter these(param tag: iterator) where tag == iterator.leader {
    for block in dom.these(tag=iterator.leader) do
      yield block;
  }

  iter these(param tag: iterator, follower) var where tag == iterator.follower {
    for i in dom.these(tag=iterator.follower, follower) do
      yield dsiAccess(i);
  }


  iter dsiSorted() {
    for e in anarray.dsiSorted() do
      yield e;
  }
}

proc DefaultOpaqueDom.dsiRemove(idx: idxType) {
  adomain.dsiRemove(idx);
}
