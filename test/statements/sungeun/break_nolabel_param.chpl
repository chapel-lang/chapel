config param iters=10;
config var contIter=4;

for param i in 1..iters do
  if i==contIter then break;
  else writeln(i);

