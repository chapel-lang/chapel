pragma "insert line file info" _extern def chpl_printMemTable(thresh);
pragma "insert line file info" _extern def chpl_printMemStat();
_extern def chpl_startVerboseMem();
_extern def chpl_stopVerboseMem();
_extern def chpl_startVerboseMemHere();
_extern def chpl_stopVerboseMemHere();
_extern def chpl_bytesPerLocale(): uint(64);
pragma "insert line file info" _extern def chpl_memoryUsed(): uint(64);

enum MemUnits {Bytes, KB, MB, GB};

def locale.physicalMemory(unit: MemUnits=MemUnits.Bytes, type retType=int(64)) {
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
  return chpl_memoryUsed();
}

def printMemTable(thresh=0) {
  chpl_printMemTable(thresh);
}

def printMemStat() {
  chpl_printMemStat();
}

def startVerboseMem() { 
  chpl_startVerboseMem();
}

def stopVerboseMem() {
  chpl_stopVerboseMem();
}

def startVerboseMemHere() {
  chpl_startVerboseMemHere();
}

def stopVerboseMemHere() {
  chpl_stopVerboseMemHere();
}
