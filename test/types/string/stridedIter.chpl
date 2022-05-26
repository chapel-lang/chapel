var valRange = 1..20 by 2;
var s= "çş"*5;

writeln("range, string");
for i in zip(valRange, s) do writeln(i);
writeln();

writeln("string, range");
for i in zip(s, valRange) do writeln(i);
writeln();
