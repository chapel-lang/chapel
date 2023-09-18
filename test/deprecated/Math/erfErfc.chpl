var a: real = -100;
writeln("a = ", a);
writeln("erf(a) = ", erf(a)); // Should trigger the deprecation message

writeln("a = ", a);
writeln("erfc(a) = ", erfc(a)); // Should trigger the deprecation message
