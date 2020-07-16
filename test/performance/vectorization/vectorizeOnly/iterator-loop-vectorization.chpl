// This test verifies that an iterator in a follower
// loop isn't inherently marked vectorized. vectorizeOnly
// must be used to opt in to it.

config const n = 1000;

proc nextcursor(cursor) {
  return (cursor * cursor + 137.0)  / 136.0;
}

iter testiter() {
  var cursor : real = 0.0;
  for i in 1..n {
    cursor = nextcursor(cursor);
    yield cursor;
  }
}

iter testiter(param tag: iterKind) where tag == iterKind.leader {
  for block in (1..n).these(tag=iterKind.leader) do
    yield block;
}

iter testiter(param tag: iterKind, followThis) where tag == iterKind.follower {
  var cursor : real = 0.0;
  for i in (1..n).these(tag=tag, followThis) {
    cursor = nextcursor(cursor);
    yield cursor;
  }
}


proc testfunc() {
  var A:[1..n] real;
  forall (x,i) in zip(testiter(), 1..n) {
    A[i] = x;
  }
}
testfunc();

iter testveciter() {
  var cursor : real = 0.0;
  for i in 1..n {
    yield i;
  }
}

iter testveciter(param tag: iterKind) where tag == iterKind.leader {
  for block in (1..n).these(tag=iterKind.leader) do
    yield block;
}

iter testveciter(param tag: iterKind, followThis) where tag == iterKind.follower
{
  var cursor : real = 0.0;
  for i in (1..n).these(tag=tag, followThis) {
    yield i;
  }
}

proc testvecfunc() {
  var A:[1..n] real;
  forall (x,i) in zip(testveciter(), 1..n) {
    A[i] = x;
  }
}
testvecfunc();
