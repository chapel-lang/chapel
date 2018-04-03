module Strassen
{
    use Random;
    use Time;
    use Rosslyn;
    use Util;

    class StrassenFactory : BenchmarkFactory
    {
        var n : int;

        proc getInstance() : Strassen  // Error message line# wrong
        {
            return new Strassen("name", false, n);
        }
    }

    class Strassen : Benchmark
    {
        var n;
    }
}
