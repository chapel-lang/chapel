var n: single int;
serial true {
  on Locales(1) {
    n = localeID();
  }
}

foo(n);

def foo(n: int) {
  var a = n;
}
