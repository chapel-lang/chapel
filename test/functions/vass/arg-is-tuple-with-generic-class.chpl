class TestDom: BaseSparseDom {
  param rank : int;
  type idxType;
  var parentDom;

  def TestDom(param rank, type idxType, 
                               parentDom: domain) {
    this.parentDom = parentDom;
  }

  def these1(follower: (TestDom(?), ?)) { yield 999; }
}

var t = new TestDom(1, int, [1..8]:domain(1));

for i in t.these1((t,0)) do writeln(i);
