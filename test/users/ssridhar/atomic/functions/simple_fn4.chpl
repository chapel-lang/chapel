var x: uint(64);

proc main() {
  var i, j: int;
  coforall i in 1..10 {
    foo(j); // check if the non-transactional copy is called
    atomic {
      foo(j);
      j += 1;
      foo(j); // check if the cache works
    }
  }
}

proc foo(i: int) {
  x = x + 1;
  atomic bar(i);
}

proc bar(i: int) {
  x = x + 1;
}