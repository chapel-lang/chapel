config param testError = 0, testDisplayRepresentation = false;

proc testDomainAPI2D(lbl, D: domain, idx, OOBidx1, OOBidx2, intDom) {
  writeln(lbl);
  writeln("------------");
  writeln(D);
  writeln("rank             = ", D.rank);
  writeln("idxType          = ", D.idxType:string);
  writeln("stridable        = ", D.stridable);
  writeln("size             = ", D.size);
  writeln("isEmpty()        = ", D.isEmpty());
  writeln("low              = ", D.low);
  writeln("high             = ", D.high);
  writeln("stride           = ", D.stride);
  writeln("alignment        = ", D.alignment);
  writeln("first            = ", D.first);
  writeln("last             = ", D.last);
  writeln("alignedLow       = ", D.alignedLow);
  writeln("alignedHigh      = ", D.alignedHigh);
  writeln("dims()           = ", D.dims());
  writeln("dim(1)           = ", D.dim(1));
  writeln("dim(2)           = ", D.dim(2));
  writeln("contains(",idx,")   = ", D.contains(idx));
  writeln("contains(", OOBidx1, ")   = ", D.contains(OOBidx1));
  writeln("contains(", OOBidx2, ")   = ", D.contains(OOBidx2));
  writeln("indexOrder(", idx, ") = ", D.indexOrder(idx));
  
  writeln("shape            = ", D.shape);
  writeln("position(", idx, ") = ", D.position(idx));
  writeln("expand(1)        = ", D.expand(1));
  writeln("expand(1,0)      = ", D.expand(1, 0));
  writeln("expand((0,2))    = ", D.expand((0, 2)));
  writeln("exterior(1)      = ", D.exterior(1));
  writeln("exterior(1,0)    = ", D.exterior(1,0));
  writeln("exterior((0,2))  = ", D.exterior((0,2)));
  writeln("interior(1)      = ", D.interior(1));
  writeln("interior(1,0)    = ", D.interior(1,0));
  writeln("interior((0,2)   = ", D.interior((0,2)));
  writeln("translate(1)     = ", D.translate(1));
  writeln("translate(1,0)   = ", D.translate(1,0));
  writeln("translate((0,2)) = ", D.translate((0,2)));
          
  writeln("serial iteration = ");
  for i in D do
    write(i, " ");
  writeln();

  // parallel iteration is handled in test/arrays/userAPI/arrayAPItest.chpl

  writeln("D[",intDom,"] = ", D[intDom]);
  writeln("D[",intDom," (as ranges)] = ", D[intDom.dim(1), intDom.dim(2)]);

  writeln("D.localSlice(",intDom,") = ", D.localSlice[intDom]);
  writeln("D.targetLocales() = ", D.targetLocales());
  writeln("D.hasSingleLocalSubdomain() = ", D.hasSingleLocalSubdomain());
  writeln("D.localSubdomain() = ", D.localSubdomain());

  writeln("D#(2,2) = ", D#(2,2));
  writeln("D == intDom = ", D == intDom);
  writeln("D(intDom) == intDom(D) = ", D(intDom) == intDom(D));
  writeln("D != intDom = ", D != intDom);
  writeln("D(intDom) != intDom(D) = ", D(intDom) != intDom(D));
  
  writeln("Copying...");
  var D2 = D;
  writeln(D2);
  writeln("Clearing...");
  D2.clear();
  writeln(D2);
  writeln("size             = ", D2.size);
  writeln("isEmpty()        = ", D2.isEmpty());
  writeln("low              = ", D2.low);
  writeln("high             = ", D2.high);
  writeln("stride           = ", D2.stride);
  writeln("alignment        = ", D2.alignment);
  writeln();
}
