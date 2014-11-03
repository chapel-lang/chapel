// Example of the Jacobi algorithm in Chapel

param M = 4;
param N = 4;
/*
config const THRESHOLD = 0.00001;
// config const THRESHOLD: = 0.00001; // generates "0.0 = 0.0001"
// param MAXDELTAPOSSIBLE:real = 1.0;// error:value not known at compile time?
*/
param THRESHOLD = 0.00001;


// would like to use subdomains for D, northOfD, southOfD, westOfD, eastOfD
var outerD: domain(2) = {0..M+1, 0..N+1};
var D: domain(2) = {1..M, 1..N};
var northOfD: domain(2) = {0..0,     1..N};  // 0..0
var southOfD: domain(2) = {M+1..M+1, 1..N};
var westOfD : domain(2) = {1..M, 0..0};
var eastOfD : domain(2) = {1..M, N+1..N+1};  // N+1..N+1

var World:[D] real;
var Temp:[outerD] real;

// starting conditions
[e in D] Temp(e) = 0.0;
[e in northOfD] Temp(e) = 0.25;
[e in southOfD] Temp(e) = 1.09;
[e in westOfD]  Temp(e) = 0.59;
[e in eastOfD]  Temp(e) = 0.63;


var moving: atomic int;
do {
  moving.write(0);
  forall (i,j) in D do {                       // calc new World
    World(i,j) = (Temp(i-1,j) + Temp(i+1,j) + Temp(i,j-1) + Temp(i,j+1)) / 4.0;
    moving.add(abs( World(i,j) - Temp(i,j)) > THRESHOLD);
  }
  [e in D] Temp(e) = World(e);               // copy back World->Temp
 } while (moving.read());

writeln( World);
