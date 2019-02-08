
config const n = 4;

proc PLUS1(a:int, b:int) { if (a*b) == 0 then writeln(a * b); }
proc PLUS2(a:int, b:int) { if (a*b) == 0 then writeln(a * b); return 0; }

writeln("plus1");
PLUS1(100,             randomIntPoints());
PLUS1(moreIntPoints(), randomIntPoints());
writeln("plus2");
PLUS2(200,             randomIntPoints());
PLUS2(moreIntPoints(), randomIntPoints());
writeln("done");

iter randomIntPoints() {
  writeln("randomIntPoints serial iterator");
  for i in 1..n {
    yield i-1;
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
    yield i-1;
  }
}

iter moreIntPoints() {
  writeln("moreIntPoints serial iterator");
  for i in 1..n {
    yield i;
  }
}

iter moreIntPoints(param tag) where tag == iterKind.leader
{
  writeln("moreIntPoints leader iterator");
  yield 1..n;
}

iter moreIntPoints(followThis, param tag) where tag == iterKind.follower
{
  writeln("moreIntPoints follower iterator");
  for i in followThis {
    yield i;
  }
}
