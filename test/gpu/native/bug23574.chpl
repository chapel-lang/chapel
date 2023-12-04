{
  // reproducer from https://github.com/chapel-lang/chapel/issues/23574
  use BlockDist;

  const n = 8;
  const Space = {1..n, 1..n};

  const BlkDist = new blockDist(boundingBox=Space);
  const BlockSpace = BlkDist.createDomain(Space);
  var BA: [BlockSpace] int;

  const myIndices = BA.localSubdomain();
}

{
  // standalone reproducer without `BlockDist`
  proc foo(dom, tgt) {
    for d in dom do
      if d == tgt then
        return d;
    return dom.first;
  }

  {
    // 1-D, this worked
    var dom = {1..10};
    var t = foo(dom, 2);
    writeln(t);
  }
  {
    // 2-D, this didn't work. now it does
    var dom = {1..10, 1..10};
    var t = foo(dom, (2,3));
    writeln(t);
  }
}
