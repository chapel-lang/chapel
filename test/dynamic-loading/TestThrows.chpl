use DynamicLoading;

import ChplConfig.CHPL_TARGET_PLATFORM;

proc libSuffix() param do
  return if CHPL_TARGET_PLATFORM == "darwin" then ".dylib" else ".so";
proc libName(name, prefix="", suffix=libSuffix()) {
  return prefix + name + suffix;
}
config const path = "./lib/";

proc main() {
  var bin = binary.load(libName("libTestThrowsLib", path));
  var callThrows = bin.retrieve("callMe", proc(): void);
  callThrows();
}
