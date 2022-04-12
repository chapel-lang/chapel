use Map, DistributedMap, Aggregator, BlockDist, Random;

/////////////////////////////////
// randomStrings parameters and reference
config const defaultSeed = 3141592;
config const numStrings  = 20_000;

// int(8) means the maps will have only 256 keys with large histogram counts
// use bigger-sized types to have more keys with smaller counts
config type stringsType = int(8);

var reference = createReference();

/////////////////////////////////
// distributedMap.bulkUpdate(): for an HPC programmer

var dm1 = new distributedMap(string, int);
dm1.bulkUpdate(new randomStrings(), new histoUpdater());
verify(dm1, "check1");

/////////////////////////////////
// compiler optimizes for the user

var dm2 = new distributedMap(string, int);

/*
Our goal is for the compiler to transform the following forall loops
into the Aggregator-enabled code shown after this comment:

// if we rely on context managers
forall s in INPUT do
  manage dm2.updateManager(s) as element do
    element += 1;

// if we rely on the 'sync' construct
forall s in INPUT do
  sync dm2.updateManager(s) do
    dm2[s] += 1;
*/
forall s in new randomStrings()
  with (var agg = dm2.createAggregator(new histoUpdater()))
do
  agg.add(s);

// independent of the above: test updateManager by itself
reference.addOrSet("1234", 1_000_000);
manage dm2.updateManager("1234") as element do
  element = 1_000_000;
assert(dm2.contains("1234"));

verify(dm2, "check2");

/////////////////////////////////
// randomStrings: generate random strings from "-32768" to "32767"

record randomStrings {
  const dom    = newBlockDom({0..#numStrings});
  const seed   = defaultSeed;
  const stream = createRandomStream(stringsType, seed, parSafe=false);

  proc init() {} // ensure the default values are used consistently

  iter these() {
    for rnd in stream.iterate(dom) do
      yield rnd:string;
  }

  iter these(param tag) where tag == iterKind.standalone {
    forall rnd in stream.iterate(dom) do
      yield rnd:string;
  }
}

// a reference to compare against
proc createReference() {
  var refmap = new map(string, int, parSafe=false);
  for s in new randomStrings() do
    refmap(s) += 1;
  return refmap;
}

proc verify(dm, msg) {
  assert(reference.size == dm.size);
  for (k,v) in reference.items() do
    manage dm.updateManager(k) as element do
      assert(element == v && v > 0);
  writeln(msg, " passed");
}

// when used in update functions, increments 'val' by 1
record histoUpdater {
  proc this(key, ref val) { val += 1; return none; }
}
