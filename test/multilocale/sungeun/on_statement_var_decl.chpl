on Locales(1) var x: real;

on Locales(1) {
  writeln((x.locale, x));
  local {
    x = 2.0;
  }
  on Locales(0) {
    writeln((x.locale, x));
  }
}

