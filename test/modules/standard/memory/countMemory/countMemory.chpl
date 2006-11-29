use Memory;

def output(type retType) {
  const mem = Locale(0).physicalMemory(retType = retType),
        bytes = Locale(0).physicalMemory(Bytes, retType),
        kb = Locale(0).physicalMemory(KB, retType),
        mb = Locale(0).physicalMemory(MB, retType),
        gb = Locale(0).physicalMemory(GB, retType);

  writeln("memory is: ", mem);
  writeln(" in bytes: ", bytes);
  writeln("    in kb: ", kb);
  writeln("    in mb: ", mb);
  writeln("    in gb: ", gb);
}

const mem = Locale(0).physicalMemory(),
      bytes = Locale(0).physicalMemory(Bytes),
      kb = Locale(0).physicalMemory(KB),
      mb = Locale(0).physicalMemory(MB),
      gb = Locale(0).physicalMemory(GB);

writeln("memory is: ", mem);
writeln(" in bytes: ", bytes);
writeln("    in kb: ", kb);
writeln("    in mb: ", mb);
writeln("    in gb: ", gb);

output(int(64));
output(uint(64));
output(real);
