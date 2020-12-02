class C {
  var a: int;
}

var A: [1..5] int;
var B: [1..5] unmanaged C?;

B = new unmanaged C(A);

writeln(B);

for b in B do
  delete b;
