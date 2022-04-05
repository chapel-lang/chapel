config const n = 10;

config const alpha = 10;

config const writeArrays = true;

writeln("Number of sublocales: ", here.getChildCount());

for subloc in 1..here.getChildCount()-1 do on here.getChild(subloc) {
  var A: [1..n] int;
  var B: [1..n] int;
  var C: [1..n] int;

  A = 1;                            writeArr(A);
  B = 2;                            writeArr(B);
  C = 3;                            writeArr(C);
  A = B + alpha * C;                writeArr(A);
}

proc writeArr(A) {
  if writeArrays {
    // normally we can just do writeln(A), but we don't have a good way of
    // having verbose GPU execution. So, if we do that the output is just too
    // messy with verbose. If we had verbosity for only launches, things would
    // have been much easier.
    write("Array: ");
    for a in A do
      write(a, " ");
    writeln();
  }
}
