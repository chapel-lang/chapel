//// to run this on linux
// sudo apt-get install mono-complete

// mcs -optimize -out:CSharpSortRandom.exe CSharpSortRandom.cs
// mono CSharpSortRandom.exe

using System;
using System.Diagnostics;

class CSharpSortRandom {
  private static void Main() {
    Console.WriteLine("Creating Random Array");
    Random rng = new Random();
    int n = 128*1024*1024;
    long[] arr = new long[n];
    for (int i = 0; i < n; i++) {
      int low = rng.Next();
      int high = rng.Next();
      arr[i] = (high << 32) | low;
    }

    Console.WriteLine("Sorting");
    Stopwatch stopwatch = Stopwatch.StartNew();
    Array.Sort(arr);
    stopwatch.Stop();
    double elapsed = stopwatch.Elapsed.TotalSeconds;
    Console.WriteLine("Sorting Time Elapsed " + elapsed + " s");
    Console.WriteLine((8*n/elapsed/1024.0/1024.0) + " MiB/s");
    Console.WriteLine((n/elapsed/1000.0/1000.0) +
                      " million elements sorted per second");
  }
}
