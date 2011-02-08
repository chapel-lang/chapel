class G {
  proc readit(): int {
    return 1;
  }
}

proc main() {
  var goo = new G();
  var gooResult = goo.readit();
  writeln("gooResult = ", gooResult);
}
