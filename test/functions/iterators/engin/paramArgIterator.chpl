config param useLF = false;
config param useSA = false;

iter myParallelIter(param x, type y, z) {
  for i in min(y):int .. x do yield i;
}

iter myParallelIter(param x, type y, z, param tag)
  where tag==iterKind.leader && useLF
{
  yield (min(y):int .. x, );
}

iter myParallelIter(param x, type y, z, param tag, followThis)
  where tag==iterKind.follower
{

  for i in followThis[0] do yield i * z;
}

iter myParallelIter(param x, type y, z, param tag) 
  where tag==iterKind.standalone && useSA
{
  for i in min(y):int .. x do yield i * z;
}

for i in myParallelIter(3,uint,10) do
  writeln("ser: ", i);

forall i in myParallelIter(3,uint,10) do
  writeln("par: ", i);
