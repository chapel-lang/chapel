use MemDiagnostics;

proc output(type retType) {
  const mem = Locales(0).physicalMemory(retType = retType),
        nBytes = Locales(0).physicalMemory(MemUnits.Bytes, retType),
        kb = Locales(0).physicalMemory(MemUnits.KB, retType),
        mb = Locales(0).physicalMemory(MemUnits.MB, retType),
        gb = Locales(0).physicalMemory(MemUnits.GB, retType);

  writeln("memory is: ", mem);
  writeln(" in bytes: ", nBytes);
  writeln("    in kb: ", kb);
  writeln("    in mb: ", mb);
  writeln("    in gb: ", gb);
}

const mem = Locales(0).physicalMemory(),
      nBytes = Locales(0).physicalMemory(MemUnits.Bytes),
      kb = Locales(0).physicalMemory(MemUnits.KB),
      mb = Locales(0).physicalMemory(MemUnits.MB),
      gb = Locales(0).physicalMemory(MemUnits.GB);

writeln("memory is: ", mem);
writeln(" in bytes: ", nBytes);
writeln("    in kb: ", kb);
writeln("    in mb: ", mb);
writeln("    in gb: ", gb);

output(int(64));
output(uint(64));
output(real);
