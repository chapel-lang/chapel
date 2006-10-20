config const n = 3;

const DS : domain(1,int) = [1..n];
const DU : domain(1,uint) = [1..n:uint];

var AS: [DS] float;
var AU: [DU] float;

writeln("AS is: ", AS);
writeln("AU is: ", AU);

