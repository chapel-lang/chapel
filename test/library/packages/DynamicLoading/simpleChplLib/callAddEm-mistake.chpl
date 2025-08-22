use DynamicLoading, ChplConfig;

config param ext = if CHPL_TARGET_PLATFORM=="darwin" then ".dylib" else ".so";

const bin = try! binary.load("./lib/libaddEm" + ext);

const myAddEm = bin.retrieve("addem", proc(x: int, y: int): int);

writeln(myAddEm(42, 45));
