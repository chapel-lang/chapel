enum MemUnits {Bytes, KB, MB, GB};

def physicalMemory(loc: locale, unit: MemUnits=Bytes, type retType=uint(64)) {
  on loc do const bytesInLocale: uint(64) = __primitive("_bytesPerLocale");
  var retVal: retType;
  select (unit) {
    when Bytes do retVal = bytesInLocale:retType;
    when KB do retVal = (bytesInLocale:retType / 1024):retType;
    when MB do retVal = (bytesInLocale:retType / (1024**2)):retType;
    when GB do retVal = (bytesInLocale:retType / (1024**3)):retType;
  }
  return retVal;
}
