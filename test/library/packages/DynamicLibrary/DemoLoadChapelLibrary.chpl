use DynamicLibrary;

proc main() {
  const path = "./lib/libDemoChapelLibrary." + dynamicLibrary.fileExt;
  var lib = loadFromPath(path);
  type testType = proc(): void;
  const testProc = try! lib.retrieve("test1", testType);
  testProc();
}
