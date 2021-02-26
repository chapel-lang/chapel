use Vector;

config type listType = int;
config param listLock = true;
config const testIters = 16;

var vec = new vector(listType, listLock);

for i in 1..testIters by -1 do
  vec.append(i);

writeln(vec);

// Sort using default comparator.
vec.sort();

writeln(vec);
