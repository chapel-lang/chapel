module loopsInForallNoVector {
  // bogus function that the compiler shouldn't eliminate
  proc bogusFunction(arg) { return false; }

  proc main() {
    use ItersConfig;
    use iters;

    // outer loop should have vector pragma, but inner should not
    forall (i, j) in zip(myiter(n, ntasks), myiter(n, ntasks)) {
      A[i] = i;
      for x in i..j do  bogusFunction(x);
      for (x, y) in zip(i..j, i..j) do bogusFunction(x+y);
      while (i != j) do bogusFunction(i);
      do { bogusFunction(i); } while (i != j);
    }

    forall i in myiter(n, ntasks) {
      A[i] += i;
      for x in i..i do  bogusFunction(x);
      for (x, y) in zip(i..i, i..i) do bogusFunction(x+y);
      while (bogusFunction(i)) do bogusFunction(i);
      do { bogusFunction(i); } while (bogusFunction(i));
    }

    for a in A do writeln(a);
  }
}
