record R1 {
    param b = true;
}

record R2 {
    param b;
}

writeln(R1(?) : string);
writeln(R1 : string);
writeln(R1(true) : string);
writeln(R1(false) : string);
writeln(R2(?) : string);
writeln(R2 : string);
writeln(R2(true) : string);
writeln(R2(false) : string);

writeln("----");

type t1 = R1(?);
writeln(t1 : string);
type t2 = R1;
writeln(t2 : string);
type t3 = R1(true);
writeln(t3 : string);
type t4 = R1(false);
writeln(t4 : string);
type t5 = R2(?);
writeln(t5 : string);
type t6 = R2;
writeln(t6 : string);
type t7 = R2(true);
writeln(t7 : string);
type t8 = R2(false);
writeln(t8 : string);
