on rootLocale.getLocales()(1) var x: real;

on rootLocale.getLocales()(1) {
  writeln((x.locale, x));
  local {
    x = 2.0;
  }
  on rootLocale.getLocales()(0) {
    writeln((x.locale, x));
  }
}

