var s1:((int, real),real);
var s2:(1*(int, real),real);
var s3:(((int, real)),real);

writeln(typeToString(s1.type));   //output ((int, real),real)
writeln(typeToString(s2.type));   //output (((int, real)),real)
writeln(typeToString(s3.type));   //output ((int, real),real)
