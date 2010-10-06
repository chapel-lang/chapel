config type myType = int;
param n = 10;
config param myParam: myType = 10*n;

writeln(myParam);

