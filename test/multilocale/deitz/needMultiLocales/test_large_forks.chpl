config param size: int = 256;


proc main() {
  var x, y, z: size*real;

  for i in 1..size {
    x(i) = i;
  }

  writeln(x);

  on rootLocale.getLocales()(1) {
    y = x;
  }

  writeln(y);

  sync {
    on rootLocale.getLocales()(1) {
      begin {
        z = x;
      }
    }
  }

  writeln(z);
}
