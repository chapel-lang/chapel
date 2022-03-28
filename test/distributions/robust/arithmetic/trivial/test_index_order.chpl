use driver_domains;

proc testio(dom, order, idx) {
  writeln("--- ", dom:string, " ", dom.sizeAs(int), " ---");
  writeln(dom.orderToIndex(0));
  writeln(dom.orderToIndex(dom.sizeAs(dom.idxType)-1));
  writeln(dom.orderToIndex(order));
  writeln(dom.indexOrder(idx));
  for ijk in dom do
    assert(dom.orderToIndex(dom.indexOrder(ijk)) == ijk);
}

testio(Dom1D, 12, 21);
testio(Dom2D, 123, (3,5));
testio(Dom3D, 67, (2,4,1));
testio(Dom4D, 37, (3,2,1,3));
testio(Dom2D32, 11, (12345675: int(32), 12345685: int(32)));
writeln("--- all done ---");
