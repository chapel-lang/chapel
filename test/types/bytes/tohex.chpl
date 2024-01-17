var testB: bytes = b"\x00\x01\x23\x45\x67\x89\xab\xcd\xef\xfe";

writeln("uppercase bytes");
printTypeAndValue(testB.toHexadecimal(uppercase=true, resultType=bytes)); 
writeln("lowercase bytes");
printTypeAndValue(testB.toHexadecimal(uppercase=false, resultType=bytes)); 
writeln("default case bytes");
printTypeAndValue(testB.toHexadecimal(resultType=bytes)); 

writeln("uppercase string");
printTypeAndValue(testB.toHexadecimal(uppercase=true, resultType=string)); 
writeln("lowercase string");
printTypeAndValue(testB.toHexadecimal(uppercase=false, resultType=string)); 
writeln("default case string");
printTypeAndValue(testB.toHexadecimal(resultType=string)); 

writeln("default case default return type");
printTypeAndValue(testB.toHexadecimal()); 

proc printTypeAndValue(x) {
  writeln(x, " : ", x.type:string);
}
