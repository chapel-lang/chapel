var n: single int;
serial true {
  on Locales(1) {
    n = here.id;
  }
}

foo(n);

proc foo(n: int) {
  var a = n;
}
