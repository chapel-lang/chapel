// typeInference.chpl
//
// Show how types are inferred from the range bounds.

writeln(typeToString((..).type));
writeln(typeToString((0..).type));
writeln(typeToString((..10).type));
writeln(typeToString((0..10).type));

writeln(typeToString((-1..).type));
writeln(typeToString((..-10).type));
writeln(typeToString((-10..10).type));

writeln(typeToString((-2147483647..).type));
writeln(typeToString((-2147483648..).type));
writeln(typeToString((-2147483649..).type));
writeln(typeToString((..2147483646).type));
writeln(typeToString((..2147483647).type));
writeln(typeToString((..2147483648).type));
writeln(typeToString((0..2147483646).type));
writeln(typeToString((0..2147483647).type));
writeln(typeToString((0..2147483648).type));
writeln(typeToString((-2147483648..2147483646).type));
writeln(typeToString((-2147483648..2147483647).type));
writeln(typeToString((-2147483648..2147483648).type));
writeln(typeToString((-2147483647..2147483647).type));
writeln(typeToString((-2147483648..2147483647).type));
writeln(typeToString((-2147483649..2147483647).type));
writeln(typeToString((..4294967294).type));
writeln(typeToString((..4294967295).type));
writeln(typeToString((..4294967296).type));
writeln(typeToString((-1..4294967294).type));
writeln(typeToString((-1..4294967295).type));
writeln(typeToString((-1..4294967296).type));
writeln(typeToString((0..4294967294).type));
writeln(typeToString((0..4294967295).type));
writeln(typeToString((0..4294967296).type));
writeln(typeToString((1..4294967294).type));
writeln(typeToString((1..4294967295).type));
writeln(typeToString((1..4294967296).type));

param minS = -9223372036854775807-1; // min(int(64));
param maxS =  9223372036854775807; // max(int(64));
param maxU = 18446744073709551615; // max(uint(64));

writeln(typeToString(((minS+1)..).type));
writeln(typeToString((minS..).type));
writeln(typeToString(((minS-1)..).type));
writeln(typeToString((..(maxS-1)).type));
writeln(typeToString((..maxS).type));
writeln(typeToString((..(maxS+1)).type));
writeln(typeToString((0..(maxS-1)).type));
writeln(typeToString((0..maxS).type));
writeln(typeToString((0..(maxS+1)).type));
writeln(typeToString((minS..(maxS-1)).type));
writeln(typeToString((minS..maxS).type));
writeln(typeToString((minS..(maxS+1)).type));
writeln(typeToString(((minS+1)..maxS).type));
writeln(typeToString((minS..maxS).type));
writeln(typeToString(((minS-1)..maxS).type));
writeln(typeToString((..(maxU-1)).type));
writeln(typeToString((..maxU).type));
writeln(typeToString((..(maxU+1)).type));
// These cases are correctly rejected by the compiler.
//writeln(typeToString((-1..(maxU-1)).type));
//writeln(typeToString((-1..maxU).type));
//writeln(typeToString((-1..(maxU-1)).type));
writeln(typeToString((0..(maxU-1)).type));
writeln(typeToString((0..maxU).type));
writeln(typeToString((0..(maxU+1)).type));
writeln(typeToString((1..(maxU-1)).type));
writeln(typeToString((1..maxU).type));
writeln(typeToString((1..(maxU+1)).type));

