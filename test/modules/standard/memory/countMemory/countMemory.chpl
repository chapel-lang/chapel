use Memory;

proc output(type retType) {
  const mem = rootLocale.getLocales()(0).physicalMemory(retType = retType),
    bytes = rootLocale.getLocales()(0).physicalMemory(MemUnits.Bytes, retType),
    kb = rootLocale.getLocales()(0).physicalMemory(MemUnits.KB, retType),
    mb = rootLocale.getLocales()(0).physicalMemory(MemUnits.MB, retType),
    gb = rootLocale.getLocales()(0).physicalMemory(MemUnits.GB, retType);

  writeln("memory is: ", mem);
  writeln(" in bytes: ", bytes);
  writeln("    in kb: ", kb);
  writeln("    in mb: ", mb);
  writeln("    in gb: ", gb);
}

const mem = rootLocale.getLocales()(0).physicalMemory(),
  bytes = rootLocale.getLocales()(0).physicalMemory(MemUnits.Bytes),
  kb = rootLocale.getLocales()(0).physicalMemory(MemUnits.KB),
  mb = rootLocale.getLocales()(0).physicalMemory(MemUnits.MB),
  gb = rootLocale.getLocales()(0).physicalMemory(MemUnits.GB);

writeln("memory is: ", mem);
writeln(" in bytes: ", bytes);
writeln("    in kb: ", kb);
writeln("    in mb: ", mb);
writeln("    in gb: ", gb);

output(int(64));
output(uint(64));
output(real);
