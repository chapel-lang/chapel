proc test() { writeln("in test()"); return "1":string; }
writeln(typeToString(test().type));
