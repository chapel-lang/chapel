config const n = 5;
config const m = 5;
config const initialize = true;
config const printOutput = true;
config const printTiming = false;

use Random, DateTime;

var A: [1..n,1..m] real;
var B: [1..n,1..m] real;

writeln("n=", n, " m=", m);

if initialize then
  fillRandom(B, 31415, algorithm=RNG.NPB);
var st = datetime.timeSinceEpoch();
for i in 1..n do
  for j in 1..m do
    A[i,j] = B[i,j];
var dt = datetime.timeSinceEpoch()-st;
if printOutput then
  writeln("assign serial:\n", A);
if printTiming then
  writeln("assign serial: ", dt);

if initialize then
  fillRandom(B, 31415, algorithm=RNG.NPB);
st = datetime.timeSinceEpoch();
for (a,b) in zip(A,B) do
  a = b;
dt = datetime.timeSinceEpoch()-st;
if printOutput then
  writeln("assign serial zipper:\n", A);
if printTiming then
  writeln("assign serial zipper: ", dt);

if initialize then
  fillRandom(B, 31415, algorithm=RNG.NPB);
st = datetime.timeSinceEpoch();
forall i in 1..n do
  forall j in 1..m do
    A[i,j] = B[i,j];
dt = datetime.timeSinceEpoch()-st;
if printOutput then
  writeln("assign nested forall:\n", A);
if printTiming then
  writeln("assign nested forall: ", dt);

if initialize then
  fillRandom(B, 31415, algorithm=RNG.NPB);
st = datetime.timeSinceEpoch();
for i in 1..n do
  forall j in 1..m do
    A[i,j] = B[i,j];
dt = datetime.timeSinceEpoch()-st;
if printOutput then
  writeln("assign for-forall:\n", A);
if printTiming then
  writeln("assign for-forall: ", dt);

if initialize then
  fillRandom(B, 31415, algorithm=RNG.NPB);
st = datetime.timeSinceEpoch();
forall i in 1..n do
  for j in 1..m do
    A[i,j] = B[i,j];
dt = datetime.timeSinceEpoch()-st;
if printOutput then
  writeln("assign forall-for:\n", A);
if printTiming then
  writeln("assign forall-for: ", dt);

if initialize then
  fillRandom(B, 31415, algorithm=RNG.NPB);
st = datetime.timeSinceEpoch();
forall (a,b) in zip(A,B) do
  a = b;
dt = datetime.timeSinceEpoch()-st;
if printOutput then
  writeln("assign forall zipper:\n", A);
if printTiming then
  writeln("assign forall zipper: ", dt);

if initialize then
  fillRandom(B, 31415, algorithm=RNG.NPB);
st = datetime.timeSinceEpoch();
A = B;
dt = datetime.timeSinceEpoch()-st;
if printOutput then
  writeln("assign whole array:\n", A);
if printTiming then
  writeln("assign whole array: ", dt);

