use Vector;

config const testNum = 42;

//
// Use a higher iteration count to try and trigger a resize.
//
config const testIters = 32;

type vectorType = int;
param vectorLock = true;

var vec1 : vector(vectorType, vectorLock);

writeln(vec1.count(testNum));

for 1..testIters do {
  vec1.append(testNum);
  const count = vec1.count(testNum);
  writeln(count);
}

var vec2 = vec1;

vec2.extend(vec1);

writeln(vec2.count(testNum));

vec1.clear();
vec2.clear();

writeln(vec1.count(testNum));
writeln(vec2.count(testNum));
