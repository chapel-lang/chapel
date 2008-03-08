enum MemUnits {Bytes, KB, MB, GB};

def locale.physicalMemory(unit: MemUnits=MemUnits.Bytes, type retType=int(64)) {

  /*on this do*/ const bytesInLocale: uint(64) = __primitive("chpl_bytesPerLocale");

  var retVal: retType;
  select (unit) {
    when MemUnits.Bytes do retVal = bytesInLocale:retType;
    when MemUnits.KB do retVal = (bytesInLocale:retType / 1024):retType;
    when MemUnits.MB do retVal = (bytesInLocale:retType / (1024**2)):retType;
    when MemUnits.GB do retVal = (bytesInLocale:retType / (1024**3)):retType;
  }

  return retVal;
}

def memoryUsed()
  return __primitive("_mem_used");
