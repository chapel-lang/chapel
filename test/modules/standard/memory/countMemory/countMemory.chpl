use Memory;

def output(type retType) {
  const mem = physicalMemory(Locale(0), retType = retType),
        bytes = physicalMemory(Locale(0), Bytes, retType),
        kb = physicalMemory(Locale(0), KB, retType),
        mb = physicalMemory(Locale(0), MB, retType),
        gb = physicalMemory(Locale(0), GB, retType);

  writeln("memory is: ", mem);
  writeln(" in bytes: ", bytes);
  writeln("    in kb: ", kb);
  writeln("    in mb: ", mb);
  writeln("    in gb: ", gb);
}

const mem = physicalMemory(Locale(0)),
      bytes = physicalMemory(Locale(0), Bytes),
      kb = physicalMemory(Locale(0), KB),
      mb = physicalMemory(Locale(0), MB),
      gb = physicalMemory(Locale(0), GB);

writeln("memory is: ", mem);
writeln(" in bytes: ", bytes);
writeln("    in kb: ", kb);
writeln("    in mb: ", mb);
writeln("    in gb: ", gb);

output(int(64));
output(uint(64));
output(real);
