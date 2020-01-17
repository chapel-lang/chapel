module OuterModule {
  // When ascii() is removed, remember to rename __primitive("ascii") as well.
  // ... and remove ascii for c_string

  module unitTest {

    inline proc f(s, useExpr=false) {
      if useExpr {
        writeMe(ascii(s));
      } else {
        const aStr = ascii(s);
        writeMe(aStr);
      }
    }

    proc asciiLocal(type t, useExpr=false) {
      writeln("=== ascii local");
      const m0 = allMemoryUsed();
      {
        const s0:t = "s";
        const s1:t = "t";
        f(s0); f(s1);
      }
      checkMemLeaks(m0);
    }

    proc asciiRemote(type t, useExpr=false) {
      writeln("=== ascii remote");
      const m0 = allMemoryUsed();
      {
        const s0:t = "s";
        const s1:t = "t";
        on Locales[numLocales-1] {
          f(s0); f(s1);
        }
      }
      checkMemLeaks(m0);
    }

    proc doIt(type t) {
      asciiLocal(t); asciiLocal(t, useExpr=true);
      asciiRemote(t); asciiRemote(t, useExpr=true);
    }

  }

  config type string_impl = string;

  config const doMemLeaksTest = true;
  config const verboseMem = false;
  config const verboseMemLeaks = false;

  use Memory;
  var totalMemLeaked = 0:uint(64);

  proc computeMemTrackOverhead() {
    const m0 = memoryUsed();
    {
      var mu: [LocaleSpace] uint(64);
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
      var mu: [LocaleSpace] uint(64);
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

  config const doCorrectnessTest = true;

  enum E { zero=-1, one=-2, two=-4, three=-8 };

  inline proc writeMe(s) {
    if doCorrectnessTest then writeln(s);
  }

  use unitTest;
  proc main() {
    doIt(string_impl);
    if totalMemLeaked > 0 then printMemAllocsByType();
  }
}
