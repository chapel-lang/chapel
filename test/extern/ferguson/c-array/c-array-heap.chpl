use CTypes;
var ptr = allocate(c_array(int, 5), 1);

ref arr = ptr.deref();

for i in 0..#5 do
  arr[i] = i;

writeln(ptr.deref());

deallocate(ptr);
