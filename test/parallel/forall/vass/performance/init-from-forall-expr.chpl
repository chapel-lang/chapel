
config var n = 4;

iter randomIntPoints() {
  writeln("randomIntPoints serial iterator");
  for i in 1..n {
    yield i;
  }
}

iter randomIntPoints(param tag) where tag == iterKind.leader
{
  writeln("randomIntPoints leader iterator");
  yield 1..n;
}

iter randomIntPoints(followThis, param tag) where tag == iterKind.follower
{
  writeln("randomIntPoints follower iterator");
  for i in followThis {
    yield i;
  }
}

proc main {
  var B = [idx in randomIntPoints()] idx * 10;
  writeln(B);
}
