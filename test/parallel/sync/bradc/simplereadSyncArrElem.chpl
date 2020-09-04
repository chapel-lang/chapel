var flag1: [1..3] sync bool;

writeln("First write/read pair");
flag1(1).writeEF(true);
flag1(1).readFE();
if (flag1(1).isFull) then halt("Should never get here!");

writeln("Second write/read pair");
flag1(1).writeEF(true);
flag1(1).readFE();
if (flag1(1).isFull) then halt("Should never get here!");

writeln("Wrote and read and wrote again flag1(1)");
