use Vector;

type vectorType = int;
param vectorLock = true;


var lst: vector(vectorType, vectorLock);

writeln(lst.isEmpty());

for i in 1..5 do
  lst.append(i:vectorType);

writeln(lst.isEmpty());

lst.clear();

writeln(lst.isEmpty());
