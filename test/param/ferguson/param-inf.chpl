
param zero = 0.0;
param nzero = -0.0;
param one = 1.0;
param huge = 1.0e+300;
param tiny = 1.0e-300;
param inf = huge / tiny;
param neginf = -huge / tiny;
param inf0 = one / zero;
param neginf0 = -1.0 / 0.0;
param neginf0n = -(one / zero);
param nanzeros = zero / zero;
param negnanzeros = -nanzeros;
param naninf = inf / inf;
param negnaninf = -naninf;

proc printit(name:string, arg:real(64), str:string, nosign=false) {
  extern proc copysign(x:real(64), y:real(64)):real(64);
  var one = 1.0;
  var pos_neg_one = copysign(one, arg);
  var neg = pos_neg_one < 0.0;
  writef("% 8s % 14s %10r % 12s", arg.type:string, name, arg, str);
  if nosign || !neg then
    writeln();
  else
    writeln(" negative");
}
proc printit(name:string, param arg:real(64), nosign=false) {
  param str = arg:string;
  printit(name, arg, str, nosign);
}

printit("zero", zero);
printit("nzero", nzero);
printit("one", one);
printit("huge", huge);
printit("tiny", tiny);
printit("inf0", inf0);
printit("inf", inf);
printit("neginf", neginf);
printit("neginf0", neginf0);
printit("neginf0n", neginf0n);

// Ignore signs for these NANs, they seem to come out the opposite
// of what I expect and I'm not sure this will be the same on all systems.
// Generally speaking, the sign of a NAN is irrelevant.
printit("nanzeros", nanzeros, nosign=true);
printit("negnanzeros", negnanzeros, nosign=true);
printit("naninf", naninf, nosign=true);
printit("negnaninf", negnaninf, nosign=true);

printit("NAN", NAN);
printit("-NAN", -NAN);
printit("INFINITY", INFINITY);
printit("-INFINITY", -INFINITY);

