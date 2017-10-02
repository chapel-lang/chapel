var r = 1..10;
writeln(r.alignment);

var r2 = 1..10 align 3;
writeln(r2.alignment);

var r3 = r align 3;
writeln(r3.alignment);

var r4 = r align 3 by 1;
writeln(r4.alignment);

var r5 = r by 1 align 3;
writeln(r5.alignment);

var r6 = r by 2 align 1;
writeln(r6.alignment);

var r7 = r by 2 align 3;
writeln(r7.alignment);

var r8 = r align 1 by 2;
writeln(r8.alignment);

var r9 = r align 3 by 2;
writeln(r9.alignment);
