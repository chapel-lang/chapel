config param testError = 0, testDisplayRepresentation = false;

proc readArray(X) {
  use IO;

  open("realValues.txt", iomode.r).reader().read(X);
}

proc testArrayAPI1D(lbl, X: [], sliceDom, reindexDom) {
  // print header
  writeln(lbl);
  writeln("----------------");
  // Test generic aspects of arrays
  writeln("eltType is: ", X.eltType: string);
  writeln("idxType is: ", X.idxType: string);
  writeln("rank is: ", X.rank);
  writeln();
  // Test simple queries
  writeln("size is: ", X.size);

  writeln("shape is: ", X.shape);
  writeln();
  // Test I/O
  readArray(X);
  writeln("X is:\n", X);
  if testDisplayRepresentation {
    writeln("X's representation:"); X.displayRepresentation();
  }
  writeln();
  // Test write accesses via tuples and varargs
  forall ind in X.domain do
    X[ind] += 0.1;
  writeln("X is:\n", X);
  writeln();
  // Test read access via tuples and varargs
  writeln("low element is: ", X[X.domain.alignedLow]);
  writeln();
  // Test local write accesses via tuples and varargs
  forall ind in X.domain do
    X.localAccess[ind] += 0.1;
  writeln("X is:\n", X);
  writeln();
  const domForLowHigh = if X.hasSingleLocalSubdomain() then X.localSubdomain() else X.domain;
  // Test local read access via tuples and varargs
  writeln("low element is: ", X.localAccess[domForLowHigh.alignedLow]);
  writeln();
  // Test serial iteration
  for x in X do
    x += 0.1;
  writeln("X is:\n", X);
  writeln();
  // Test standalone parallel iteration
  forall x in X do
    x += 0.1;
  writeln("X is:\n", X);
  writeln();
  // Test leader parallel iteration
  forall (x,i) in zip(X, X.domain) do
    x += 0.1;
  writeln("X is:\n", X);
  writeln();
  // Test follower parallel iteration
  forall (i,x) in zip(X.domain, X) do
    x += 0.1;
  writeln("X is:\n", X);
  writeln();
  // Test locality interface
  writeln("target locales: ", X.targetLocales());
  if (X.hasSingleLocalSubdomain()) then
    writeln("local subdomain: ", X.localSubdomain());
  writeln("local subdomains:");
  for locdom in X.localSubdomains() do
    writeln(locdom);
  writeln();
  // Test collection interface
  writeln("is empty: ", X.isEmpty());
  writeln("head: ", X.head());
  writeln("tail: ", X.tail());
  writeln("find last: ", X.find(X[X.domain.alignedHigh]));
  writeln("count last: ", X.count(X[X.domain.alignedHigh]));
  var Y = X;
  writeln("equals same: ", X.equals(Y));
  var Z = X + 0.1;
  writeln("equals diff: ", X.equals(Z));
  writeln();
  // Test views
  writeln("slice by ", sliceDom, ":\n", X[sliceDom]);
  for (i,x) in zip(reindexDom, X.reindex(reindexDom)) do
    writeln("reindexed X[", i, "] = ", x);
  writeln();
  // Test vector ops
  if testError == 10 then
    X.reverse();
  // Test sparse-specific things
  if testError == 12 then
    writeln("IRV is: ", X.IRV);
  // Test sorted iterator
  if testError == 13 {
    writeln("in sorted order: ");
    for x in X.sorted() do
      writeln(x);
  }
}

proc testArrayAPI2D(lbl, X: [], sliceDom, reindexDom) {
  // print header
  writeln(lbl);
  writeln("----------------");

  // Test generic aspects of arrays
  writeln("eltType is: ", X.eltType: string);
  writeln("idxType is: ", X.idxType: string);
  writeln("rank is: ", X.rank);
  writeln();

  // Test simple queries
  writeln("size is: ", X.size);

  writeln("shape is: ", X.shape);
  writeln();

  // Test I/O
  readArray(X);
  writeln("X is:\n", X);
  if testDisplayRepresentation {
    writeln("X's representation:"); X.displayRepresentation();
  }
  writeln();

  // Test write accesses via tuples and varargs
  forall ind in X.domain do
    X[ind] += 0.1;
  writeln("X is:\n", X);
  writeln();
  if (X.rank > 1) then
    forall ind in X.domain do
      X[(...ind)] += 0.1;
  writeln("X is:\n", X);
  writeln();

  // Test read access via tuples and varargs
  writeln("low element is: ", X[X.domain.alignedLow]);
  if (X.rank > 1) then
    writeln("high element is: ", X[(...X.domain.alignedHigh)]);
  writeln();

  // Test local write accesses via tuples and varargs
  forall ind in X.domain do
    X.localAccess[ind] += 0.1;
  writeln("X is:\n", X);
  writeln();
  if (X.rank > 1) then
    forall ind in X.domain do
      X.localAccess[(...ind)] += 0.1;
  writeln("X is:\n", X);
  writeln();

  const domForLowHigh = if X.hasSingleLocalSubdomain() then X.localSubdomain() else X.domain;
  // Test local read access via tuples and varargs
  writeln("low element is: ", X.localAccess[domForLowHigh.alignedLow]);
  if (X.rank > 1) then
    writeln("high element is: ", X.localAccess[(...domForLowHigh.alignedHigh)]);
  writeln();

  // Test serial iteration
  for x in X do
    x += 0.1;
  writeln("X is:\n", X);
  writeln();

  // Test standalone parallel iteration
  forall x in X do
    x += 0.1;
  writeln("X is:\n", X);
  writeln();

  // Test leader parallel iteration
  forall (x,i) in zip(X, X.domain) do
    x += 0.1;
  writeln("X is:\n", X);
  writeln();

  // Test follower parallel iteration
  forall (i,x) in zip(X.domain, X) do
    x += 0.1;
  writeln("X is:\n", X);
  writeln();

  // Test locality interface
  writeln("target locales: ", X.targetLocales());
  if (X.hasSingleLocalSubdomain()) then
    writeln("local subdomain: ", X.localSubdomain());
  writeln("local subdomains:");
  for locdom in X.localSubdomains() do
    writeln(locdom);
  writeln();

  // Test collection interface
  writeln("is empty: ", X.isEmpty());
  writeln("head: ", X.head());
  writeln("tail: ", X.tail());
  writeln("find last: ", X.find(X[X.domain.alignedHigh]));
  writeln("count last: ", X.count(X[X.domain.alignedHigh]));
  var Y = X;
  writeln("equals same: ", X.equals(Y));
  var Z = X + 0.1;
  writeln("equals diff: ", X.equals(Z));
  writeln();

  // Test views
  writeln("slice by ", sliceDom, ":\n", X[sliceDom]);
  writeln("rank change 1: ", X[X.domain.alignedLow(1), ..]);
  writeln("rank change 2: ", X[sliceDom.dim(1), X.domain.alignedHigh(2)]);
  for (i,x) in zip(reindexDom, X.reindex(reindexDom)) do
    writeln("reindexed X[", i, "] = ", x);
  writeln();

  // Test vector ops
  if testError == 10 then
    X.reverse();
  
  // Test sparse-specific things
  if testError == 12 then
    writeln("IRV is: ", X.IRV);

  // Test sorted iterator
  if testError == 13 {
    writeln("in sorted order: ");
    for x in X.sorted() do
      writeln(x);
  }
}
