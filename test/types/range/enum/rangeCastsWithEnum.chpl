enum color {red, green, blue}
enum flavor {chocolate=0, vanilla, strawberry}
enum size {small=1, medium, large}
enum polygon {triangle=300, square=400, pentagon=500}

var rb = false..true;
var ri0 = 0..1;
var ri1 = 1..2;
var ru0 = 0:uint..1:uint;
var ru1 = 1:uint..2:uint;
var rc = color.red..color.green;
var rf = flavor.chocolate..flavor.strawberry;
var rs = size.small..size.medium;
var rp = polygon.triangle..polygon.pentagon;


tryCasts(ri0);
tryCasts(ri1);
tryCasts(ru0);
tryCasts(ru1);
tryCasts(rc);
tryCasts(rs);
tryCasts(rp);
tryCasts(rf);

proc tryCasts(r) {
  type types = (bool, int, uint, int(8), uint(8), color, flavor, size, polygon);
  for param i in 0..<types.size do
    tryCast(r, types(i));
}
  
proc tryCast(r, type t) {
  // only focus on cases involving enums
  if !isEnumType(r.idxType) && !isEnumType(t) then
    return;

  // can't cast to/from abstract enums
  if isAbstractEnumType(t) || isAbstractEnumType(r.idxType) then
    return;

  // can't currently cast between enums
  if isEnumType(r.idxType) && isEnumType(t) then
    return;
  
  writeln("casting ", r, " to range of ", t:string);
  try {
    var res = r: range(t);
    printRange(res);
  } catch e {
    writeln("  caught Error: ", e.message());
    writeln();
  }
  writeln("casting ", r, " to stridable range of ", t:string, ":");
  try {
    var resStr = r: range(t, strides=strideKind.any);
    printRange(resStr);
  } catch e {
    writeln("  caught Error: ", e.message());
    writeln();
  }
}

proc printRange(r) {
  writeln("  ", r, ": ", (r.low, r.high, r.stride, r.alignment));
  writeln();
}
