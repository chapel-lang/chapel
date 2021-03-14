use driver_arrays;

for e in A2D do e = next();

ref AA = A2D(Dom2D.dim(0), n2-2);

writeln(A2D);
writeln(AA);

for i in Dom2D.dim(0) {
  writeln(A2D(i, n2-2), " == ", AA(i));
  if A2D(i, n2-2).locale != AA(i).locale then
    halt("aliased array element on unexpected locale");
}
