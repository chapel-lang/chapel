// This program exercises the --cache-remote data structure
// with two concurrent configurable update streams.
// This is meant to verify that the 2Q behavior is working
//  (where random updates to a small space will end up in Am).

// stream one runs numUpdates.
// stream two runs only an update every once in a while.
// In particular it runs an update only once for every streamTwoUpdateRate
// updates in stream one.

use SysCTypes;
use Time;
use Random;

extern proc chpl_cache_print_stats();
extern proc chpl_cache_mock_get(node:c_int, raddr:uint(64), size:size_t):c_int;
extern proc printf(fmt:c_string, arg:c_int);

config const seed = 97;
config const streamOneLocales:uint = 512;
config const streamOneSpacePerLocale:uint = 4*1024*1024*1024;
config const streamTwoLocales:uint = 1;
config const streamTwoSpacePerLocale:uint = 1;
config const streamTwoUpdateRate = 500;
config const numUpdates = 50000;
config const basePtr:uint = 0x7ffff5931010;
config const printStats = false;
config const printMissRate = false;

proc main() {
  var rng = new RandomStream(uint, seed);
  var streamOneUpdates = 0;
  var streamOneMisses = 0;
  var streamTwoUpdates = 0;
  var streamTwoMisses = 0;

  var t:Timer;

  if printStats || printMissRate {
    writeln("streamOneLocales = ", streamOneLocales);
    writeln("streamOneSpacePerLocale = ", streamOneSpacePerLocale);
    writeln("streamTwoLocales = ", streamTwoLocales);
    writeln("streamTwoSpacePerLocale = ", streamTwoSpacePerLocale);
    writeln("streamTwoUpdateRate = ", streamTwoUpdateRate);
    writeln("Number of updates = ", numUpdates);
  }

  // Compute the random values
  var streamOneNodes:[0..<numUpdates] uint;
  var streamOneAddrs:[0..<numUpdates] uint;
  var streamTwoNodes:[0..<numUpdates] uint;
  var streamTwoAddrs:[0..<numUpdates] uint;
  for update in 0..<numUpdates {
    // add numLocales to each dest update to make each stored in cache
    // (cache ignores updates to current locale)
    const nl = numLocales:uint;
    {
      const node = nl+rng.getNext(min=0, max=streamOneLocales-1);
      const addr = basePtr+rng.getNext(min=0, max=streamOneSpacePerLocale-1);
      streamOneNodes[update] = node;
      streamOneAddrs[update] = addr;
    }
    {
      const node = nl+rng.getNext(min=0, max=streamTwoLocales-1);
      const addr = basePtr+rng.getNext(min=0, max=streamTwoSpacePerLocale-1);
      streamTwoNodes[update] = node;
      streamTwoAddrs[update] = addr;
    }
  }

  t.start();
  for update in 0..<numUpdates {
    {
      // Stream 1 update
      const node = streamOneNodes[update];
      const addr = streamOneAddrs[update];
      const cached = chpl_cache_mock_get(node:c_int, addr, 8);
      streamOneUpdates += 1;
      if cached == 0 then
        streamOneMisses += 1;
    }
    if update % streamTwoUpdateRate == 0 {
      // Stream 2 update
      const node = streamTwoNodes[update];
      const addr = streamTwoAddrs[update];
      const cached = chpl_cache_mock_get(node:c_int, addr, 8);
      streamTwoUpdates += 1;
      if cached == 0 then
        streamTwoMisses += 1;
    }
    if printStats && update % (numUpdates/3) == 0 {
      printf("After %i updates\n", (update+1):c_int);
      chpl_cache_print_stats();
    }
  }
  t.stop();

  if printStats {
    writeln("Execution time = ", t.elapsed());
    writeln("µs per update = ", 1000.0 * 1000.0 * t.elapsed() / numUpdates);
  }

  if printStats || printMissRate {
    writeln("streamOneUpdates = ", streamOneUpdates);
    writeln("streamOneMisses = ", streamOneMisses);
    writeln("streamTwoUpdates = ", streamTwoUpdates);
    writeln("streamTwoMisses = ", streamTwoMisses);
    writeln();
    writef("stream one miss rate = %{#.##}\n",
           streamOneMisses * 1.0 / streamOneUpdates);
    writef("stream two miss rate = %{#.##}\n",
           streamTwoMisses * 1.0 / streamTwoUpdates);
  }
}
