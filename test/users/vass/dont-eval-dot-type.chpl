proc test() { writeln("in test()"); return "1"; }
writeln(typeToString(test().type));
