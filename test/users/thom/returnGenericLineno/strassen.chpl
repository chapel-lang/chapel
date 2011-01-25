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
            return new Strassen(n);
        }
    }

    class Strassen : Benchmark
    {
        var n;
    }
}
