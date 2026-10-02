use DynamicLibrary, ChplConfig;

config param ext = if CHPL_TARGET_PLATFORM=="darwin" then ".dylib" else ".so";

const lib = try! loadFromPath("./lib/libaddEm" + ext);

const myAddEm = lib.retrieve("addEm", proc(x: int, y: int): int);

for loc in Locales do
  on loc do
    writeln(myAddEm(42, here.id));

// Explicit 'exit' call is no longer needed - teardown happens properly.
// exit(0);
