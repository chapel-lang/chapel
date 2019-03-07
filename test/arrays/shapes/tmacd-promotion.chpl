config const n = 2;
var b: [0..n+1, 0..n+1, 0..1] int;

const N = b[0..n-1, 0..n-1, 0];
writeln("====== N ======");
writeln(N);
writeln(N.type:string);

const NN = b[0..n-1, 0..n-1, 0] + b[0..n-1, 2..n+1, 0];
writeln("====== NN =====");
writeln(NN);
writeln(NN.type:string); 
