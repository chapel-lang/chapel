proc main() {
  var x, y, z: int;

  proc foo() var {
    return z;
  }

  on rootLocale.getLocales()(1) {
    x = 1;
    y = 2;
    z = x + y;
    on rootLocale.getLocales()(0) {
      local {
        x = y + z;
        foo() = 8;
      }
    }
  }
  writeln(x, y, z);
}
