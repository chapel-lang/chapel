var x: uint(64);

proc main() {
  var i: int;
  coforall i in 1..10 {
    foo(i); // check if the non-transactional copy is called
    atomic {
      foo(i);
      foo(i); // check if the cache works
    }
  }
  writeln("x = ", x);
}

proc foo(i: int) { // 1 argument
  atomic bar();
}

proc bar() { // 0 arguments
  x = x + 1;
}