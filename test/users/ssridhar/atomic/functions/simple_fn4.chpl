var x: int(64);

proc main() {
  var i: int;
  foo(1); // check if the non-transactional copy is called
  coforall i in 1..10 {
    atomic {
      foo(i);
      foo(i); // check if the cache works
    }
  }
  writeln("x = ", x);
}

proc foo(i: int) {
  x = x + i;
  atomic bar(i);
}

proc bar(i: int) {
  x = x + i;
}