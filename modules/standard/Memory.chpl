pragma "insert line file info" extern proc chpl_memoryUsed(): uint(64);

enum MemUnits {Bytes, KB, MB, GB};

proc locale.physicalMemory(unit: MemUnits=MemUnits.Bytes, type retType=int(64)) {
  extern proc chpl_bytesPerLocale(): uint(64);

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
  pragma "insert line file info" 
  extern proc chpl_printMemTable(thresh);

  chpl_printMemTable(thresh);
}

proc printMemLeaksTable() {
  extern proc chpl_printLeakedMemTable();
  chpl_printLeakedMemTable();
}

proc printMemStat() {
  pragma "insert line file info"
  extern proc chpl_printMemStat();

  chpl_printMemStat();
}

proc startVerboseMem() { 
  extern proc chpl_startVerboseMem();
  chpl_startVerboseMem();
}

proc stopVerboseMem() {
  extern proc chpl_stopVerboseMem();
  chpl_stopVerboseMem();
}

proc startVerboseMemHere() {
  extern proc chpl_startVerboseMemHere();
  chpl_startVerboseMemHere();
}

proc stopVerboseMemHere() {
  extern proc chpl_stopVerboseMemHere();
  chpl_stopVerboseMemHere();
}
