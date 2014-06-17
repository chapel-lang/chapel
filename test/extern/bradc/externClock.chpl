type chpl_clock_t = int(32);

config const printTimings = true;

extern proc clock(): chpl_clock_t;
extern proc get_CLOCKS_PER_SEC(): chpl_clock_t;

use Time;  // to get sleep() function

config const numSpins = 1000000;
const chpl_CLOCKS_PER_SEC = get_CLOCKS_PER_SEC();

proc main() {
  var start, finish: chpl_clock_t;

  writeln("About to spin ", numSpins, " times...");
  start = clock();
  for i in 1..numSpins do
    write("");
  finish = clock();

  if (printTimings) {
    writeln("start  = ", start);
    writeln("finish = ", finish);
    writeln("delta  = ", finish - start);

    writeln("time = ", (finish - start):real(64) / chpl_CLOCKS_PER_SEC);
  }
}
