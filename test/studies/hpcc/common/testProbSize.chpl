use HPCCProblemSize;

def main() {
  writeln("For default settings:");
  writeln("  Stream = ", computeProblemSize(real(64), 3));
  writeln("  RA     = ", computeProblemSize(uint(64), 1));
  writeln("  FFT    = ", computeProblemSize(real(64), 2));
  writeln();

  writeln("For 2**k case:");
  writeln("  Stream = ", computeProblemSize(real(64), 3, returnPow2 = true));
  writeln("  RA     = ", computeProblemSize(uint(64), 1, returnPow2 = true));
  writeln("  FFT    = ", computeProblemSize(real(64), 2, returnPow2 = true));
}

