use Memory;

def output(type retType) {
  const mem = Locales(0).physicalMemory(retType = retType),
        bytes = Locales(0).physicalMemory(Bytes, retType),
        kb = Locales(0).physicalMemory(KB, retType),
        mb = Locales(0).physicalMemory(MB, retType),
        gb = Locales(0).physicalMemory(GB, retType);

  writeln("memory is: ", mem);
  writeln(" in bytes: ", bytes);
  writeln("    in kb: ", kb);
  writeln("    in mb: ", mb);
  writeln("    in gb: ", gb);
}

const mem = Locales(0).physicalMemory(),
      bytes = Locales(0).physicalMemory(Bytes),
      kb = Locales(0).physicalMemory(KB),
      mb = Locales(0).physicalMemory(MB),
      gb = Locales(0).physicalMemory(GB);

writeln("memory is: ", mem);
writeln(" in bytes: ", bytes);
writeln("    in kb: ", kb);
writeln("    in mb: ", mb);
writeln("    in gb: ", gb);

output(int(64));
output(uint(64));
output(real);
