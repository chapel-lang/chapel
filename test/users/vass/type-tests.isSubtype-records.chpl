record R1{}  record R2:R1{}  record R3:R1{}  record R4:R2{}

compilerWarning("isSubtype - records");
compilerWarning("R1, R1  ", isSubtype(R1,R1):string);
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

compilerWarning("isProperSubtype - records");
compilerWarning("R1, R1  ", isProperSubtype(R1,R1):string);
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
compilerError("done"); // no need for a C compile
