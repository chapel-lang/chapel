/* yields elements from B with a between each element */
iter intersperse(a, B) {
  var first = true;
  for b in B {
    if ! first then yield a;
    yield b;
    first = false;
  }
}
writeln(intersperse(0, 1..3));
