require "c_sin.c";
use Random;
use Time;
use Math;
use CTypes;
use ReplicatedDist;

config const seed: uint(32) = 314159;
config const correctness = false;

extern proc c_sin(size: uint(32),
                  iterations: int(32),
                  seed: uint(32),
                  resArray: c_ptr(real(32))): real(32);
extern proc c_table(size: uint(32),
                    iterations: int(32),
                    seed: uint(32),
                    resArray: c_ptr(real(32))): real(32);

proc sin_calc(size: uint(32), iterations : int(32)): real(32) {

    var answer: [0..#numLocales] real(32);

    coforall loc in Locales {
        on loc {
            var locale_timer = new stopwatch();
            locale_timer.start();
            var localAnswer: [0..<iterations] real(32);
            var randStream = new randomStream(uint(32), seed);
            var random_number: uint(32);
            for i in 0..<iterations {
                random_number = randStream.next() % (size-1);
                localAnswer[i] = 2 * 6.28 * sin(2.0 * 3.1415927 * random_number:real(32) / size:real(32));
            }
            locale_timer.stop();
            if !correctness then
              writeln("Calc Locale ", loc, " Timer: ", locale_timer.elapsed(), " seconds");
            answer[loc.id] = localAnswer[(randStream.next() % (iterations-1)):int(32)];
        }
    }
    var randStream = new randomStream(uint(32), seed);
    const idx = if numLocales == 1
                  then 0
                  else (randStream.next() % (numLocales-1)):int(32);
    return answer[idx];
}

proc fillTable(size: uint(32))
{
    const Space = {0..<size};
    const ReplicatedSpace = Space dmapped new replicatedDist();
    var RA: [ReplicatedSpace] real(32);

    coforall loc in Locales {
        on loc {
            for i in 0..size-1{
                RA.replicand(loc)[i] = sin(2.0 * 3.1415927 * i:real(32) / size:real(32));
            }
        }
    }
    return RA;
}

proc fast_sin(size: uint(32), iterations : int(32)): real(32) {

    var table = fillTable(size);

    var answer: [0..#numLocales] real(32);

    coforall loc in Locales {
        on loc {
            var locale_timer = new stopwatch();
            locale_timer.start();
            var localAnswer: [0..<iterations] real(32);
            var randStream = new randomStream(uint(32), seed);
            var random_number: uint(32);
            for i in 0..<iterations {
                random_number = randStream.next() % (size-1);
                localAnswer[i] = 2 * 6.28 * table.replicand(loc)(random_number);
            }
            locale_timer.stop();
            if !correctness then
              writeln("Lookup Locale ", loc, " Timer: ", locale_timer.elapsed(), " seconds");
            answer[loc.id] = localAnswer[(randStream.next() % (iterations-1)):int(32)];
        }
    }
    var randStream = new randomStream(uint(32), seed);
    const idx = if numLocales == 1
                  then 0
                  else (randStream.next() % (numLocales-1)):int(32);
    return answer[idx];}

proc main()
{
    var iterations = if !correctness then 100_000_000:int(32) else 1_000:int(32);
    const size:uint(32) = 256;

    var calc = new stopwatch();
    var table = new stopwatch();

    var c_calc = new stopwatch();
    var c_lookup = new stopwatch();

    calc.start();
    var calc_res = sin_calc(size, iterations);
    calc.stop();
    write("Chapel Calculated: ");
    if correctness
      then writeln(calc_res);
      else writeln(calc.elapsed(), " seconds");
    
    table.start();
    var table_res = fast_sin(size, iterations);
    table.stop();
    write("Chapel Lookup: ");
    if correctness
      then writeln(table_res);
      else writeln(table.elapsed(), " seconds");

    var resArray = allocate(real(32), iterations);

    c_calc.start();
    var c_calc_res = c_sin(size, iterations, seed, resArray);
    c_calc.stop();
    write("C Calculated: ");
    if correctness
      then writeln(c_calc_res);
      else writeln(c_calc.elapsed(), " seconds");

    c_lookup.start();
    var table_calc_res = c_table(size, iterations, seed, resArray);
    c_lookup.stop();
    write("C Lookup: ");
    if correctness
      then writeln(table_calc_res);
      else writeln(c_lookup.elapsed(), " seconds");

    deallocate(resArray);
}
