// This program exercises the --cache-remote data structure
// in a manner similar to the RA-rmo benchmark.

use SysCTypes;
use Time;
use Random;

extern proc chpl_cache_print_stats();
extern proc chpl_cache_mock_get(node:c_int, raddr:uint(64), size:size_t):c_int;

config const seed = 97;
config const nLocales:uint = 512;
config const spacePerLocale:uint = 4*1024*1024*1024;
config const numUpdates = 10000;
config const basePtr:uint = 0x7ffff5931010;
config const printStats = false;

proc main() {
  var rng = new RandomStream(uint, seed, parSafe=false);

  var t:Timer;

  if printStats {
    writeln("Number of Locales = ", nLocales);
    writeln("Total memory required (GB) = ",
            nLocales*spacePerLocale/1024.0/1024.0/1024.0);
    writeln("Number of updates = ", numUpdates);
  }

  t.start();
  for update in 0..<numUpdates {
    const node = rng.getNext(min=0, max=nLocales-1);
    const addr = rng.getNext(min=0, max=spacePerLocale-1);
    chpl_cache_mock_get(node:c_int, basePtr+addr, 8);
    if printStats && update % (numUpdates/3) == 0 {
      writeln("After ", update+1, " updates");
      chpl_cache_print_stats();
    }
  }
  t.stop();

  if printStats {
    writeln("Execution time = ", t.elapsed());
    writeln("µs per update = ", 1000.0 * 1000.0 * t.elapsed() / numUpdates);
  }
}
