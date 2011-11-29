class TestDom: BaseSparseDom {
  param rank : int;
  type idxType;
  var parentDom;

  proc TestDom(param rank, type idxType, 
                               parentDom: domain) {
    this.parentDom = parentDom;
  }

  iter these1(followThis: (TestDom(?), ?)) { yield 999; }
}

var t = new TestDom(1, int, [1..8]:domain(1));

for i in t.these1((t,0)) do writeln(i);
