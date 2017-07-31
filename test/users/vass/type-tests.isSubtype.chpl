var D1 = {1..3};
var D2 = {2..4};

var AA1: [D1] int;
var AA2: [D2] int;

var q1:int, q2:real, q3:real;

class C1{}  class C2:C1{}  class C3:C1{}  class C4:C2{}

record R1{} // record R2:R1{}  record R3:R1{}  record R4:R2{} // no record inheritance at the moment.  Output preserved in type-tests.isSubtype-record.good

union U1 { var x,y: int; }  union U2 { var x,y: int; }  // no union inheritance at the moment

compilerWarning("isSubtype - scalars");
compilerWarning("int, real   ", isSubtype(q1.type, q2.type):string);
compilerWarning("real, real  ", isSubtype(q2.type, q3.type):string);

compilerWarning("isSubtype - domains and arrays");
compilerWarning("D1, D2            ", isSubtype(D1.type, D2.type):string);
compilerWarning("{D1,D2}._value    ", isSubtype(D1._value.type, D2._value.type):string);
compilerWarning("AA1, AA2          ", isSubtype(AA1.type, AA2.type):string);
compilerWarning("{AA1,AA2}.domain  ", isSubtype(AA1.domain.type, AA2.domain.type):string);
compilerWarning("{AA1,AA2}._value  ", isSubtype(AA1._value.type, AA2._value.type):string);

compilerWarning("isSubtype - classes");
compilerWarning("C1, C1  ", isSubtype(C1,C1):string);
compilerWarning("C1, C2  ", isSubtype(C1,C2):string);
compilerWarning("C1, C3  ", isSubtype(C1,C3):string);
compilerWarning("C1, C4  ", isSubtype(C1,C4):string);
compilerWarning("C2, C1  ", isSubtype(C2,C1):string);
compilerWarning("C2, C2  ", isSubtype(C2,C2):string);
compilerWarning("C2, C3  ", isSubtype(C2,C3):string);
compilerWarning("C2, C4  ", isSubtype(C2,C4):string);
compilerWarning("C3, C1  ", isSubtype(C3,C1):string);
compilerWarning("C3, C2  ", isSubtype(C3,C2):string);
compilerWarning("C3, C3  ", isSubtype(C3,C3):string);
compilerWarning("C3, C4  ", isSubtype(C3,C4):string);
compilerWarning("C4, C1  ", isSubtype(C4,C1):string);
compilerWarning("C4, C2  ", isSubtype(C4,C2):string);
compilerWarning("C4, C3  ", isSubtype(C4,C3):string);
compilerWarning("C4, C4  ", isSubtype(C4,C4):string);

compilerWarning("isSubtype - records");
compilerWarning("R1, R1  ", isSubtype(R1,R1):string);
/*
compilerWarning("R1, R2  ", isSubtype(R1,R2):string);
compilerWarning("R1, R3  ", isSubtype(R1,R3):string);
compilerWarning("R1, R4  ", isSubtype(R1,R4):string);
compilerWarning("R2, R1  ", isSubtype(R2,R1):string);
compilerWarning("R2, R2  ", isSubtype(R2,R2):string);
compilerWarning("R2, R3  ", isSubtype(R2,R3):string);
compilerWarning("R2, R4  ", isSubtype(R2,R4):string);
compilerWarning("R3, R1  ", isSubtype(R3,R1):string);
compilerWarning("R3, R2  ", isSubtype(R3,R2):string);
compilerWarning("R3, R3  ", isSubtype(R3,R3):string);
compilerWarning("R3, R4  ", isSubtype(R3,R4):string);
compilerWarning("R4, R1  ", isSubtype(R4,R1):string);
compilerWarning("R4, R2  ", isSubtype(R4,R2):string);
compilerWarning("R4, R3  ", isSubtype(R4,R3):string);
compilerWarning("R4, R4  ", isSubtype(R4,R4):string);
*/

compilerWarning("isSubtype - mixing up");
compilerWarning("real,     D1.type  ", isSubtype(real,D1.type):string);
compilerWarning("real,     AA1.type ", isSubtype(real,AA1.type):string);
compilerWarning("real,     C1       ", isSubtype(real,C1):string);
compilerWarning("real,     R1       ", isSubtype(real,R1):string);
compilerWarning("real,     U1       ", isSubtype(real,U1):string);
compilerWarning("D1.type,  real     ", isSubtype(D1.type,real):string);
compilerWarning("D1.type,  D1.type  ", isSubtype(D1.type,D1.type):string);
compilerWarning("D1.type,  AA1.type ", isSubtype(D1.type,AA1.type):string);
compilerWarning("D1.type,  C1       ", isSubtype(D1.type,C1):string);
compilerWarning("D1.type,  R1       ", isSubtype(D1.type,R1):string);
compilerWarning("D1.type,  U1       ", isSubtype(D1.type,U1):string);
compilerWarning("AA1.type, real     ", isSubtype(AA1.type,real):string);
compilerWarning("AA1.type, D1.type  ", isSubtype(AA1.type,D1.type):string);
compilerWarning("AA1.type, AA1.type ", isSubtype(AA1.type,AA1.type):string);
compilerWarning("AA1.type, C1       ", isSubtype(AA1.type,C1):string);
compilerWarning("AA1.type, R1       ", isSubtype(AA1.type,R1):string);
compilerWarning("AA1.type, U1       ", isSubtype(AA1.type,U1):string);
compilerWarning("C1,       real     ", isSubtype(C1,real):string);
compilerWarning("C1,       D1.type  ", isSubtype(C1,D1.type):string);
compilerWarning("C1,       AA1.type ", isSubtype(C1,AA1.type):string);
compilerWarning("C1,       R1       ", isSubtype(C1,R1):string);
compilerWarning("C1,       U1       ", isSubtype(C1,U1):string);
compilerWarning("R1,       real     ", isSubtype(R1,real):string);
compilerWarning("R1,       D1.type  ", isSubtype(R1,D1.type):string);
compilerWarning("R1,       AA1.type ", isSubtype(R1,AA1.type):string);
compilerWarning("R1,       C1       ", isSubtype(R1,C1):string);
compilerWarning("R1,       U1       ", isSubtype(R1,U1):string);
compilerWarning("U1,       real     ", isSubtype(U1,real):string);
compilerWarning("U1,       D1.type  ", isSubtype(U1,D1.type):string);
compilerWarning("U1,       AA1.type ", isSubtype(U1,AA1.type):string);
compilerWarning("U1,       C1       ", isSubtype(U1,C1):string);
compilerWarning("U1,       R1       ", isSubtype(U1,R1):string);

compilerWarning("isProperSubtype - scalars");
compilerWarning("int, real   ", isProperSubtype(q1.type, q2.type):string);
compilerWarning("real, real  ", isProperSubtype(q2.type, q3.type):string);

compilerWarning("isProperSubtype - domains and arrays");
compilerWarning("D1, D2            ", isProperSubtype(D1.type, D2.type):string);
compilerWarning("{D1,D2}._value    ", isProperSubtype(D1._value.type, D2._value.type):string);
compilerWarning("AA1, AA2          ", isProperSubtype(AA1.type, AA2.type):string);
compilerWarning("{AA1,AA2}.domain  ", isProperSubtype(AA1.domain.type, AA2.domain.type):string);
compilerWarning("{AA1,AA2}._value  ", isProperSubtype(AA1._value.type, AA2._value.type):string);

compilerWarning("isProperSubtype - classes");
compilerWarning("C1, C1  ", isProperSubtype(C1,C1):string);
compilerWarning("C1, C2  ", isProperSubtype(C1,C2):string);
compilerWarning("C1, C3  ", isProperSubtype(C1,C3):string);
compilerWarning("C1, C4  ", isProperSubtype(C1,C4):string);
compilerWarning("C2, C1  ", isProperSubtype(C2,C1):string);
compilerWarning("C2, C2  ", isProperSubtype(C2,C2):string);
compilerWarning("C2, C3  ", isProperSubtype(C2,C3):string);
compilerWarning("C2, C4  ", isProperSubtype(C2,C4):string);
compilerWarning("C3, C1  ", isProperSubtype(C3,C1):string);
compilerWarning("C3, C2  ", isProperSubtype(C3,C2):string);
compilerWarning("C3, C3  ", isProperSubtype(C3,C3):string);
compilerWarning("C3, C4  ", isProperSubtype(C3,C4):string);
compilerWarning("C4, C1  ", isProperSubtype(C4,C1):string);
compilerWarning("C4, C2  ", isProperSubtype(C4,C2):string);
compilerWarning("C4, C3  ", isProperSubtype(C4,C3):string);
compilerWarning("C4, C4  ", isProperSubtype(C4,C4):string);

compilerWarning("isProperSubtype - records");
compilerWarning("R1, R1  ", isProperSubtype(R1,R1):string);
/*
compilerWarning("R1, R2  ", isProperSubtype(R1,R2):string);
compilerWarning("R1, R3  ", isProperSubtype(R1,R3):string);
compilerWarning("R1, R4  ", isProperSubtype(R1,R4):string);
compilerWarning("R2, R1  ", isProperSubtype(R2,R1):string);
compilerWarning("R2, R2  ", isProperSubtype(R2,R2):string);
compilerWarning("R2, R3  ", isProperSubtype(R2,R3):string);
compilerWarning("R2, R4  ", isProperSubtype(R2,R4):string);
compilerWarning("R3, R1  ", isProperSubtype(R3,R1):string);
compilerWarning("R3, R2  ", isProperSubtype(R3,R2):string);
compilerWarning("R3, R3  ", isProperSubtype(R3,R3):string);
compilerWarning("R3, R4  ", isProperSubtype(R3,R4):string);
compilerWarning("R4, R1  ", isProperSubtype(R4,R1):string);
compilerWarning("R4, R2  ", isProperSubtype(R4,R2):string);
compilerWarning("R4, R3  ", isProperSubtype(R4,R3):string);
compilerWarning("R4, R4  ", isProperSubtype(R4,R4):string);
*/

compilerWarning("isProperSubtype - mixing up");
compilerWarning("real,     D1.type  ", isProperSubtype(real,D1.type):string);
compilerWarning("real,     AA1.type ", isProperSubtype(real,AA1.type):string);
compilerWarning("real,     C1       ", isProperSubtype(real,C1):string);
compilerWarning("real,     R1       ", isProperSubtype(real,R1):string);
compilerWarning("real,     U1       ", isProperSubtype(real,U1):string);
compilerWarning("D1.type,  real     ", isProperSubtype(D1.type,real):string);
compilerWarning("D1.type,  D1.type  ", isProperSubtype(D1.type,D1.type):string);
compilerWarning("D1.type,  AA1.type ", isProperSubtype(D1.type,AA1.type):string);
compilerWarning("D1.type,  C1       ", isProperSubtype(D1.type,C1):string);
compilerWarning("D1.type,  R1       ", isProperSubtype(D1.type,R1):string);
compilerWarning("D1.type,  U1       ", isProperSubtype(D1.type,U1):string);
compilerWarning("AA1.type, real     ", isProperSubtype(AA1.type,real):string);
compilerWarning("AA1.type, D1.type  ", isProperSubtype(AA1.type,D1.type):string);
compilerWarning("AA1.type, AA1.type ", isProperSubtype(AA1.type,AA1.type):string);
compilerWarning("AA1.type, C        ", isProperSubtype(AA1.type,C1):string);
compilerWarning("AA1.type, R        ", isProperSubtype(AA1.type,R1):string);
compilerWarning("AA1.type, U1       ", isProperSubtype(AA1.type,U1):string);
compilerWarning("C1,       real     ", isProperSubtype(C1,real):string);
compilerWarning("C1,       D1.type  ", isProperSubtype(C1,D1.type):string);
compilerWarning("C1,       AA1.type ", isProperSubtype(C1,AA1.type):string);
compilerWarning("C1,       R1       ", isProperSubtype(C1,R1):string);
compilerWarning("C1,       U1       ", isProperSubtype(C1,U1):string);
compilerWarning("R1,       real     ", isProperSubtype(R1,real):string);
compilerWarning("R1,       D1.type  ", isProperSubtype(R1,D1.type):string);
compilerWarning("R1,       AA1.type ", isProperSubtype(R1,AA1.type):string);
compilerWarning("R1,       C1       ", isProperSubtype(R1,C1):string);
compilerWarning("R1,       U1       ", isProperSubtype(R1,U1):string);
compilerWarning("U1,       real     ", isProperSubtype(U1,real):string);
compilerWarning("U1,       D1.type  ", isProperSubtype(U1,D1.type):string);
compilerWarning("U1,       AA1.type ", isProperSubtype(U1,AA1.type):string);
compilerWarning("U1,       C1       ", isProperSubtype(U1,C1):string);
compilerWarning("U1,       R1       ", isProperSubtype(U1,R1):string);

// unions
compilerWarning("U1, U2  ", isSubtype(U1,U2):string);
compilerWarning("U1, U2  ", isProperSubtype(U1,U2):string);

compilerError("done"); // no need for a C compile
