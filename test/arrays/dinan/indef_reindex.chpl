var A: [0..99] real;

var BP => A[10..19];
var B: {0..} => BP;

writeln(A.domain, BP.domain, B.domain);
