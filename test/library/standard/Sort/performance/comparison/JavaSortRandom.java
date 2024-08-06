// javac JavaSortRandom.java 
// java JavaSortRandom

import java.util.Arrays;
import java.util.Random;

public class JavaSortRandom {

  public static void main(String[] args) {
    int n = 128*1024*1024;

    System.out.println("Creating Array");
    long[] arr = new long[n];
    Random rng = new Random();
    for (int i = 0; i < n; i++) {
      arr[i] = rng.nextLong();
    }
    System.out.println("Sorting");
    long start = System.nanoTime();
    Arrays.sort(arr);
    long end = System.nanoTime();
    double elapsed = (end-start)/1000.0/1000.0/1000.0;
    System.out.println("Sorting took " + elapsed + " seconds");
    System.out.println("MiB/s: " + (8*n/elapsed/1024.0/1024.0));
    System.out.println("million elements sorted per second: " +
                       (n/elapsed/1000.0/1000.0));
  }
}
