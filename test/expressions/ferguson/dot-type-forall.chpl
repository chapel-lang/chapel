
proc f(arg) {
  writeln("in f");
  return 5;
}

proc default(type t) {
  var tmp:t;
  return tmp;
}

forall loc in Locales with (var tpva: f(1).type,
                            var tpvb: f(2).type) {
  writeln(tpva, tpvb);
}

forall loc in Locales with (var tpva = default(f(1).type),
                            var tpvb = default(f(2).type)) {
  writeln(tpva, tpvb);
}

forall loc in Locales with (var tpva: f(1).type,
                            var tpvb: f(tpva).type) {
  writeln(tpva, tpvb);
}

forall loc in Locales with (var tpva = default(f(1).type),
                            var tpvb = default(f(tpva).type)) {
  writeln(tpva, tpvb);
}
