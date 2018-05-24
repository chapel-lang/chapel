class C {
  var x: int;
}

var A: [1..5] unmanaged C;

for i in 1..5 do
  A(i) = new unmanaged C(i);

writeln(A);

var B: [1..5] int;

B = A.x;

writeln(B);

for i in 1..5 do
  delete A(i);
