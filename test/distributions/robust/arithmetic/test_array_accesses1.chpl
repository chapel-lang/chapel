use driver;

const Dom1D: domain(1) distributed Dist1D = [1..5];
const Dom2D: domain(2) distributed Dist2D = [1..5, 1..5];
const Dom3D: domain(3) distributed Dist3D = [1..5, 1..5, 1..5];
const Dom4D: domain(4) distributed Dist4D = [1..5, 1..5, 1..5, 1..5];

var A1D: [Dom1D] int;
var A2D: [Dom2D] int;
var A3D: [Dom3D] int;
var A4D: [Dom4D] int;

//
// test that elements were initialized to 0
//
for i in [1..5] do
  if A1D[i] != 0 then
    halt("Element ", i, " in array A1D is not initialized to 0");

for i in [1..5, 1..5] do
  if A2D[i] != 0 then
    halt("Element ", i, " in array A2D is not initialized to 0");

for i in [1..5, 1..5, 1..5] do
  if A3D[i] != 0 then
    halt("Element ", i, " in array A3D is not initialized to 0");

for i in [1..5, 1..5, 1..5, 1..5] do
  if A4D[i] != 0 then
    halt("Element ", i, " in array A4D is not initialized to 0");

//
// write values
//
var value = 1;
for i in [1..5] {
  A1D[i] = value;
  value += 1;
}

value = 1;
for i in [1..5, 1..5] {
  A2D[i] = value;
  value += 1;
}

value = 1;
for i in [1..5, 1..5, 1..5] {
  A3D[i] = value;
  value += 1;
}

value = 1;
for i in [1..5, 1..5, 1..5, 1..5] {
  A4D[i] = value;
  value += 1;
}

//
// test that elements were written correctly
//
value = 1;
for i in [1..5] {
  if A1D[i] != value then
    halt("Element ", i, " in array A1D is not what was written");
  value += 1;
}

value = 1;
for i in [1..5, 1..5] {
  if A2D[i] != value then
    halt("Element ", i, " in array A2D is not what was written");
  value += 1;
}

value = 1;
for i in [1..5, 1..5, 1..5] {
  if A3D[i] != value then
    halt("Element ", i, " in array A3D is not what was written");
  value += 1;
}

value = 1;
for i in [1..5, 1..5, 1..5, 1..5] {
  if A4D[i] != value then
    halt("Element ", i, " in array A4D is not what was written");
  value += 1;
}

writeln("success");
