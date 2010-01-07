use driver;

const Dom1D: domain(1) distributed Dist1D = Space1;
const Dom2D: domain(2) distributed Dist2D = Space2;
const Dom3D: domain(3) distributed Dist3D = Space3;
const Dom4D: domain(4) distributed Dist4D = Space4;
const Dom2D64: domain(2,int(64)) distributed Dist2D64 = Space2D64;

var A1D: [Dom1D] int;
var A2D: [Dom2D] int;
var A3D: [Dom3D] int;
var A4D: [Dom4D] int;
var A2D64: [Dom2D64] int;

//
// test that elements were initialized to 0
//
for i in Space1 do
  if A1D[i] != 0 then
    halt("Element ", i, " in array A1D is not initialized to 0");

for i in Space2 do
  if A2D[i] != 0 then
    halt("Element ", i, " in array A2D is not initialized to 0");

for i in Space3 do
  if A3D[i] != 0 then
    halt("Element ", i, " in array A3D is not initialized to 0");

for i in Space4 do
  if A4D[i] != 0 then
    halt("Element ", i, " in array A4D is not initialized to 0");

for i in Space2D64 do
  if A2D64[i] != 0 then
    halt("Element ", i, " in array A2D64 is not initialized to 0");

//
// write values
//
var value = 1;
for i in Space1 {
  A1D[i] = value;
  value += 1;
}

value = 1;
for i in Space2 {
  A2D[i] = value;
  value += 1;
}

value = 1;
for i in Space3 {
  A3D[i] = value;
  value += 1;
}

value = 1;
for i in Space4 {
  A4D[i] = value;
  value += 1;
}

value = 1;
for i in Space2D64 {
  A2D64[i] = value;
  value += 1;
}

//
// test that elements were written correctly
//
value = 1;
for i in Space1 {
  if A1D[i] != value then
    halt("Element ", i, " in array A1D is not what was written");
  value += 1;
}

value = 1;
for i in Space2 {
  if A2D[i] != value then
    halt("Element ", i, " in array A2D is not what was written");
  value += 1;
}

value = 1;
for i in Space3 {
  if A3D[i] != value then
    halt("Element ", i, " in array A3D is not what was written");
  value += 1;
}

value = 1;
for i in Space4 {
  if A4D[i] != value then
    halt("Element ", i, " in array A4D is not what was written");
  value += 1;
}

value = 1;
for i in Space2D64 {
  if A2D64[i] != value then
    halt("Element ", i, " in array A2D64 is not what was written");
  value += 1;
}

writeln("success");
