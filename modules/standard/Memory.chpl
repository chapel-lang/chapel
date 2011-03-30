pragma "insert line file info" _extern proc chpl_printMemTable(thresh);
pragma "insert line file info" _extern proc chpl_printMemStat();
_extern proc chpl_startVerboseMem();
_extern proc chpl_stopVerboseMem();
_extern proc chpl_startVerboseMemHere();
_extern proc chpl_stopVerboseMemHere();
_extern proc chpl_bytesPerLocale(): uint(64);
pragma "insert line file info" _extern proc chpl_memoryUsed(): uint(64);

enum MemUnits {Bytes, KB, MB, GB};

proc locale.physicalMemory(unit: MemUnits=MemUnits.Bytes, type retType=int(64)) {
  var bytesInLocale: uint(64);

  on this do bytesInLocale = chpl_bytesPerLocale();

  var retVal: retType;
  select (unit) {
    when MemUnits.Bytes do retVal = bytesInLocale:retType;
    when MemUnits.KB do retVal = (bytesInLocale:retType / 1024):retType;
    when MemUnits.MB do retVal = (bytesInLocale:retType / (1024**2)):retType;
    when MemUnits.GB do retVal = (bytesInLocale:retType / (1024**3)):retType;
  }

  return retVal;
}

proc memoryUsed() {
  return chpl_memoryUsed();
}

proc printMemTable(thresh=0) {
  chpl_printMemTable(thresh);
}

proc printMemStat() {
  chpl_printMemStat();
}

proc startVerboseMem() { 
  chpl_startVerboseMem();
}

proc stopVerboseMem() {
  chpl_stopVerboseMem();
}

proc startVerboseMemHere() {
  chpl_startVerboseMemHere();
}

proc stopVerboseMemHere() {
  chpl_stopVerboseMemHere();
}
