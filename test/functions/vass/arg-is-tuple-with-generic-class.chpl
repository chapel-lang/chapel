class TestDom: BaseSparseDom {
  param rank_ : int;
  type idxType_;
  var parentDom_;

  proc init(param rank, type idxType,
                               parentDom: domain) {
    super.init(rank, idxType, parentDom);
    this.rank_ = rank;
    this.idxType_ = idxType;
    this.parentDom_ = parentDom;
  }

  iter these1(followThis: (TestDom(?), ?)) { yield 999; }
}

var t = new TestDom(1, int, {1..8}:domain(1));

for i in t.these1((t,0)) do writeln(i);
