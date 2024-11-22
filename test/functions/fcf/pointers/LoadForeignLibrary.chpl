use ChapelDynamicLoading;

proc test1() {
  var err;
  var got = chpl_BinaryInfo.create("./TestCLibraryToLoad.so", err);
  if err != nil then writeln(err);
  assert(got != nil && err == nil);
  var bin = got!;

  type P1 = proc(_: int, _: int): int;
  const addTwoReturn = bin.loadSymbol("addTwoReturn", P1, err);
  if err != nil then writeln(err);
  assert(addTwoReturn != nil && err == nil);

  for loc in Locales do on loc {
    const n = (here.id : int);
    const x = addTwoReturn(n, n);
    writeln(here, ' got: ', x);
  }

  // TODO: This is leaking system handles right now.
  bin.dropRefCount();
  delete bin;
}

proc main() {
  test1();
}
