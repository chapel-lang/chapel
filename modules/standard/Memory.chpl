enum MemUnits {Bytes, KB, MB, GB};

def locale.physicalMemory(unit: MemUnits=MemUnits.Bytes, type retType=int(64)) {
  _extern def chpl_bytesPerLocale(): uint(64);
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

def memoryUsed() {
  _extern def chpl_memoryUsed(lineno: int(32), filename: string): uint(64);
  return __primitive("chpl_memoryUsed");
}

def printMemTable(thresh=0) {
  __primitive("chpl_printMemTable", thresh);
}

def printMemStat() {
  __primitive("chpl_printMemStat");
}

def startVerboseMem() { 
  _extern def chpl_startVerboseMem();
  chpl_startVerboseMem();
}

def stopVerboseMem() {
  _extern def chpl_stopVerboseMem();
  chpl_stopVerboseMem();
}

def startVerboseMemHere() {
  _extern def chpl_startVerboseMemHere();
  chpl_startVerboseMemHere();
}

def stopVerboseMemHere() {
  _extern def chpl_stopVerboseMemHere();
  chpl_stopVerboseMemHere();
}
