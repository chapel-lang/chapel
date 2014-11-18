// inspired by test/types/range/hilde/count.chpl

param maxU2 = 2:uint**31;
param maxU2string:c_string = maxU2:c_string;
writeln(typeToString(maxU2.type));
writeln(maxU2string);
writeln(maxU2);
