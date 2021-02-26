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


proc forall_testiter() {
  var A:[1..n] real;
  // should not be vectorized because of loop carried dependency in testiter
  forall x in testiter() {
  }
}
forall_testiter();

proc forall_testiterz() {
  var A:[1..n] real;
  // should not be vectorized because of loop carried dependency in testiter
  forall (x,i) in zip(testiter(), 1..n) {
    A[i] = x;
  }
}
forall_testiterz();

proc vec_testiter() {
  var A:[1..n] real;
  // should not be vectorized because of loop carried dependency in testiter
  for x in vectorizeOnly(testiter()) {
  }
}
vec_testiter();

proc vec_testiterz() {
  var A:[1..n] real;
  // should not be vectorized because of loop carried dependency in testiter
  for (x,i) in vectorizeOnly(testiter(), 1..n) {
    A[i] = x;
  }
}
vec_testiterz();


pragma "order independent yielding loops"
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

pragma "order independent yielding loops"
iter testveciter(param tag: iterKind, followThis) where tag == iterKind.follower
{
  var cursor : real = 0.0;
  for i in (1..n).these(tag=tag, followThis) {
    yield i;
  }
}

proc forall_testveciter() {
  var A:[1..n] real;
  // vectorization OK because testveciter has no loop carried dependency
  forall x in testveciter() {
  }
}
forall_testveciter();

proc forall_testveciterz() {
  var A:[1..n] real;
  // vectorization OK because testveciter has no loop carried dependency
  forall (x,i) in zip(testveciter(), 1..n) {
    A[i] = x;
  }
}
forall_testveciterz();

proc vec_testveciter() {
  var A:[1..n] real;
  // vectorization OK because testveciter has no loop carried dependency
  for x in vectorizeOnly(testveciter()) {
  }
}
vec_testveciter();

proc vec_testveciterz() {
  var A:[1..n] real;
  // vectorization OK because testveciter has no loop carried dependency
  for (x,i) in vectorizeOnly(testveciter(), 1..n) {
    A[i] = x;
  }
}
vec_testveciterz();
