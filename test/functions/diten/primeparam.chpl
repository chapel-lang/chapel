proc prime_param(param n: int) param {
  // Decide if n is a prime 100% in the compiler.
  // Generate TONS of function clones while doing so.
  if (n == 2) then
    return true;
  else if (n < 2) then
    return false;
  else if (n%2 == 0) then
    return false;
  else
    return prime_param_help();

  proc prime_param_help(param mm: int = 3) param {
    // I'll use a REALLY bad estimate of sqrt(n) = n/2
    param nn = n/2;
    if (mm > nn) then
      return true;
    else if (n%mm == 0) then
      return false;
    else
      return prime_param_help(mm+2);
  }
}

proc main() {
  for param i in 2..40 do
    writeln(i, if prime_param(i) then " is prime" else " is not prime");
}

