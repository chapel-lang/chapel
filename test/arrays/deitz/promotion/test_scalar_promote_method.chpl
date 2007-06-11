class C {
  var x: int;
}

var A: [1..5] C;

for i in 1..5 do
  A(i) = C(i);

writeln(A);

var B: [1..5] int;

B = A.x;

writeln(B);
