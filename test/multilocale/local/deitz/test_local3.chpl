proc main() {
  var x, y, z: int;

  proc foo() ref {
    return z;
  }

  on Locales(1) {
    x = 1;
    y = 2;
    z = x + y;
    on Locales(0) {
      local {
        x = y + z;
        foo() = 8;
      }
    }
  }
  writeln(x, y, z);
}
