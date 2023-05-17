config param n = 77;
config const printMemStats = false;

use BlockDist, CyclicDist, BlockCycDist, ReplicatedDist;

enum DMType { default, block, cyclic, blockcyclic, replicated };

proc myDM(param dmType: DMType) {
  select dmType {
    when DMType.default do return defaultDist;
    when DMType.block do return new dmap(new Block(rank=1, boundingBox={1..n}));
    when DMType.cyclic do return new dmap(new Cyclic(startIdx=1));
    when DMType.blockcyclic do return new dmap(new BlockCyclic(startIdx=(1,), blocksize=(3,)));
    when DMType.replicated do return new dmap(new Replicated());
    otherwise halt("unexpected 'dmType': ", dmType);
    }
}

proc privatizedUsed():int {
  var total = 0;
  for loc in Locales do on loc {
    var count = 0;
    extern proc chpl_numPrivatizedClasses():int;
    count = chpl_numPrivatizedClasses();
    total += count;
  }
  return total;
}

use MemDiagnostics;

proc return_domain_map(param dmType: DMType) {
  return myDM(dmType);
}

proc doit(param dmType: DMType) {
  const dm0 = myDM(dmType);

  writeln("Copy of domain map:");
  var m1 = memoryUsed();
  var p1 = privatizedUsed();
  {
    const dm1 = dm0;
  }
  var m2 = memoryUsed();
  var p2 = privatizedUsed();
  writeln("\t", m2-m1, " bytes leaked");
  writeln("\t", p2-p1, " private objects leaked");
  if printMemStats then printMemAllocs();

  writeln("Return domain map:");
  m1 = memoryUsed();
  p1 = privatizedUsed();
  {
    const dm1 = return_domain_map(dmType);
  }
  m2 = memoryUsed();
  p2 = privatizedUsed();
  writeln("\t", m2-m1, " bytes leaked");
  writeln("\t", p2-p1, " private objects leaked");
  if printMemStats then printMemAllocs();

  writeln("Create domain:");
  m1 = memoryUsed();
  p1 = privatizedUsed();
  {
    const D = {1..n} dmapped dm0;
  }
  m2 = memoryUsed();
  p2 = privatizedUsed();
  writeln("\t", m2-m1, " bytes leaked");
  writeln("\t", p2-p1, " private objects leaked");
  if printMemStats then printMemAllocs();

  writeln("Create domain and array:");
  m1 = memoryUsed();
  p1 = privatizedUsed();
  {
    const D = {1..n} dmapped dm0;
    var A: [D] int;
    A = -1;
  }
  m2 = memoryUsed();
  p2 = privatizedUsed();
  writeln("\t", m2-m1, " bytes leaked");
  writeln("\t", p2-p1, " private objects leaked");
  if printMemStats then printMemAllocs();

}

proc main() {
  writeln("Default Dist");
  writeln("============");
  var m1 = memoryUsed();
  serial do doit(DMType.default);
  var m2 = memoryUsed();
  writeln("total:");
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Block Dist");
  writeln("==========");
  m1 = memoryUsed();
  serial do doit(DMType.block);
  m2 = memoryUsed();
  writeln("total:");
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Cyclic Dist");
  writeln("===========");
  m1 = memoryUsed();
  serial do doit(DMType.cyclic);
  m2 = memoryUsed();
  writeln("total:");
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Block-Cyclic Dist");
  writeln("=================");
  m1 = memoryUsed();
  serial do doit(DMType.blockcyclic);
  m2 = memoryUsed();
  writeln("total:");
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Replicated Dist");
  writeln("===============");
  m1 = memoryUsed();
  serial do doit(DMType.replicated);
  m2 = memoryUsed();
  writeln("total:");
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();
}
