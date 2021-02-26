use Vector;

config type vectorType = int;
config param vectorLock = true;

config const testIters = 1024;

var lst1 = new vector(vectorType, vectorLock);

for i in 1..testIters do
  lst1.append(i);

var lst2 = new vector(vectorType, vectorLock);

writeln(lst1 != lst2);

lst2.extend(lst1);

writeln(lst1 == lst2);

lst1.clear();

writeln(lst1 != lst2);

lst2.clear();

writeln(lst1 == lst2);
