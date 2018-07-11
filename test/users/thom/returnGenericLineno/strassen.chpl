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
        proc writeThis(w)
        {
          w <~> "StrassenFactory " <~> n;
        }

    }

    class Strassen : Benchmark
    {
        var n;
        proc writeThis(w)
        {
          w <~> "Strassen " <~> n;
        }
        proc runKernel()
        {
        }
    }
}
