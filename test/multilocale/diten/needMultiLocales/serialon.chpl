var n: single int;
serial {
  on Locales(1) {
    n.writeEF(here.id);
  }
}

foo(n.readFF());

proc foo(n: int) {
  var a = n;
}
