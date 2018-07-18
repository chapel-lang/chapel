use StencilDist;
use util;

config const n = 100;
const Small = {1..n/2,1..n/2};
const Big = {1..n,1..n};

var Space = Small dmapped Stencil(Small, fluff=(2,2), periodic=true);
var A : [Space] int;

// Resize the domain, causing the periodic info to be recomputed
Space = Big;

A = [(i,j) in Space] (i*Space.size)+j;
A.updateFluff();

verifyStencil(A);
writeln("Success!");
