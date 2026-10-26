use DynamicLoading, ChplConfig;

config param ext = if CHPL_TARGET_PLATFORM=="darwin" then ".dylib" else ".so";

const bin = try! binary.load("./lib/libaddEm" + ext);

try {
  // NOTE: The name "addem" is intentionally incorrect!
  const myAddEm = bin.retrieve("addem", proc(x: int, y: int): int);
  writeln(myAddEm(42, 45));
} catch e : DynLoadError {
  assert(e.message().find("addem") != -1);
  writeln('Caught error!');
} catch e {
  writeln(e);
}

// Explicit 'exit' call is no longer needed - teardown happens properly.
// exit(0);
