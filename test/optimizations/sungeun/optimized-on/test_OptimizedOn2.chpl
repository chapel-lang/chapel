config const n = 10;
config const l = n/2;

record R {
  var r: int;
}
var r:R;

on (rootLocale.getLocales()(0)) do {
  local {
    r.r = l;
  }
 }

writeln(r);
