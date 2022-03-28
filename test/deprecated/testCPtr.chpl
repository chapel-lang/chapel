use CPtr;
var x: c_ptr(int);
var y: c_void_ptr;
writeln((x, y));
var a, b: c_array(int, 10);
for i in 0..<10 do
  b[i] = i+3;
a = b;
writeln(a);
var p: c_ptr(int);
p = a;
writeln(p.deref());


