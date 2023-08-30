
config param testTryCast = false;

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

/* abstract enums are not castable to/from anything other than strings
enum absEnum { a0, a1 }
var rabs = absEnum.a0..absEnum.a1;
tryCasts(rabs);
*/

enum concEnum {c2m=-2, c2p=2 }
var rconc = concEnum.c2m..concEnum.c2p;
tryCasts(rconc);

proc tryCasts(r) {
  type types = (bool, int, uint, int(8), uint(8), /*absEnum,*/ concEnum);
  for param i in 0..<types.size do
    tryCast(r, types(i));
}
  
proc tryCast(r, type t) {
  if isBool(r.idxType) && isEnum(t) then return; // not castable
  // tryCast() into range(bool) is disabled, see #22905
  param runTryCast = testTryCast && (!isBool(t) || isBool(r.idxType));

  writeln("casting ", r, " to range of ", t:string);
 try {
  var res = if runTryCast then r.tryCast(range(t)) else r: range(t);
  printRange(res);
 } catch e {
  writeln("  ", e); writeln();
 }

  var sa   = r: range(r.idxType, strides=strideKind.any);   // always succeeds
  var sa2  = r.tryCast( range(r.idxType, strides=strideKind.any) );  // "
 try {
  var sneg = sa.tryCast(range(r.idxType, strides=strideKind.negative));//throws
  printRange(sneg);
 } catch e {
  assert(e.message() == "bad cast from stride 1 to strideKind.negative");
 }
}

proc printRange(r) {
  writeln("  ", r, ": ", (r.low, r.high, r.stride, r.alignment));
  writeln();
}
