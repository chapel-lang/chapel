class G {
  proc read(): int {
    return 1;
  }
}

proc main() {
  var goo = new G();
  var gooResult = goo.read();
  writeln("gooResult = ", gooResult);
}
