use CPtr;
var ptr = c_malloc(c_array(int, 5), 1);

ref arr = ptr.deref();

for i in 0..#5 do
  arr[i] = i;

writeln(ptr.deref());

c_free(ptr);
