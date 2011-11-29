const R1 = 1..2;
const R2 = 1..1;
const R3 = 1..0;
const R4 = 1..-1;

const R5 = 1..2 by -1;
const R6 = 1..1 by -1;
const R7 = 1..0 by -1;
const R8 = 1..-1 by -1;

writeln("R1.length = ", R1.length);
writeln("R2.length = ", R2.length);
writeln("R3.length = ", R3.length);
writeln("R4.length = ", R4.length);
writeln("R5.length = ", R5.length);
writeln("R6.length = ", R6.length);
writeln("R7.length = ", R7.length);
writeln("R8.length = ", R8.length);
