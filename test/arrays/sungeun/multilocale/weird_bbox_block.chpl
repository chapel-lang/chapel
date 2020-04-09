const n = max(int);
const m = min(int);
config const bbLow = -5;
config const bbSize = 1;
config const aLow = 255;

use BlockDist;

const bbox = {bbLow..bbLow+bbSize};
const minSpace = {m..m+1};
const maxSpace = {n-1..n};
const aSpace = {aLow..aLow+1};

const BlockD: domain(1) dmapped new dmap(new Block(rank=1,boundingBox=bbox)) = aSpace;
const BlockmaxD: domain(1) dmapped new dmap(new Block(rank=1,boundingBox=bbox)) = maxSpace;
const BlockminD: domain(1) dmapped new dmap(new Block(rank=1,boundingBox=bbox)) = minSpace;

var A: [BlockD] int;
A = -1;
[i in BlockD] A[i] = -2;
for i in BlockD do
  A[i] = -3;
forall i in BlockD do
  A[i] = -4;
coforall i in BlockD do
  A[i] = -5;
writeln("SUCCESS");

var BAmin: [BlockminD] int;
BAmin = -1;
[i in BlockminD] BAmin[i] = -2;
for i in BlockminD do
  BAmin[i] = -3;
forall i in BlockminD do
  BAmin[i] = -4;
coforall i in BlockminD do
  BAmin[i] = -5;
writeln("SUCCESS");

var BAmax: [BlockmaxD] int;
BAmax = -1;
[i in BlockmaxD] BAmax[i] = -2;
for i in BlockmaxD do
  BAmax[i] = -3;
forall i in BlockmaxD do
  BAmax[i] = -4;
coforall i in BlockmaxD do
  BAmax[i] = -5;
writeln("SUCCESS");
