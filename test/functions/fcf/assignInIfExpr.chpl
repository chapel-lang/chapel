// code from https://github.com/chapel-lang/chapel/issues/18413, note that
// it did not compile then either, though at a later point in compilation
var isHammingWeightFixed: bool = true;

var projFn = if isHammingWeightFixed
  then proc (x) { return x; }
  else proc (x) { return x; };
