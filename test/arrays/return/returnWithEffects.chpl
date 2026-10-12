proc sideeffect(type t, n) {
  writeln("side effect ", n, " for ", t:string);
  return [i in 1..10] i:t;
}

proc basic(type t): [] t {
  return sideeffect(t, 1);
}
proc nested(type t): ([] t, [] t) {
  return (sideeffect(t, 1), sideeffect(t, 2));
}
proc superNested(type t): (([] t, [] t), [] t) {
  return ((sideeffect(t, 1), sideeffect(t, 2)), sideeffect(t, 3));
}
proc superNested2(type t): ([] t, ([] t, [] t), [] t) {
  return (sideeffect(t, 1), (sideeffect(t, 2), sideeffect(t, 3)), sideeffect(t, 4));
}


writeln(basic(int));
writeln(nested(int));
writeln(superNested(real));
writeln(superNested2(int(32)));
