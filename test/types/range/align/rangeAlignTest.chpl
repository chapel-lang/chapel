var r = 1..10;
writeln(r.alignment);

var r1 = r by 1;
writeln(r1.alignment);

var r2 = r align 3;
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

var r10 = ..;
writeln(r10.alignment);

var r11 = 1..;
writeln(r11.alignment);

var r12 = ..10;
writeln(r12.alignment);

var r13 = .. align 1 by 2;
writeln(r13.alignment);

var r14 = ..10 align 1 by 2;
writeln(r14.alignment);

var r15 = 1.. by 2;
writeln(r15.alignment);

var r16 = 1.. align 2 by 2;
writeln(r16.alignment);

var r17 = .. by 2;
writeln(r17.alignment);

var r18 = .. by 2 align 2;
writeln(r18.alignment);

var r19 = ..10 by 2 align 1;
writeln(r19.alignment);
