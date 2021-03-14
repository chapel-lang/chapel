proc main() {
  bitshiftUnchecked(8);
  bitshiftUnchecked(16);
  bitshiftUnchecked(32);
  bitshiftUnchecked(64);

  return;
}

//
// Shift a signed and unsigned value by a variety of values, some positive
// and some negative, irregardless of potential undefined behavior.
//
proc bitshiftUnchecked(param w: int) {
  const si: int(w) = 127:int(w);
  for shift in (-2*w)..2*w do
    const foo = si << shift;

  const ui: uint(w) = 127:uint(w);
  for shift in (-2*w)..2*w do
    const foo = ui << shift;

  return;
}
