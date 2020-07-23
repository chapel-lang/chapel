use Vector;

config type vectorType = int;
config param vectorLock = true;

var vec1 = new vector(vectorType, vectorLock);

for i in 1..5 do
  vec1.append(i);

writeln(vec1);

var arr1: [1..5] vectorType = [6, 7, 8, 9, 10];

vec1.extend(arr1);

writeln(vec1);

var vec2 = new vector(vectorType, vectorLock);
for i in 11..15 do
  vec2.append(i);

vec1.extend(vec2);
writeln(vec1);

vec1.extend(16..20);
writeln(vec1);
