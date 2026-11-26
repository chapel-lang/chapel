use ChapelDynamicLoading;
use ChapelLibraryTestCommon only chapelLibraryExtension;
use DynamicLibraryHelper;

config param chapelLibraryPath = './lib/libIncompatibleChapelLibrary' +
                                 '.' + chapelLibraryExtension;
config param constant = 'CHPL_CACHE_REMOTE';
config param expectError = true;

proc skipConstant() {
  use ChplConfig;
  if !expectError then return true;
  if constant == 'CHPL_LAUNCHER' && CHPL_LAUNCHER == 'none' then return true;
  return false;
}

proc main() {
  const lib = new dynamicLibraryHelper(chapelLibraryPath);
  const err = lib.err();

  writeln('Checking constant: ', constant);

  if err != nil {
    // Always write the error message so we can debug more easily.
    writeln(err!.message());
  }

  if skipConstant() {
    assert(err == nil);
    writeln('SKIPPED');
  } else {
    assert(err != nil);
    assert(err!.message().contains(constant));
  }
}
