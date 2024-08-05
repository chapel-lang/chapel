use CTypes;

proc test(ref Arr) {
  write(Arr);
  read(Arr);
}

proc write(ref Arr) {
  var Arr_p = c_addrOf(Arr);

  for i in Arr.domain do
   Arr_p.deref()[i] = i;
}

proc read(const ref Arr) {
  var Arr_p = c_addrOfConst(Arr);

  for i in Arr.domain do
   write(Arr_p.deref()[i], " ");
  writeln();
}

{
  writeln("Block");
  use BlockDist;
  var Arr = blockDist.createArray(1..10, int);
  test(Arr);
}

{
  writeln("Cyclic");
  use CyclicDist;
  var Arr = cyclicDist.createArray(1..10, int);
  test(Arr);
}

{
  writeln("Stencil");
  use StencilDist;
  var Arr = stencilDist.createArray(1..10, int);
  test(Arr);
}

{
  writeln("BlockCyc");
  use BlockCycDist;
  const Dom = {1..10} dmapped new blockCycDist(1, 2);
  var Arr: [Dom] int;
  test(Arr);
}

{
  writeln("Replicated");
  use ReplicatedDist;
  const Dom = {1..10} dmapped new replicatedDist();
  var Arr: [Dom] int;
  test(Arr);
}

{
  writeln("Private");
  use PrivateDist;
  var Arr: [PrivateSpace] int;

  // we want this to pass both in single and multi locales, so can't use `test`
  // above
  write(Arr);
  var Arr_p = c_addrOfConst(Arr);
  for i in LocaleSpace do assert(Arr_p.deref()[i] == i);
  writeln("Success");
}
