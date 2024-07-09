var s = "my string";
var b = b"my bytes";

var cs= s.c_str();
var cb = b.c_str();

assert(string.createCopyingBuffer(cs) == s);
assert(bytes.createCopyingBuffer(cb) == b);
