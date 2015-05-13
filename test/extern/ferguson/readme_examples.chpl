var i:c_int;
var i_ptr = c_ptrTo(i); // now i_ptr has type c_ptr(c_int) == int* in C
writeln(typeToString(i_ptr.type));

var cArray = c_calloc(c_int, 10);
assert(cArray != nil);
assert(! (cArray == nil));
for i in 0..#10 {
  cArray[i] = i:c_int;
}
// c_ptr.deref() always refers to the first element.
cArray.deref() = 17;
for i in 0..#10 {
  writeln(cArray[i]);
}
c_free(cArray);
cArray = nil;

// both of these correspond to void fn(int* x)
extern proc byRef(ref x:c_int);
extern proc byPtr(x:c_ptr(c_int));

var x:c_int = 7;
byRef(x); // ref argument intent allows the variable to be passed directly
byPtr(c_ptrTo(x)); // c_ptr argument must be constructed explicitly

// both of these correspond to void fn(const char* x)
extern proc byCString(x:c_string);

byCString("Hello".c_str());

