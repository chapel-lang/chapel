public use driver_domains;

var A1D: [Dom1D] int;
var A2D: [Dom2D] int;
var A3D: [Dom3D] int;
var A4D: [Dom4D] int;
var A2D32: [Dom2D32] int;

proc outputArrays() {
  write("A1D  :"); for i in Space1 do write(" ", A1D[i]); writeln();
  write("A2D  :"); for i in Space2 do write(" ", A2D[i]); writeln();
  write("A3D  :"); for i in Space3 do write(" ", A3D[i]); writeln();
  write("A4D  :"); for i in Space4 do write(" ", A4D[i]); writeln();
  write("A2D32:"); for i in Space2D32 do write(" ", A2D32[i]); writeln();
}

