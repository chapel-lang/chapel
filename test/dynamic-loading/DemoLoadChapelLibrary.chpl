use DynamicLoading;

proc main() {
  var lib = binary.load("./lib/libDemoChapelLibrary.so");
  type testType = proc(): void;
  const testProc = try! lib.retrieve("test1", testType);
  testProc();
}
