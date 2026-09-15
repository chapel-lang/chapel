use DynamicLoading;

proc main() {
  const path = "./lib/libDemoChapelLibrary." + binary.libSuffix;
  var lib = binary.load(path);
  type testType = proc(): void;
  const testProc = try! lib.retrieve("test1", testType);
  testProc();
}
