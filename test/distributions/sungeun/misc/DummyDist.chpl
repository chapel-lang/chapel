class Dummy: BaseDist {
  proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool, inds) {
    return new DummyDom(rank=rank, idxType=idxType, stridable=stridable);
  }
}

class DummyDom: BaseRectangularDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  var dist: Dummy;
}

const DummySpace: domain(1) dmapped new dmap(new Dummy());

