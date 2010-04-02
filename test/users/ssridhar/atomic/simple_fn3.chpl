var x: uint(64);

def main() {
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

def foo(i: int) {
  x = x + 1;
  atomic bar(i);
}

def bar(i: int) {
  foo(i);
}