// typeInference.chpl
//
// Show how types are inferred from the range bounds.

writeln((..).type:string);
writeln((0..).type:string);
writeln((..10).type:string);
writeln((0..10).type:string);

writeln((-1..).type:string);
writeln((..-10).type:string);
writeln((-10..10).type:string);

writeln((-2147483647..).type:string);
writeln((-2147483648..).type:string);
writeln((-2147483649..).type:string);
writeln((..2147483646).type:string);
writeln((..2147483647).type:string);
writeln((..2147483648).type:string);
writeln((0..2147483646).type:string);
writeln((0..2147483647).type:string);
writeln((0..2147483648).type:string);
writeln((-2147483648..2147483646).type:string);
writeln((-2147483648..2147483647).type:string);
writeln((-2147483648..2147483648).type:string);
writeln((-2147483647..2147483647).type:string);
writeln((-2147483648..2147483647).type:string);
writeln((-2147483649..2147483647).type:string);
writeln((..4294967294).type:string);
writeln((..4294967295).type:string);
writeln((..4294967296).type:string);
writeln((-1..4294967294).type:string);
writeln((-1..4294967295).type:string);
writeln((-1..4294967296).type:string);
writeln((0..4294967294).type:string);
writeln((0..4294967295).type:string);
writeln((0..4294967296).type:string);
writeln((1..4294967294).type:string);
writeln((1..4294967295).type:string);
writeln((1..4294967296).type:string);

param minS = -9223372036854775807-1; // min(int(64));
param maxS =  9223372036854775807; // max(int(64));
param maxU = 18446744073709551615; // max(uint(64));

writeln(((minS+1)..).type:string);
writeln((minS..).type:string);
writeln(((minS-1)..).type:string);
writeln((..(maxS-1)).type:string);
writeln((..maxS).type:string);
writeln((..(maxS+1)).type:string);
writeln((0..(maxS-1)).type:string);
writeln((0..maxS).type:string);
writeln((0..(maxS+1)).type:string);
writeln((minS..(maxS-1)).type:string);
writeln((minS..maxS).type:string);
writeln((minS..(maxS+1)).type:string);
writeln(((minS+1)..maxS).type:string);
writeln((minS..maxS).type:string);
writeln(((minS-1)..maxS).type:string);
writeln((..(maxU-1)).type:string);
writeln((..maxU).type:string);
writeln((..(maxU+1)).type:string);
// These cases are correctly rejected by the compiler.
//writeln((-1..(maxU-1)).type:string);
//writeln((-1..maxU).type:string);
//writeln((-1..(maxU-1)).type:string);
writeln((0..(maxU-1)).type:string);
writeln((0..maxU).type:string);
writeln((0..(maxU+1)).type:string);
writeln((1..(maxU-1)).type:string);
writeln((1..maxU).type:string);
writeln((1..(maxU+1)).type:string);

