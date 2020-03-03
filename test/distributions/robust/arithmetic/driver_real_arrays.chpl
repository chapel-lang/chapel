public use driver_domains;

var R1D: [Dom1D] real;
var R2D: [Dom2D] real;
var R3D: [Dom3D] real;
var R4D: [Dom4D] real;
var R2D32: [Dom2D32] real;

proc outputRealArrays() {
  write("R1D  :"); for i in Space1 do write(" ", R1D[i]); writeln();
  write("R2D  :"); for i in Space2 do write(" ", R2D[i]); writeln();
  write("R3D  :"); for i in Space3 do write(" ", R3D[i]); writeln();
  write("R4D  :"); for i in Space4 do write(" ", R4D[i]); writeln();
  write("R2D32:"); for i in Space2D32 do write(" ", R2D32[i]); writeln();
}

