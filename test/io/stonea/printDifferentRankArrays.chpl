use IO, ChplFormat;

var A: [1..2] string = "hi";
var B: [1..2, 1..2] string = "hi";

writeln(A);
writeln(B);

var chplOut = stdout.withSerializer(chplSerializer);

chplOut.writef("%?\n", A);
chplOut.writef("%?\n", B);
