use driver_arrays;

outputArrays();

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
for i in Space2D32 {
  A2D32[i] = value;
  value += 1;
}

outputArrays();
