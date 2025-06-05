config var isHammingWeightFixed: bool = true;

var projFn: proc(x:int): int;

if isHammingWeightFixed {
  projFn = proc(x:int) { return x; };
} else {
  projFn = proc(x:int) { return x+1; };
}

writeln(projFn(1));
