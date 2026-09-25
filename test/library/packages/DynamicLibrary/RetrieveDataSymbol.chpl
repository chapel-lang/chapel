use DynamicLoading;
use CTypes;

// A binary may or may not exist at this path.
const path = './TestCLibraryToLoad.so';

// If loading fails an error will be issued and the 'try!' will halt.
const bin = try! binary.load(path);

type T = c_int;

// Should be a compiler error, this is not supported yet.
ref t1 = try! bin.retrieve('num', T);

