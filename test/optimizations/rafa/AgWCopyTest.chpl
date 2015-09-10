use BlockDist;
use PrintComms;

config const m = 3;
config const n = 7;
config const o = 5;

config const initialize = true;
config const printOutput = false;
config const printTiming = false;

const shift = (123, 456, 789);

var Dist = new dmap(new Block({1..m,1..n,1..o}));

use Random, Time;

var Dom: domain(3) dmapped Dist = {1..m, 1..n, 1..o};

var Ref: [Dom] real;
var A: [Dom] real;
var B: [Dom] real;
{
  if initialize {
    fillRandom(B, 31415);
    fillRandom(Ref, 31415);
  }
  A = -1;
  var st = getCurrentTime();
  A=B;

  var dt = getCurrentTime()-st;
  for i in A.domain do
    if A[i] != Ref[i] then
      writeln("ERROR in whole array assignment: ", i);
  if printOutput then
    writeln("whole array assignment ", (m,n,o), ":\n", A);
  if printTiming then
    writeln("whole array assignment ", (m,n,o), ": ", dt);
}

var C: [Dom.translate(shift)] real;
{
  if initialize {
    fillRandom(C, 92653);
    fillRandom(Ref, 92653);
  }
  A = -1;
  var st = getCurrentTime();
  A=C;
  var dt = getCurrentTime()-st;
  for i in A.domain do
    if A[i] != Ref[i] then
      writeln("ERROR in whole array assignment (+ offset): ", i);
  if printOutput then
    writeln("whole array assignment (+ offset) ", (m,n,o), ":\n", A);
  if printTiming then
    writeln("whole array assignment (+ offset) ", (m,n,o), ": ", dt);
}

var D: [Dom.translate(-shift)] real;
{
  if initialize {
    fillRandom(D, 58979);
    fillRandom(Ref, 58979);
  }
  A = -1;
  var st = getCurrentTime();
  A=D;
  var dt = getCurrentTime()-st;
  for i in A.domain do
    if A[i] != Ref[i] then
      writeln("ERROR in whole array assignment (- offset): ", i);
  if printOutput then
    writeln("whole array assignment (- offset) ", (m,n,o), ":\n", A);
  if printTiming then
    writeln("whole array assignment (- offset) ", (m,n,o), ": ", dt);
}

// aliased arrays
var DomSlice = {1..m/2, 1..n, 1..o};
var Refa => Ref[DomSlice];
var Aa => A[DomSlice];
var Ba => B[DomSlice];
{
  if initialize then {
    fillRandom(Ba, 31415);
    fillRandom(Refa, 31415);
  }
  A = 0;
  Aa = -1;
  var st = getCurrentTime();
  Aa=Ba;
  var dt = getCurrentTime()-st;
  for i in Aa.domain do
    if Aa[i] != Refa[i] then
      writeln("ERROR in whole array assignment (alias): ", i);
  if printOutput then
    writeln("whole array assignment (alias) ", (m,n,o), ":\n", Aa);
  if printTiming then
    writeln("whole array assignment (alias) ", (m,n,o), ": ", dt);
}

var Ca => C[DomSlice.translate(shift)];
{
  if initialize {
    fillRandom(Ca, 92653);
    fillRandom(Refa, 92653);
  }
  A = 0;
  Aa = -1;
  var st = getCurrentTime();
  Aa=Ca;
  var dt = getCurrentTime()-st;
  for i in Aa.domain do
    if Aa[i] != Refa[i] then
      writeln("ERROR in whole array assignment (alias, + offset): ", i);
  if printOutput then
    writeln("whole array assignment (alias, + offset) ", (m,n,o), ":\n", Aa);
  if printTiming then
    writeln("whole array assignment (alias, + offset) ", (m,n,o), ": ", dt);
}

var Da => D[DomSlice.translate(-shift)];
{
  if initialize {
    fillRandom(Da, 58979);
    fillRandom(Refa, 58979);
  }
  A = 0;
  Aa = -1;
  var st = getCurrentTime();
  Aa=Da;
  var dt = getCurrentTime()-st;
  for i in Aa.domain do
    if Aa[i] != Refa[i] then
      writeln("ERROR in whole array assignment (alias, - offset): ", i);
  if printOutput then
    writeln("whole array assignment (alias, - offset) ", (m,n,o), ":\n", Aa);
  if printTiming then
    writeln("whole array assignment (alias, - offset) ", (m,n,o), ": ", dt);
}
