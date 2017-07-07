// This test ensures that --force-initializers doesn't break constructor types
// that don't inherit.
class LotsOFields {
  var f1: int;
  var f2: real = 14.3;
  const f3 = false;

  proc LotsOFields(f1, f2) {
    this.f1 = f1;
    this.f2 = f2;
    this.f3 = false;
  }
}

proc main() {
  var c: LotsOFields = new LotsOFields(2, 6.3, true);

  writeln(c);
  delete c;
}
