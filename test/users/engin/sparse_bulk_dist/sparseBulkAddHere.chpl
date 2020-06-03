use BlockDist;

proc test(space) {
  type idxType = if space.rank==1 then int else space.rank*int;
  const dom = space dmapped Block(boundingBox=space);
  var spsdom: sparse subdomain(dom);

  coforall l in Locales do on l {
    const locSubDom = dom.localSubdomain();
    var myIndices: [0..1] idxType;

    myIndices[0] = locSubDom.low;
    myIndices[1] = locSubDom.high;

    spsdom.bulkAdd(myIndices, addOn=here);
  }
  for i in spsdom do
    write(i, " ");
  writeln();
}

test({1..10});
test({1..10, 1..10});
