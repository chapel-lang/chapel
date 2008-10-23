use HPCCProblemSize;

def main() {
  writeln("For default settings:");
  writeln("  Stream = ", computeProblemSize(3, real(64)));
  writeln("  RA     = ", computeProblemSize(1, uint(64)));
  writeln("  FFT    = ", computeProblemSize(2, real(64)));
  writeln("  HPL    = ", computeProblemSize(1, real(64), rank=2, memFraction=2));
  writeln();

  writeln("For 2**k case:");
  writeln("  Stream = ", 2**computeProblemSize(3, real(64), returnLog2=true));
  writeln("  RA     = ", 2**computeProblemSize(1, uint(64), returnLog2=true));
  writeln("  FFT    = ", 2**computeProblemSize(2, real(64), returnLog2=true));
  /*
  writeln("  HPL    = ", 2**computeProblemSize(1, real(64), returnLog2=true, 
                                               rank=2, memFraction=2));
  */
}

