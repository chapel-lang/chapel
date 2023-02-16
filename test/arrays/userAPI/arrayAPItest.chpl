config param testError = 0, testDisplayRepresentation = false;

proc readArray(X) {
  use IO;

  open("realValues.txt", ioMode.r).reader().read(X);
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
  writeln("dims are: ", X.dims());
  writeln("dim(0) is: ", X.dim(0));

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
  writeln("low element is: ", X[X.domain.low]);
  writeln();
  // Test local write accesses via tuples and varargs
  forall ind in X.domain do
    X.localAccess[ind] += 0.1;
  writeln("X is:\n", X);
  writeln();
  const domForLowHigh = if X.hasSingleLocalSubdomain() then X.localSubdomain() else X.domain;
  // Test local read access via tuples and varargs
  writeln("low element is: ", X.localAccess[domForLowHigh.low]);
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
  // Test sparse-specific things
  if testError == 12 then
    writeln("IRV is: ", X.IRV);
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
  writeln("dims are: ", X.dims());
  writeln("dim(0) is: ", X.dim(0));
  writeln("dim(1) is: ", X.dim(1));

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
  writeln("low element is: ", X[X.domain.low]);
  if (X.rank > 1) then
    writeln("high element is: ", X[(...X.domain.high)]);
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
  writeln("low element is: ", X.localAccess[domForLowHigh.low]);
  if (X.rank > 1) then
    writeln("high element is: ", X.localAccess[(...domForLowHigh.high)]);
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
  var Y = X;
  writeln("equals same: ", X.equals(Y));
  var Z = X + 0.1;
  writeln("equals diff: ", X.equals(Z));
  writeln();

  // Test views
  writeln("slice by ", sliceDom, ":\n", X[sliceDom]);
  writeln("rank change 1: ", X[X.domain.low(0), ..]);
  writeln("rank change 2: ", X[sliceDom.dim(0), X.domain.high(1)]);
  for (i,x) in zip(reindexDom, X.reindex(reindexDom)) do
    writeln("reindexed X[", i, "] = ", x);
  writeln();

  // Test sparse-specific things
  if testError == 12 then
    writeln("IRV is: ", X.IRV);
}
