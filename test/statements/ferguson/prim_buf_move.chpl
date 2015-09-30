
var buf = c_calloc(int(8), 20):c_void_ptr;
// assignment (vs move) between c_void_ptr is not working now


var x: int = 100;
var y: int;

// try copying into the buffer.
__primitive("move to buf", buf, x);

// try copying from the buffer
__primitive("move from buf", y, buf);

writeln("x=", x);
writeln("y=", y);

assert(x == y);

