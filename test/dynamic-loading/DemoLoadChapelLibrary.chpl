use DynamicLoading;

// Provides the correct extension based on platform, e.g., '.dylib' or '.so'.
use ChapelLibraryTestCommon only chapelLibraryExtension;

proc main() {
  const path = "./lib/libDemoChapelLibrary." + chapelLibraryExtension;
  var lib = binary.load(path);
  type testType = proc(): void;
  const testProc = try! lib.retrieve("test1", testType);
  testProc();
}
