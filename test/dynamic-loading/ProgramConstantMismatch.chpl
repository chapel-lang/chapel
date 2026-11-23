use ChapelDynamicLoading;
use ChapelLibraryTestCommon only chapelLibraryExtension;
use DynamicLibraryHelper;

config param chapelLibraryPath = './lib/libIncompatibleChapelLibrary' +
                                 '.' + chapelLibraryExtension;
config param constant = 'CHPL_CACHE_REMOTE';
config param expectError = true;

proc main() {
  const lib = new dynamicLibraryHelper(chapelLibraryPath);
  const err = lib.err();

  writeln('Checking constant: ', constant);

  if err != nil {
    writeln(err!.message());
  }

  if !expectError {
    assert(err == nil);
    writeln('SKIPPED');
  } else {
    assert(err != nil);
    assert(err!.message().contains(constant));
  }
}
