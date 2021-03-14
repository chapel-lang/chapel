proc main() {
  var x, y, z: int;
  on Locales(1) {
    x = 1;
    y = 2;
    z = x + y;
    on Locales(0) {
      local {
        x = y + z;
      }
    }
  }
  writeln(x, y, z);
}
