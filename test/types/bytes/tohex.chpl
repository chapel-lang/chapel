var testB: bytes = b"\x00\x01\x23\x45\x67\x89\xab\xcd\xef\xfe";

writeln("uppercase");
writeln(testB.toHexadecimal(uppercase=true)); 
writeln("lowercase");
writeln(testB.toHexadecimal(uppercase=false)); 
writeln("default case");
writeln(testB.toHexadecimal()); 
