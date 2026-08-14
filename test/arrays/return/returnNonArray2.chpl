proc doit(): [] int {
  return 2; // error: can't coerce to array, we don't know the domain
}

writeln(doit());
