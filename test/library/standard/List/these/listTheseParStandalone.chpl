private use List;

config const testIters = 8;

const arr: [1..testIters] int = 1..testIters;

var lst: list(int) = arr;

forall (x, y) in zip(arr, lst) do
  writeln("arr is ", x.type:string, " ", x, " lst is ", y.type:string, " ", y);


