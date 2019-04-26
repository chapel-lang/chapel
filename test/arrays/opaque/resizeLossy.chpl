use Sort;

var D: domain(opaque);
var A: [D] real;
var B: [1..100] real;

for i in 1..100 {
  const ind = D.create();
  A[ind] = i:real;
}

B = A;

sort(B);

writeln(B);
