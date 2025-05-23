import ChapelDynamicLoading as mod;

proc test0() {
  // Test should never actually execute, but assume it fails just in case.
  compilerAssert(!mod.isDynamicLoadingSupported);

  assert(!mod.isDynamicLoadingEnabled);

  var err: mod.DynLoadError?;
  var bin = mod.chpl_BinaryInfo.create("somePathThatDoesNotExist", err);
  if const e = err! then writeln(e);

  if const b = bin! {
    // Should not reach here, but should get another compiler error.
    type P = proc(): void;
    const p = bin.loadSymbol("foobarbaz", P, err);
    if const e = err! then writeln(e);
  }
}

proc main() {
  test0();
}
