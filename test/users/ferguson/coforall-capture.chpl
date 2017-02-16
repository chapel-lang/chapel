iter modifyAndYield(ref x) {
  for i in 1..10 {
    x = i;
    yield i;
  }
}

proc run() {
  var x:int;
  coforall i in modifyAndYield(x) {
    assert(x == i);
  }
}

run();
