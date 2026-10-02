use DynamicLibrary, ChplConfig;

config param ext = if CHPL_TARGET_PLATFORM=="darwin" then ".dylib" else ".so";

const lib = try! loadFromPath("./lib/libaddEm" + ext);

const myAddEm = lib.retrieve("addEm", proc(x: int, y: int): int);

writeln(myAddEm(42, 45));
