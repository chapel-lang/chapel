// Run an RA-like pattern to stress --cache-remote
// on all cores and report the overhead per operation.

use SysCTypes;
use Time;
use Random;

extern proc chpl_cache_print_stats();
extern proc chpl_cache_mock_get(node:c_int, raddr:uint(64), size:size_t):c_int;

config const printStats = true;
config const seed = 101;
config const numUpdates = 1000000;
config const basePtr:uint = 0x7ffff5931010;

proc run(name:string,
         nLocales:uint = 512,
         spacePerLocale:uint = 4*1024*1024*1024,
         threads = here.numPUs(logical=false)) {

  if printStats {
    writeln();
    writeln("Running mock-ra with configuration ", name);
    writeln("Number of Locales = ", nLocales);
    writeln("Bytes per Locale = ", spacePerLocale);
    writeln("Total memory simulated (GB) = ",
            nLocales*spacePerLocale/1024.0/1024.0/1024.0);
    writeln("Number of updates = ", numUpdates);
    writeln("Number of tasks = ", threads);
  }

  coforall task in 0..#threads {
    var rng = new RandomStream(uint, seed, parSafe=false);
    var t:Timer;
    t.start();
    for update in 0..<numUpdates {
      const node = rng.getNext(min=0, max=nLocales-1);
      const addr = rng.getNext(min=0, max=spacePerLocale-1);
      chpl_cache_mock_get(node:c_int, basePtr+addr, 8);
    }
    t.stop();
    if task == threads-1 && printStats {
      writeln(name, "-time ", t.elapsed());
      writeln(name, "-microseconds/update ",
              1000.0 * 1000.0 * t.elapsed() / numUpdates);
    }
  }
}

run("tiny",    nLocales=16,        spacePerLocale=16); 
run("smallA",  nLocales=16,        spacePerLocale=32); 
run("smallB",  nLocales=32,        spacePerLocale=16); 
run("mediumA", nLocales=64,        spacePerLocale=1024*1024); 
run("mediumB", nLocales=1024*1024, spacePerLocale=64);
run("largeA",  nLocales=1024,      spacePerLocale=1024*1024*1024); 
run("largeB",  nLocales=1024*1024, spacePerLocale=1024*1024);
run("massive", nLocales=1024*1024, spacePerLocale=1024*1024*1024*1024); 
