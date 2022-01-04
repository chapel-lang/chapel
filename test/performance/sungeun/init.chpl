config const n = 5;
config const m = 5;
config const printOutput = true;
config const printTiming = false;

use Random, DateTime;

var A: [1..n,1..m] real;

writeln("n=", n, " m=", m);

var st = datetime.timeSinceEpoch();
for i in 1..n do
  for j in 1..m do
    A[i,j] = i+j;
var dt = datetime.timeSinceEpoch()-st;
if printOutput then
  writeln("init serial:\n", A);
if printTiming then
  writeln("init serial: ", dt);

st = datetime.timeSinceEpoch();
forall i in 1..n do
  forall j in 1..m do
    A[i,j] = i+j;
dt = datetime.timeSinceEpoch()-st;
if printOutput then
  writeln("init nested forall:\n", A);
if printTiming then
  writeln("init nested forall: ", dt);

st = datetime.timeSinceEpoch();
for i in 1..n do
  forall j in 1..m do
    A[i,j] = i+j;
dt = datetime.timeSinceEpoch()-st;
if printOutput then
  writeln("init for-forall:\n", A);
if printTiming then
  writeln("init for-forall: ", dt);

st = datetime.timeSinceEpoch();
forall i in 1..n do
  for j in 1..m do
    A[i,j] = i+j;
dt = datetime.timeSinceEpoch()-st;
if printOutput then
  writeln("init forall-for:\n", A);
if printTiming then
  writeln("init forall-for: ", dt);
