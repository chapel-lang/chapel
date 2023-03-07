config param n = 10;
config param m = 20;

const D4D0 = {1..3, 4..6};
const D4D1 = {7..9, 1..5 by -2};
const D4D2 = {1..3, 5..9 by -2};

var A: [D4D0] int;
writeln(A.reindex(D4D1));
writeln(A.reindex(7..11 by -2, 1..3));

var B: [D4D1] int;
writeln(B.reindex(D4D2));
writeln(B.reindex(1..3, 5..9 by -2));
