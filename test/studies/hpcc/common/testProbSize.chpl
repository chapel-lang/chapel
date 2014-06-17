use HPCCProblemSize;

proc main() {
  writeln("For default settings:");
  writeln("  Stream = ", computeProblemSize(real(64), 3));
  writeln("  RA     = ", computeProblemSize(uint(64), 1));
  writeln("  FFT    = ", computeProblemSize(complex(128), 2));
  writeln("  HPL    = ", computeProblemSize(real(64), 1, memRatio=2));
  writeln();

  writeln("For 2**k case:");
  writeln("  Stream = ", 2**computeProblemSize(real(64), 3, returnLog2 = true));
  writeln("  RA     = ", 2**computeProblemSize(uint(64), 1, returnLog2 = true));
  writeln("  FFT    = ",
          2**computeProblemSize(complex(128), 2, returnLog2 = true));
  writeln("  HPL    = ",
          2**computeProblemSize(real(64), 1, returnLog2 = true, memRatio=2));
}

