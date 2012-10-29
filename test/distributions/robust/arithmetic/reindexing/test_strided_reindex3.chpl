use driver_domains;

const D: domain(1,int,true) dmapped Dist1D = {Space1};

var A: [D] int;

var AA: [45..55 by 2] => A[49..54];

AA = 1;
writeln(A[45..55]);
