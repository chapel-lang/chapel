class C {
  var x: int;
}

var A: [1..5] C;

for i in 1..5 do
  A(i) = new C(i);

writeln(A);

var B: [1..5] int;

B = A.x;

writeln(B);

for i in 1..5 do
  delete A(i);
