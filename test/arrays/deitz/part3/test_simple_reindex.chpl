config var size: int = 4, iterations: int = 10;

var A: [1..size] int;
for i in 1..iterations {
  ref AA = A.reindex(0..size-1);
}
