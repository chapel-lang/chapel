proc main() {
  var x, y, z: int;
  on rootLocale.getLocales()(1) {
    x = 1;
    y = 2;
    z = x + y;
    on rootLocale.getLocales()(0) {
      local {
        x = y + z;
      }
    }
  }
  writeln(x, y, z);
}
