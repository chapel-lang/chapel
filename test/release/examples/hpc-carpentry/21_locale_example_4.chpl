use Memory.Diagnostics;
  for loc in Locales do
    on loc {
      writeln("locale #", here.id, "...");
      writeln("  ...is named: ", here.name);
      writeln("  ...has ", here.numPUs(), " processor cores");
      writeln("  ...has ", here.physicalMemory(unit=MemUnits.GB, retType=real), " GB of memory");
      writeln("  ...has ", here.maxTaskPar, " maximum parallelism");
}
