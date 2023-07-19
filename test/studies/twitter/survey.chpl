use MemDiagnostics;
for l in Locales do
  writeln(l.name, ": ", l.physicalMemory(MemUnits.GB, real), "GB, ", l.numPUs(), " cores");
