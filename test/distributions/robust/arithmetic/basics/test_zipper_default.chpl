use driver_arrays;

for e in A1D do e = next();
for e in A2D do e = next();
for e in A3D do e = next();
for e in A4D do e = next();
for e in A2D64 do e = next();

var DefaultA1D: [Space1] int;
var DefaultA2D: [Space2] int;
var DefaultA3D: [Space3] int;
var DefaultA4D: [Space4] int;
var DefaultA2D64: [Space2D64] int;

forall (d,e) in (DefaultA1D, A1D) do
  d = e + 1;

forall (d,e) in (DefaultA2D, A2D) do
  d = e + 1;

forall (d,e) in (DefaultA3D, A3D) do
  d = e + 1;

forall (d,e) in (DefaultA4D, A4D) do
  d = e + 1;

forall (d,e) in (DefaultA2D64, A2D64) do
  d = e + 1;

writeln("-----------------------------------");
writeln(DefaultA1D);
writeln(DefaultA2D);
writeln(DefaultA3D);
writeln(DefaultA4D);
writeln(DefaultA2D64);

forall (e,d) in (A1D, DefaultA1D) do
  d = e + 2;

forall (e,d) in (A2D, DefaultA2D) do
  d = e + 2;

forall (e,d) in (A3D, DefaultA3D) do
  d = e + 2;

forall (e,d) in (A4D, DefaultA4D) do
  d = e + 2;

forall (e,d) in (A2D64, DefaultA2D64) do
  d = e + 2;

writeln("-----------------------------------");
writeln(DefaultA1D);
writeln(DefaultA2D);
writeln(DefaultA3D);
writeln(DefaultA4D);
writeln(DefaultA2D64);

forall (d,e) in (DefaultA1D, A1D) do
  e = d + 1;

forall (d,e) in (DefaultA2D, A2D) do
  e = d + 1;

forall (d,e) in (DefaultA3D, A3D) do
  e = d + 1;

forall (d,e) in (DefaultA4D, A4D) do
  e = d + 1;

forall (d,e) in (DefaultA2D64, A2D64) do
  e = d + 1;

writeln("-----------------------------------");
outputArrays();

forall (e,d) in (A1D, DefaultA1D) do
  e = d + 2;

forall (e,d) in (A2D, DefaultA2D) do
  e = d + 2;

forall (e,d) in (A3D, DefaultA3D) do
  e = d + 2;

forall (e,d) in (A4D, DefaultA4D) do
  e = d + 2;

forall (e,d) in (A2D64, DefaultA2D64) do
  e = d + 2;

writeln("-----------------------------------");
outputArrays();
