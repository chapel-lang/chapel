config param version = 1;
config const N = 10;

var aDom: domain = if version == 1 then 5.67 else 0..#N;
writeln(aDom, ": ", aDom.type:string);
aDom = {0..#2*N};
writeln(aDom, ": ", aDom.type:string);
