module Strassen_Main
{
    use Strassen;
    use Rosslyn;

    config var n = 4;

    proc main()
    {
        var factory = new unmanaged StrassenFactory(n);
        var runner = new unmanaged BenchmarkRunner(factory,10);

        runner.runBenchmark();
    }
}
