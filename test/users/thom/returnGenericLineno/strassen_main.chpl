use Strassen;
use Rosslyn;


module Strassen_Main
{
    config var n = 4;

    def main()
    {
        var factory = new StrassenFactory(n);
        var runner = new BenchmarkRunner(factory,10);

        runner.runBenchmark();
    }
}
