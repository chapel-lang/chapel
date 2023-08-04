// deprecated by Vass in 1.32

/////////// ranges ///////////

var rb = false..true;
tryCasts(rb);

var ri0 = 0..1;
var ri1 = 1..2;
var rin = -2..2;
tryCasts(ri0);
tryCasts(ri1);
tryCasts(rin);

var ru0 = 0:uint..1:uint;
var ru1 = 1:uint..2:uint;
var rup = (max(uint))-10..(max(uint))-9;
tryCasts(ru0);
tryCasts(ru1);
tryCasts(rup);

proc tryCasts(r) {
  type types = (bool, int, uint, int(8), uint(8));
  for param i in 0..<types.size do
    tryCast(r, types(i));
}

proc safe(r, type t) {
  if t == bool && ((r.low != 0 && r.low != 1) || (r.high != 0 && r.high != 1)) then
    return false;
  if isUintType(t) && isIntType(r.idxType) &&  (r.low < 0 || r.high < 0) then
    return false;
  if isIntegralType(t) && r.idxType != bool && (r.low > max(t) || r.high > max(t)) then
    return false;
  return true;
}

proc tryCast(r, type t) {
  if safe(r,t) {
    writeln("casting ", r, " to range of ", t:string);
    var res = r.safeCast(range(t));
    printRange(res);

    writeln("casting ", r, " to stridable range of ", t:string, ":");
    var resStr = r.safeCast(range(t, strides=strideKind.any));
    printRange(resStr);
  }
}

proc printRange(r) {
  writeln("  ", r, ": ", (r.low, r.high, r.stride, r.alignment));
  writeln();
}

/////////// domains ///////////

// from test/domains/diten/castAwayStridableDomain.chpl

var stridableDom = {1..20 by 2};
var stridableDomUnit = {1..20 by 1}: domain(1, strides=strideKind.any);
var unstridableDom = {1..10};

writeln("before cast/assign unstridableDom=", unstridableDom);        // {1..10}
writeln("before cast/assign stridableDom=", stridableDom);            // {1..20 by 2}

unstridableDom = stridableDomUnit.safeCast(domain(1, strides=strideKind.one));

writeln("after safeCast/assign unstridableDom=", unstridableDom);     // {1..20}
writeln("after safeCast/assign stridableDomUnit=", stridableDomUnit); // {1..20}

// halt reached - non-stridable range assigned non-unit stride
unstridableDom = stridableDom.safeCast(domain(1, strides=strideKind.one));
