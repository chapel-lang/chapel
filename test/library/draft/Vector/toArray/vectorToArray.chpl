use Vector;

config type vectorType = int;
config param vectorLock = true;

config const testIters = 8;


var vec = new vector(vectorType, vectorLock);

for i in 1..testIters do
  vec.append(i);

var arr = vec.toArray();

writeln(vec);
writeln(arr);

