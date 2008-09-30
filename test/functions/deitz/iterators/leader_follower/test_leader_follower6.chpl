def foo(n: int) {
  for i in 1..n do
    yield i;
}

def foo(param tag: iterator, n: int) where tag == iterator.leader {
  cobegin {
    yield 0..n/2-1;
    yield n/2..n-1;
  }
}

def foo(param tag: iterator, follower, n: int) where tag == iterator.follower {
  for i in follower+1 do
    yield i:int;
}

def bar(n: uint) {
  for i in 1..n do
    yield i;
}

def bar(param tag: iterator, n: uint) where tag == iterator.leader {
  yield 0..n/2-2;
  yield n/2-1..n-1;
}

def bar(param tag: iterator, follower, n: uint) where tag == iterator.follower {
  for i in follower+1 do
    yield i:uint;
}


config var n: int = 8;

var A: [1..n] int;
var B: [1..n:uint] int;

for (i,j) in (foo(n), bar(n:uint)) {
  A(i) = 1;
  B(j) = 1;
}

writeln(A);

forall (i,j) in (foo(n), bar(n:uint)) {
  A(i) = 2;
  B(j) = 2;
}

writeln(A);

forall (i,j) in (foo(n), bar(n:uint)) {
  A(i) = 3;
  B(j) = 3;
}

writeln(A);
