var D: domain(opaque);
var A: [D] index(D);

var root = D.create();
A(root) = D.create();
A(A(root)) = D.create();
A(A(A(root))) = root;

var B: [D] int;

var tmp = root;
var i = 1;
while B(tmp) == 0 {
  B(tmp) = i;
  i += 1;
  tmp = A(tmp);
}

writeln(B.sorted());
