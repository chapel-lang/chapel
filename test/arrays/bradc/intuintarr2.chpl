config const n = 3;

const DS : domain(1,int) = {1..n};
const DU : domain(1,uint) = {1..n:uint};

var AS: [DS] real;
var AU: [DU] real;

writeln("AS is: ", AS);
writeln("AU is: ", AU);

