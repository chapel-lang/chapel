var D: domain(1) = {1..5};

var A: [D] int = 1..5;
var B = A;

writeln(A);
writeln(B);

D = {1..6};

writeln(A);
writeln(B);
