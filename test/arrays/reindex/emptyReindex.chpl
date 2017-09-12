var A: [1..0] real;
var A2: [1..0, 1..10] real;
var A3: [1..10, 1..0] real;

var B = A.reindex(10..9);
var B2 = A2.reindex(10..9, 10..19);
var B3 = A3.reindex(10..19, 10..9);

writeln(B);
writeln(B.domain);

writeln(B2);
writeln(B2.domain);

writeln(B3);
writeln(B3.domain);
