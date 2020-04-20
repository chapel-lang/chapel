config param size: int = 256;


proc main() {
  var x, y, z: size*real;

  for i in 0..#size {
    x(i) = i+1;
  }

  writeln(x);

  on Locales(1) {
    y = x;
  }

  writeln(y);

  sync {
    on Locales(1) {
      begin with (ref z) {
        z = x;
      }
    }
  }

  writeln(z);
}
