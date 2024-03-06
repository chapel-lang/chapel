require "c_sin.c";
use Random;
use Time;
use Math;
use CTypes;

config const seed = 314159;
config const correctness = false;

extern proc c_sin(size: uint(32), iterations: int(32), resArray: c_ptr(real(32))): real(32);
extern proc c_table(size: uint(32), iterations: int(32), resArray: c_ptr(real(32))): real(32);

proc sin_calc(size: uint(32), iterations : int(32)): real(32) {

    var answer: [0..<iterations] real(32);
    var randStream = new randomStream(uint(32), seed);
    var random_number: uint(32);
    for i in 0..<iterations {
        random_number = randStream.next() % (size-1);
        answer[i] = 2 * 6.28 * sin(2.0 * 3.1415927 * random_number:real(32) / size:real(32));
    }
    return answer[(randStream.next() % (iterations-1)):int(32)];

}

proc fillTable(size: uint(32))
{
    var table :[0..<size] real(32);
    for i in 0..size-1{
        table[i] = sin(2.0 * 3.1415927 * i:real(32) / size:real(32));
    }
    return table;
}

proc fast_sin(size: uint(32), iterations : int(32)): real(32) {

    var answer: [0..<iterations] real(32);
    var randStream = new randomStream(uint(32), seed);
    var random_number: uint(32);
    var table = fillTable(size);

    for i in 0..<iterations {
        random_number = randStream.next() % (size-1);
        answer[i] = 2 * 6.28 * table(random_number);
    }
    return answer[(randStream.next() % (iterations-1)):int(32)];
}

proc main()
{
    var iterations : int(32) = 100_000_000;
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
    var c_calc_res = c_sin(size, iterations, resArray);
    c_calc.stop();
    write("C Calculated: ");
    if correctness
      then writeln(c_calc_res);
      else writeln(c_calc.elapsed(), " seconds");

    c_lookup.start();
    var table_calc_res = c_table(size, iterations, resArray);
    c_lookup.stop();
    write("C Lookup: ");
    if correctness
      then writeln(table_calc_res);
      else writeln(c_lookup.elapsed(), " seconds");

    deallocate(resArray);
}
