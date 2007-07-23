enum MemUnits {Bytes, KB, MB, GB};

def locale.physicalMemory(unit: MemUnits=Bytes, type retType=int(64)) {

  /*on this do*/ const bytesInLocale: uint(64) = __primitive("_bytesPerLocale");

  var retVal: retType;
  select (unit) {
    when Bytes do retVal = bytesInLocale:retType;
    when KB do retVal = (bytesInLocale:retType / 1024):retType;
    when MB do retVal = (bytesInLocale:retType / (1024**2)):retType;
    when GB do retVal = (bytesInLocale:retType / (1024**3)):retType;
  }

  return retVal;
}

def memoryUsed()
  return __primitive("_mem_used");
