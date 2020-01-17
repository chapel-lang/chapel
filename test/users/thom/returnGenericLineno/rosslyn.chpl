/*
 Rosslyn Chapel Benchmarking Suite wotsit

 The plan:
 * Implement a bunch of parameterised benchmarks, extending from some base
   Benchmark class.

*/
// Whole bunch of stuff for benchmarking
module Rosslyn
{
    use Time;

    /** If we had the facilities, this class would be abstract. */

    class Benchmark
    {
        var name = "<Default Benchmark>"; //override this in subclass.

        var hasBeenRun = false;

        proc runKernel()
        {
            assert(false,"runKernel() method not overridden in Benchmark ",
                   name," subclass");
        }

        proc validate() : bool
        {
            writeln("WARNING: No validation code written, assuming run was valid");
            return true;
        }

        /* Time runKernel() method, return results in ms or given time unit*/
        proc timeKernel(units = TimeUnits.milliseconds) : real
        {
            assert(!hasBeenRun,"Benchmark instance has already been run, ",
                               "create new class instance");
            hasBeenRun = true;

            var timer : Timer;

            timer.start();
            runKernel(); //assume there is minimal overhead here
            timer.stop();

            assert(validate(),"Benchmark run did not validate");

            return timer.elapsed(units);
        }

    }


    class BenchmarkFactory
    {
        //abstract
        proc getInstance() : unmanaged Benchmark
        {
            assert(false,"BenchmarkFactory.getInstance() should be",
                         "overridden in the subclass");
            return new unmanaged Benchmark();
        }


        proc writeThis(w) throws
        {
            assert(false,"BenchmarkFactory.writeThis() should be",
                         "overridden in the subclass");
        }
    }


    class BenchmarkRunner
    {
        var factory : unmanaged BenchmarkFactory;
        var repeats : int;


        /** Explicit form of default constructor */
        proc init(factory : unmanaged BenchmarkFactory, repeats = 5 )
        {
            this.factory = factory;
            this.repeats = repeats;
        }

        proc runBenchmark()// : ResultSet
        {
            var benchmark : unmanaged Benchmark?;
            writeln("Running benchmark: \"",factory,"\", ",repeats," runs");
            var results : [1..repeats] real;//TimeResult;
            for run in 1..repeats
            {

                benchmark = factory.getInstance();

                results[run] = benchmark!.timeKernel();
                writeln("Run[",run,"]: ",results[run]);
            }


            writeln("Min: ",min reduce results," ",
                    "Avg: ",(+ reduce results)/results.numElements);

        }

    }
}
