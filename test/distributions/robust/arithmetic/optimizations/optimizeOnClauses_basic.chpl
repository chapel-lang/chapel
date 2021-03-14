use driver_arrays;

on (Locales(numLocales-1)) do {
  local {
    A1D(n1) = n1;
    A2D(n2,n2) = n2;
    A3D(n3,n3,n3) = n3;
    A4D(n4,n4,n4,n4) = n4;
    A2D32(n5,n5) = o5;
  }
 }

writeln(A1D(n1));
writeln(A2D(n2,n2));
writeln(A3D(n3,n3,n3));
writeln(A4D(n4,n4,n4,n4));
writeln(A2D32(n5,n5));
