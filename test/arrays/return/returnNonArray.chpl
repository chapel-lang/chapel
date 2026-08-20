proc doit(): ([] int, int) {
  return (2, 3); // error: can't coerce individual tuple elements
}

writeln(doit());
