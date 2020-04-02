proc main {
  var t: 2*string;
  t = ("hi", "hello");
  writeln(t);
  on Locales(1) {
    var tmp: string;
    tmp = t(0);
    t(0) = t(1);
    t(1) = tmp;
  }
  writeln(t);
}
