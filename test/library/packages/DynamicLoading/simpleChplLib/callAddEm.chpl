use DynamicLoading, ChplConfig;

config param ext = if CHPL_TARGET_PLATFORM=="darwin" then ".dylib" else ".so";

const bin = try! binary.load("./lib/libaddEm" + ext);

const myAddEm = bin.retrieve("addEm", proc(x: int, y: int): int);

for loc in Locales do
  on loc do
    writeln(myAddEm(42, here.id));

// Explicit 'exit' call is no longer needed - teardown happens properly.
// exit(0);
