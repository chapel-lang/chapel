config const doMemLeaksTest = true;
config const verboseMem = false;
config const verboseMemLeaks = false;

public use Memory;
var totalMemLeaked = 0:uint(64);

proc computeMemTrackOverhead() {
  const m0 = memoryUsed();
  {
    var mu: [0..#numLocales] uint(64);
    const m1 = memoryUsed();
    mu; // keep it alive
    return m1-m0;
  }
}

const memTrackOverhead = if CHPL_COMM=="none" then 0
                         else computeMemTrackOverhead();

proc allMemoryUsed(first=true) {
  if !first && verboseMem then stopVerboseMem();

  if CHPL_COMM == "none" {
    if first && verboseMem then startVerboseMem();
    if !first && verboseMemLeaks then printMemAllocsByType();
    return memoryUsed();
  } else {
    var mu: [0..#numLocales] uint(64);
    for loc in Locales do on loc do mu[here.id] = memoryUsed();
    if first && verboseMem then startVerboseMem();
    if !first && verboseMemLeaks then
      for loc in Locales do on loc do printMemAllocsByType();
    if !first then mu[0] -= memTrackOverhead;
    return mu;
  }
}

proc checkMemLeaks(m0) {
  const m1 = allMemoryUsed(false);
  const memLeaked = if CHPL_COMM=="none" then m1-m0 else + reduce (m1-m0);
  if memLeaked != 0 {
    if doMemLeaksTest then writeln(memLeaked, " bytes leaked");
    totalMemLeaked += memLeaked;
  }
}
