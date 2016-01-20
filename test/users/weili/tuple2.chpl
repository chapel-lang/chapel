var s1:((int, real),real);
var s2:(1*(int, real),real);
var s3:(((int, real)),real);

writeln(s1.type:string);   //output ((int, real),real)
writeln(s2.type:string);   //output (((int, real)),real)
writeln(s3.type:string);   //output ((int, real),real)
