const R1 = 1..2;
const R2 = 1..1;
const R3 = 1..0;
const R4 = 1..-1;

const R5 = 1..2 by -1;
const R6 = 1..1 by -1;
const R7 = 1..0 by -1;
const R8 = 1..-1 by -1;

writeln("R1.size = ", R1.size);
writeln("R2.size = ", R2.size);
writeln("R3.size = ", R3.size);
writeln("R4.size = ", R4.size);
writeln("R5.size = ", R5.size);
writeln("R6.size = ", R6.size);
writeln("R7.size = ", R7.size);
writeln("R8.size = ", R8.size);
