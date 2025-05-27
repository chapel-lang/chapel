use DynamicLoading;

// A binary may or may not exist at this path.
const path = './TestBinary.so';

// If loading fails an error will be issued and the 'try!' will halt.
const bin = try! binary.load(path);

// Declare the type of the procedure.
type P = proc(): void;

// Retrieve a procedure named 'foo' from 'bin' with the type 'P'.
const p1 = try! bin.retrieve('foo', P);

// Call the procedure.
p1();

