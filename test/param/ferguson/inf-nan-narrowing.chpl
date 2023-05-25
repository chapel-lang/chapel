proc checkOne(arg: real(32)) {
  assert(!isnan(arg));
  assert(!isinf(arg));
  assert(signbit(arg) == false);
  assert(arg == 1);
}
proc checkNan(arg: real(32)) {
  assert(isnan(arg));
}
proc checkPosInf(arg: real(32)) {
  assert(isinf(arg));
  assert(signbit(arg) == false);
}
proc checkNegInf(arg: real(32)) {
  assert(isinf(arg));
  assert(signbit(arg) == true);
}

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

var x: real(32) = 1;

checkOne(x*one);
checkPosInf(x*inf);
checkNegInf(x*neginf);
checkPosInf(x*inf0);
checkNegInf(x*neginf0);
checkNegInf(x*neginf0n);
checkNan(x*nanzeros);
checkNan(x*negnanzeros);
checkNan(x*naninf);
checkNan(x*negnaninf);

checkPosInf(x*INFINITY);
checkNegInf(-(x*INFINITY));
checkNegInf(x*(-INFINITY));
