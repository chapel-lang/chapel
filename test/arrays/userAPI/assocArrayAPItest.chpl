config param testError = 0, testDisplayRepresentation = false;

proc readArray(X) {
  X["zero"]  = 0.0;
  X["pointone"]  = 0.1;
  X["half"]  = 0.5;
  X["one"]   = 1.0;
  X["two"]   = 2.0;
  X["three"] = 3.0;
  X["four"]  = 4.0;
  X["five"]  = 5.0;
  X["six"]   = 6.0;
  X["seven"] = 7.0;
  X["eight"] = 8.0;
  X["nine"]  = 9.0;
  X["ten"]   = 10.0;
}

proc testAssocArrayAPI(X: []) {
  // print header
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

  var low = "zero";
  var high = "half";

  // Test read access via tuples and varargs
  writeln("low element is: ", X[low]);
  if (X.rank > 1) then
    writeln("high element is: ", X[(...high)]);
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
  writeln("low element is: ", X.localAccess[low]);
  if (X.rank > 1) then
    writeln("high element is: ", X.localAccess[(...high)]);
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
  writeln("find last: ", X.find(X[high]));
  writeln("count last: ", X.count(X[high]));
  var Y = X;
  writeln("equals same: ", X.equals(Y));
  var Z = X + 0.1;
  writeln("equals diff: ", X.equals(Z));
  writeln();
}
