class G {
  proc read(): int {
    return 1;
  }
}

proc main() {
  var goo       = new unmanaged G();
  var gooResult = goo.read();

  writeln("gooResult = ", gooResult);

  delete goo;
}
