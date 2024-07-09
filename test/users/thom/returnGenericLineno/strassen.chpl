module Strassen
{
    use Random;
    use Time;
    use Rosslyn;
    use Util;
    private use IO;

    class StrassenFactory : BenchmarkFactory
   , writeSerializable {
        var n : int;

        override proc getInstance() : unmanaged Strassen  // Error message line# wrong
        {
            return new unmanaged Strassen("name", false, n);
        }
        override proc serialize(writer, ref serializer) throws
        {
          writer.write("StrassenFactory ", n);
        }

    }

    class Strassen : Benchmark
   , writeSerializable {
        var n;
        proc serialize(writer, ref serializer) throws
        {
          writer.write("Strassen ", n);
        }
        override proc runKernel()
        {
        }
    }
}
