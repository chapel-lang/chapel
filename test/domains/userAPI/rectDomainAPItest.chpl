config param testError = 0, testDisplayRepresentation = false;

proc readArray(X) {
  open("realValues.txt", iomode.r).reader().read(X);
}

proc testRectDomainAPI2D(lbl, X: domain, sliceDom, sliceRanges, subsetDom) {
  // print header
  writeln(lbl);
  writeln("----------------");

  // Test generic aspects of domains
  writeln("idxType is: ", X.idxType: string);
  writeln("rank is: ", X.rank);
  writeln("stridable is: ", X.stridable);
  writeln();

  // Test simple quries
  //  writeln("dist is: ", X.dist);

  // Test simple queries
  writeln("dims is: ", X.dims());
  writeln("dim(1) is: ", X.dim(1));
  writeln("size is: ", X.size);
  writeln("numIndices is: ", X.numIndices);
  writeln("low is: ", X.low);
  writeln("high is: ", X.high);
  writeln("stride is: ", X.stride);
  writeln("alignment is: ", X.alignment);
  writeln("first is: ", X.first);
  writeln("last is: ", X.last);
  writeln("alignedLow is: ", X.alignedLow);
  writeln("alignedHigh is: ", X.alignedHigh);
  writeln("shape is: ", X.shape);
  writeln();

  // Test I/O
//  readArray(X);
  writeln("X is:\n", X);
  if testDisplayRepresentation {
    writeln("X's representation:"); X.displayRepresentation();
  }
  writeln();

  // Test serial iteration
  for i in X do
    writeln(i);
  writeln();

  // Test standalone parallel iteration
  var A: [X] index(X);
  forall x in X do
    A(x) = x;
  writeln("Indices are:\n", A);
  writeln();

  var B: [X] index(X);
  // Test leader parallel iteration
  forall (b,i) in zip(B, X) do
    b = i;
  writeln("Indices are:\n", B);
  writeln();

  var C: [X] index(X);
  // Test follower parallel iteration
  forall (i,c) in zip(X, C) do
    c = i;
  writeln("Indices are:\n", C);
  writeln();


  // Test locality interface
  writeln("target locales: ", X.targetLocales());
  if (X.hasSingleLocalSubdomain()) then
    writeln("local subdomain: ", X.localSubdomain());
  writeln("local subdomains:");
  for locdom in X.localSubdomains() do
    writeln(locdom);
  writeln();

  // Test views
  writeln("slice by domain ", sliceDom, ":\n", X[sliceDom]);
  writeln("slice by ranges ", sliceRanges, ":\n", X[(...sliceRanges)]);

  writeln("rank change 1: ", X[X.alignedLow(1), ..]);
  writeln("rank change 2: ", X[sliceDom.dim(1), X.alignedHigh(2)]);

  if testError == 1 then
    X.add(1);

  if testError == 2 then
    X.bulkAdd([1, 2, 3]);

  if testError == 3 then
    X.remove(1);

  if testError == 4 then
    X.requestCapacity(20);

  writeln("member test: ", X.member(X.alignedLow));

  if testError == 5 then
    writeln("subset test: ", X.isSubset(subsetDom));
  if testError == 6 then
    writeln("super test: ", X.isSuper(subsetDom));
  //  writeln("index order: ", X.indexOrder(X.alignedLow()(1)));
  //  writeln("position: ", X.position(1,1));
  writeln("expanded: ", X.expand(1));
  writeln("exterior: ", X.exterior(1));
  //  writeln("interior: ", X.interior(-1));
  writeln("translate: ", X.translate(1));
  //  writeln("count (#): ", X#(3,2));

  var Y = X;
  Y = subsetDom;
  writeln("copy reassigned to: ", subsetDom);

  if testError == 7 {
    writeln("In sorted order: ");
    for i in X.sorted() do
      writeln(i);
    writeln();
  }

  writeln("local slice: ");
  writeln(X.localSlice(sliceDom));

  X.clear();
  writeln("After clearing, X is: ", X);

  writeln();
}
