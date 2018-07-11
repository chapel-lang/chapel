class G {
  proc readit(): int {
    return 1;
  }
}

proc main() {
  var goo       = new unmanaged G();
  var gooResult = goo.readit();

  writeln("gooResult = ", gooResult);

  delete goo;
}
