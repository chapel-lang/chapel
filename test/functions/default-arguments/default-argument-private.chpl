module Library {
  private proc myZero(param rank : int, type idxType) {
    var ret : rank*idxType;
    return ret;
  }

  proc foo(param rank=1,
           type idxType=int,
           tup:rank*idxType = myZero(rank, idxType)) {
    writeln("In foo ", tup);
  }

}

module Test {
  use Library;

  proc main() {
    foo(1, real);
  }
}
