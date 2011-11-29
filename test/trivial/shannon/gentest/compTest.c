#include <stdio.h>

main() {

  int m = 2;
  int n = 3;
  int o = 5;
  int answer = 0;

  printf("* and * permutations\n");
  printf("=========================\n");
  answer = m * n * o;
  printf("%d\n", answer);
  answer = (m * n) * o;
  printf("%d\n", answer);
  answer = m * (n * o);
  printf("%d\n\n", answer);
  
  printf("* and / permutations\n");
  printf("=========================\n");
  answer = m * n / o;
  printf("%d\n", answer);
  answer = (m * n) / o;
  printf("%d\n", answer);
  answer = m * (n / o);
  printf("%d\n\n", answer);
  
  printf("* and %% permutations\n");
  printf("=========================\n");
  answer = m * n % o;
  printf("%d\n", answer);
  answer = (m * n) % o;
  printf("%d\n", answer);
  answer = m * (n % o);
  printf("%d\n\n", answer);
  
  printf("* and + permutations\n");
  printf("=========================\n");
  answer = m * n + o;
  printf("%d\n", answer);
  answer = (m * n) + o;
  printf("%d\n", answer);
  answer = m * (n + o);
  printf("%d\n\n", answer);
  
  printf("* and - permutations\n");
  printf("=========================\n");
  answer = m * n - o;
  printf("%d\n", answer);
  answer = (m * n) - o;
  printf("%d\n", answer);
  answer = m * (n - o);
  printf("%d\n\n", answer);
  
  printf("* and < permutations\n");
  printf("=========================\n");
  answer = m * n < o;
  printf("%d\n", answer);
  answer = (m * n) < o;
  printf("%d\n", answer);
  answer = m * (n < o);
  printf("%d\n\n", answer);
  
  printf("* and <= permutations\n");
  printf("=========================\n");
  answer = m * n <= o;
  printf("%d\n", answer);
  answer = (m * n) <= o;
  printf("%d\n", answer);
  answer = m * (n <= o);
  printf("%d\n\n", answer);
  
  printf("* and > permutations\n");
  printf("=========================\n");
  answer = m * n > o;
  printf("%d\n", answer);
  answer = (m * n) > o;
  printf("%d\n", answer);
  answer = m * (n > o);
  printf("%d\n\n", answer);
  
  printf("* and >= permutations\n");
  printf("=========================\n");
  answer = m * n >= o;
  printf("%d\n", answer);
  answer = (m * n) >= o;
  printf("%d\n", answer);
  answer = m * (n >= o);
  printf("%d\n\n", answer);
  
  printf("* and == permutations\n");
  printf("=========================\n");
  answer = m * n == o;
  printf("%d\n", answer);
  answer = (m * n) == o;
  printf("%d\n", answer);
  answer = m * (n == o);
  printf("%d\n\n", answer);
  
  printf("* and != permutations\n");
  printf("=========================\n");
  answer = m * n != o;
  printf("%d\n", answer);
  answer = (m * n) != o;
  printf("%d\n", answer);
  answer = m * (n != o);
  printf("%d\n\n", answer);
  
  printf("* and & permutations\n");
  printf("=========================\n");
  answer = m * n & o;
  printf("%d\n", answer);
  answer = (m * n) & o;
  printf("%d\n", answer);
  answer = m * (n & o);
  printf("%d\n\n", answer);
  
  printf("* and ^ permutations\n");
  printf("=========================\n");
  answer = m * n ^ o;
  printf("%d\n", answer);
  answer = (m * n) ^ o;
  printf("%d\n", answer);
  answer = m * (n ^ o);
  printf("%d\n\n", answer);
  
  printf("* and | permutations\n");
  printf("=========================\n");
  answer = m * n | o;
  printf("%d\n", answer);
  answer = (m * n) | o;
  printf("%d\n", answer);
  answer = m * (n | o);
  printf("%d\n\n", answer);
  
  printf("* and && permutations\n");
  printf("=========================\n");
  answer = m * n && o;
  printf("%d\n", answer);
  answer = (m * n) && o;
  printf("%d\n", answer);
  answer = m * (n && o);
  printf("%d\n\n", answer);
  
  printf("* and || permutations\n");
  printf("=========================\n");
  answer = m * n || o;
  printf("%d\n", answer);
  answer = (m * n) || o;
  printf("%d\n", answer);
  answer = m * (n || o);
  printf("%d\n\n", answer);
  
  printf("/ and * permutations\n");
  printf("=========================\n");
  answer = m / n * o;
  printf("%d\n", answer);
  answer = (m / n) * o;
  printf("%d\n", answer);
  answer = m / (n * o);
  printf("%d\n\n", answer);
  
  printf("/ and / permutations\n");
  printf("=========================\n");
  answer = m / n / o;
  printf("%d\n", answer);
  answer = (m / n) / o;
  printf("%d\n", answer);

  n = 10;
  answer = m / (n / o);
  printf("%d\n\n", answer);
  n = 3;
  
  printf("/ and %% permutations\n");
  printf("=========================\n");
  answer = m / n % o;
  printf("%d\n", answer);
  answer = (m / n) % o;
  printf("%d\n", answer);
  answer = m / (n % o);
  printf("%d\n\n", answer);
  
  printf("/ and + permutations\n");
  printf("=========================\n");
  answer = m / n + o;
  printf("%d\n", answer);
  answer = (m / n) + o;
  printf("%d\n", answer);
  answer = m / (n + o);
  printf("%d\n\n", answer);
  
  printf("/ and - permutations\n");
  printf("=========================\n");
  answer = m / n - o;
  printf("%d\n", answer);
  answer = (m / n) - o;
  printf("%d\n", answer);
  answer = m / (n - o);
  printf("%d\n\n", answer);
  
  printf("/ and < permutations\n");
  printf("=========================\n");
  answer = m / n < o;
  printf("%d\n", answer);
  answer = (m / n) < o;
  printf("%d\n", answer);
  answer = m / (n < o);
  printf("%d\n\n", answer);
  
  printf("/ and <= permutations\n");
  printf("=========================\n");
  answer = m / n <= o;
  printf("%d\n", answer);
  answer = (m / n) <= o;
  printf("%d\n", answer);
  answer = m / (n <= o);
  printf("%d\n\n", answer);
  
  printf("/ and > permutations\n");
  printf("=========================\n");
  answer = m / n > o;
  printf("%d\n", answer);
  answer = (m / n) > o;
  printf("%d\n", answer);

  n = 10;
  answer = m / (n > o);
  printf("%d\n\n", answer);
  n = 3;
  
  printf("/ and >= permutations\n");
  printf("=========================\n");
  answer = m / n >= o;
  printf("%d\n", answer);
  answer = (m / n) >= o;
  printf("%d\n", answer);

  n = 10;
  answer = m / (n >= o);
  printf("%d\n\n", answer);
  n = 3;
  
  printf("/ and == permutations\n");
  printf("=========================\n");
  answer = m / n == o;
  printf("%d\n", answer);
  answer = (m / n) == o;
  printf("%d\n", answer);

  n = 5;
  answer = m / (n == o);
  printf("%d\n\n", answer);
  n = 3;
  
  printf("/ and != permutations\n");
  printf("=========================\n");
  answer = m / n != o;
  printf("%d\n", answer);
  answer = (m / n) != o;
  printf("%d\n", answer);
  answer = m / (n != o);
  printf("%d\n\n", answer);
  
  printf("/ and & permutations\n");
  printf("=========================\n");
  answer = m / n & o;
  printf("%d\n", answer);
  answer = (m / n) & o;
  printf("%d\n", answer);
  answer = m / (n & o);
  printf("%d\n\n", answer);
  
  printf("/ and ^ permutations\n");
  printf("=========================\n");
  answer = m / n ^ o;
  printf("%d\n", answer);
  answer = (m / n) ^ o;
  printf("%d\n", answer);
  answer = m / (n ^ o);
  printf("%d\n\n", answer);
  
  printf("/ and | permutations\n");
  printf("=========================\n");
  answer = m / n | o;
  printf("%d\n", answer);
  answer = (m / n) | o;
  printf("%d\n", answer);
  answer = m / (n | o);
  printf("%d\n\n", answer);
  
  printf("/ and && permutations\n");
  printf("=========================\n");
  answer = m / n && o;
  printf("%d\n", answer);
  answer = (m / n) && o;
  printf("%d\n", answer);
  answer = m / (n && o);
  printf("%d\n\n", answer);
  
  printf("/ and || permutations\n");
  printf("=========================\n");
  answer = m / n || o;
  printf("%d\n", answer);
  answer = (m / n) || o;
  printf("%d\n", answer);
  answer = m / (n || o);
  printf("%d\n\n", answer);
  
  printf("%% and * permutations\n");
  printf("=========================\n");
  answer = m % n * o;
  printf("%d\n", answer);
  answer = (m % n) * o;
  printf("%d\n", answer);
  answer = m % (n * o);
  printf("%d\n\n", answer);
  
  printf("%% and / permutations\n");
  printf("=========================\n");
  answer = m % n / o;
  printf("%d\n", answer);
  answer = (m % n) / o;
  printf("%d\n", answer);

  n = 10;
  answer = m % (n / o);
  printf("%d\n\n", answer);
  n = 3;
  
  printf("%% and %% permutations\n");
  printf("=========================\n");
  answer = m % n % o;
  printf("%d\n", answer);
  answer = (m % n) % o;
  printf("%d\n", answer);
  answer = m % (n % o);
  printf("%d\n\n", answer);
  
  printf("%% and + permutations\n");
  printf("=========================\n");
  answer = m % n + o;
  printf("%d\n", answer);
  answer = (m % n) + o;
  printf("%d\n", answer);
  answer = m % (n + o);
  printf("%d\n\n", answer);
  
  printf("%% and - permutations\n");
  printf("=========================\n");
  answer = m % n - o;
  printf("%d\n", answer);
  answer = (m % n) - o;
  printf("%d\n", answer);
  answer = m % (n - o);
  printf("%d\n\n", answer);
  
  printf("%% and < permutations\n");
  printf("=========================\n");
  answer = m % n < o;
  printf("%d\n", answer);
  answer = (m % n) < o;
  printf("%d\n", answer);
  answer = m % (n < o);
  printf("%d\n\n", answer);
  
  printf("%% and <= permutations\n");
  printf("=========================\n");
  answer = m % n <= o;
  printf("%d\n", answer);
  answer = (m % n) <= o;
  printf("%d\n", answer);
  answer = m % (n <= o);
  printf("%d\n\n", answer);
  
  printf("%% and > permutations\n");
  printf("=========================\n");
  answer = m % n > o;
  printf("%d\n", answer);
  answer = (m % n) > o;
  printf("%d\n", answer);

  n = 10;
  answer = m % (n > o);
  printf("%d\n\n", answer);
  n = 3;
  
  printf("%% and >= permutations\n");
  printf("=========================\n");
  answer = m % n >= o;
  printf("%d\n", answer);
  answer = (m % n) >= o;
  printf("%d\n", answer);

  n = 10;
  answer = m % (n >= o);
  printf("%d\n\n", answer);
  n = 3;
  
  printf("%% and == permutations\n");
  printf("=========================\n");
  answer = m % n == o;
  printf("%d\n", answer);
  answer = (m % n) == o;
  printf("%d\n", answer);

  n = 5;
  answer = m % (n == o);
  printf("%d\n\n", answer);
  n = 3;
  
  printf("%% and != permutations\n");
  printf("=========================\n");
  answer = m % n != o;
  printf("%d\n", answer);
  answer = (m % n) != o;
  printf("%d\n", answer);
  answer = m % (n != o);
  printf("%d\n\n", answer);
  
  printf("%% and & permutations\n");
  printf("=========================\n");
  answer = m % n & o;
  printf("%d\n", answer);
  answer = (m % n) & o;
  printf("%d\n", answer);
  answer = m % (n & o);
  printf("%d\n\n", answer);
  
  printf("%% and ^ permutations\n");
  printf("=========================\n");
  answer = m % n ^ o;
  printf("%d\n", answer);
  answer = (m % n) ^ o;
  printf("%d\n", answer);
  answer = m % (n ^ o);
  printf("%d\n\n", answer);
  
  printf("%% and | permutations\n");
  printf("=========================\n");
  answer = m % n | o;
  printf("%d\n", answer);
  answer = (m % n) | o;
  printf("%d\n", answer);
  answer = m % (n | o);
  printf("%d\n\n", answer);
  
  printf("%% and && permutations\n");
  printf("=========================\n");
  answer = m % n && o;
  printf("%d\n", answer);
  answer = (m % n) && o;
  printf("%d\n", answer);
  answer = m % (n && o);
  printf("%d\n\n", answer);
  
  printf("%% and || permutations\n");
  printf("=========================\n");
  answer = m % n || o;
  printf("%d\n", answer);
  answer = (m % n) || o;
  printf("%d\n", answer);
  answer = m % (n || o);
  printf("%d\n\n", answer);
  
  printf("+ and * permutations\n");
  printf("=========================\n");
  answer = m + n * o;
  printf("%d\n", answer);
  answer = (m + n) * o;
  printf("%d\n", answer);
  answer = m + (n * o);
  printf("%d\n\n", answer);
  
  printf("+ and / permutations\n");
  printf("=========================\n");
  answer = m + n / o;
  printf("%d\n", answer);
  answer = (m + n) / o;
  printf("%d\n", answer);
  answer = m + (n / o);
  printf("%d\n\n", answer);
  
  printf("+ and %% permutations\n");
  printf("=========================\n");
  answer = m + n % o;
  printf("%d\n", answer);
  answer = (m + n) % o;
  printf("%d\n", answer);
  answer = m + (n % o);
  printf("%d\n\n", answer);
  
  printf("+ and + permutations\n");
  printf("=========================\n");
  answer = m + n + o;
  printf("%d\n", answer);
  answer = (m + n) + o;
  printf("%d\n", answer);
  answer = m + (n + o);
  printf("%d\n\n", answer);
  
  printf("+ and - permutations\n");
  printf("=========================\n");
  answer = m + n - o;
  printf("%d\n", answer);
  answer = (m + n) - o;
  printf("%d\n", answer);
  answer = m + (n - o);
  printf("%d\n\n", answer);
  
  printf("+ and < permutations\n");
  printf("=========================\n");
  answer = m + n < o;
  printf("%d\n", answer);
  answer = (m + n) < o;
  printf("%d\n", answer);
  answer = m + (n < o);
  printf("%d\n\n", answer);
  
  printf("+ and <= permutations\n");
  printf("=========================\n");
  answer = m + n <= o;
  printf("%d\n", answer);
  answer = (m + n) <= o;
  printf("%d\n", answer);
  answer = m + (n <= o);
  printf("%d\n\n", answer);
  
  printf("+ and > permutations\n");
  printf("=========================\n");
  answer = m + n > o;
  printf("%d\n", answer);
  answer = (m + n) > o;
  printf("%d\n", answer);
  answer = m + (n > o);
  printf("%d\n\n", answer);
  
  printf("+ and >= permutations\n");
  printf("=========================\n");
  answer = m + n >= o;
  printf("%d\n", answer);
  answer = (m + n) >= o;
  printf("%d\n", answer);
  answer = m + (n >= o);
  printf("%d\n\n", answer);
  
  printf("+ and == permutations\n");
  printf("=========================\n");
  answer = m + n == o;
  printf("%d\n", answer);
  answer = (m + n) == o;
  printf("%d\n", answer);
  answer = m + (n == o);
  printf("%d\n\n", answer);
  
  printf("+ and != permutations\n");
  printf("=========================\n");
  answer = m + n != o;
  printf("%d\n", answer);
  answer = (m + n) != o;
  printf("%d\n", answer);
  answer = m + (n != o);
  printf("%d\n\n", answer);
  
  printf("+ and & permutations\n");
  printf("=========================\n");
  answer = m + n & o;
  printf("%d\n", answer);
  answer = (m + n) & o;
  printf("%d\n", answer);
  answer = m + (n & o);
  printf("%d\n\n", answer);
  
  printf("+ and ^ permutations\n");
  printf("=========================\n");
  answer = m + n ^ o;
  printf("%d\n", answer);
  answer = (m + n) ^ o;
  printf("%d\n", answer);
  answer = m + (n ^ o);
  printf("%d\n\n", answer);
  
  printf("+ and | permutations\n");
  printf("=========================\n");
  answer = m + n | o;
  printf("%d\n", answer);
  answer = (m + n) | o;
  printf("%d\n", answer);
  answer = m + (n | o);
  printf("%d\n\n", answer);
  
  printf("+ and && permutations\n");
  printf("=========================\n");
  answer = m + n && o;
  printf("%d\n", answer);
  answer = (m + n) && o;
  printf("%d\n", answer);
  answer = m + (n && o);
  printf("%d\n\n", answer);
  
  printf("+ and || permutations\n");
  printf("=========================\n");
  answer = m + n || o;
  printf("%d\n", answer);
  answer = (m + n) || o;
  printf("%d\n", answer);
  answer = m + (n || o);
  printf("%d\n\n", answer);
  
  printf("- and * permutations\n");
  printf("=========================\n");
  answer = m - n * o;
  printf("%d\n", answer);
  answer = (m - n) * o;
  printf("%d\n", answer);
  answer = m - (n * o);
  printf("%d\n\n", answer);
  
  printf("- and / permutations\n");
  printf("=========================\n");
  answer = m - n / o;
  printf("%d\n", answer);
  answer = (m - n) / o;
  printf("%d\n", answer);
  answer = m - (n / o);
  printf("%d\n\n", answer);
  
  printf("- and %% permutations\n");
  printf("=========================\n");
  answer = m - n % o;
  printf("%d\n", answer);
  answer = (m - n) % o;
  printf("%d\n", answer);
  answer = m - (n % o);
  printf("%d\n\n", answer);
  
  printf("- and + permutations\n");
  printf("=========================\n");
  answer = m - n + o;
  printf("%d\n", answer);
  answer = (m - n) + o;
  printf("%d\n", answer);
  answer = m - (n + o);
  printf("%d\n\n", answer);
  
  printf("- and - permutations\n");
  printf("=========================\n");
  answer = m - n - o;
  printf("%d\n", answer);
  answer = (m - n) - o;
  printf("%d\n", answer);
  answer = m - (n - o);
  printf("%d\n\n", answer);
  
  printf("- and < permutations\n");
  printf("=========================\n");
  answer = m - n < o;
  printf("%d\n", answer);
  answer = (m - n) < o;
  printf("%d\n", answer);
  answer = m - (n < o);
  printf("%d\n\n", answer);
  
  printf("- and <= permutations\n");
  printf("=========================\n");
  answer = m - n <= o;
  printf("%d\n", answer);
  answer = (m - n) <= o;
  printf("%d\n", answer);
  answer = m - (n <= o);
  printf("%d\n\n", answer);
  
  printf("- and > permutations\n");
  printf("=========================\n");
  answer = m - n > o;
  printf("%d\n", answer);
  answer = (m - n) > o;
  printf("%d\n", answer);
  answer = m - (n > o);
  printf("%d\n\n", answer);
  
  printf("- and >= permutations\n");
  printf("=========================\n");
  answer = m - n >= o;
  printf("%d\n", answer);
  answer = (m - n) >= o;
  printf("%d\n", answer);
  answer = m - (n >= o);
  printf("%d\n\n", answer);
  
  printf("- and == permutations\n");
  printf("=========================\n");
  answer = m - n == o;
  printf("%d\n", answer);
  answer = (m - n) == o;
  printf("%d\n", answer);
  answer = m - (n == o);
  printf("%d\n\n", answer);
  
  printf("- and != permutations\n");
  printf("=========================\n");
  answer = m - n != o;
  printf("%d\n", answer);
  answer = (m - n) != o;
  printf("%d\n", answer);
  answer = m - (n != o);
  printf("%d\n\n", answer);
  
  printf("- and & permutations\n");
  printf("=========================\n");
  answer = m - n & o;
  printf("%d\n", answer);
  answer = (m - n) & o;
  printf("%d\n", answer);
  answer = m - (n & o);
  printf("%d\n\n", answer);
  
  printf("- and ^ permutations\n");
  printf("=========================\n");
  answer = m - n ^ o;
  printf("%d\n", answer);
  answer = (m - n) ^ o;
  printf("%d\n", answer);
  answer = m - (n ^ o);
  printf("%d\n\n", answer);
  
  printf("- and | permutations\n");
  printf("=========================\n");
  answer = m - n | o;
  printf("%d\n", answer);
  answer = (m - n) | o;
  printf("%d\n", answer);
  answer = m - (n | o);
  printf("%d\n\n", answer);
  
  printf("- and && permutations\n");
  printf("=========================\n");
  answer = m - n && o;
  printf("%d\n", answer);
  answer = (m - n) && o;
  printf("%d\n", answer);
  answer = m - (n && o);
  printf("%d\n\n", answer);
  
  printf("- and || permutations\n");
  printf("=========================\n");
  answer = m - n || o;
  printf("%d\n", answer);
  answer = (m - n) || o;
  printf("%d\n", answer);
  answer = m - (n || o);
  printf("%d\n\n", answer);
  
  printf("< and * permutations\n");
  printf("=========================\n");
  answer = m < n * o;
  printf("%d\n", answer);
  answer = (m < n) * o;
  printf("%d\n", answer);
  answer = m < (n * o);
  printf("%d\n\n", answer);
  
  printf("< and / permutations\n");
  printf("=========================\n");
  answer = m < n / o;
  printf("%d\n", answer);
  answer = (m < n) / o;
  printf("%d\n", answer);
  answer = m < (n / o);
  printf("%d\n\n", answer);
  
  printf("< and %% permutations\n");
  printf("=========================\n");
  answer = m < n % o;
  printf("%d\n", answer);
  answer = (m < n) % o;
  printf("%d\n", answer);
  answer = m < (n % o);
  printf("%d\n\n", answer);
  
  printf("< and + permutations\n");
  printf("=========================\n");
  answer = m < n + o;
  printf("%d\n", answer);
  answer = (m < n) + o;
  printf("%d\n", answer);
  answer = m < (n + o);
  printf("%d\n\n", answer);
  
  printf("< and - permutations\n");
  printf("=========================\n");
  answer = m < n - o;
  printf("%d\n", answer);
  answer = (m < n) - o;
  printf("%d\n", answer);
  answer = m < (n - o);
  printf("%d\n\n", answer);
  
  printf("< and < permutations\n");
  printf("=========================\n");
  answer = m < n < o;
  printf("%d\n", answer);
  answer = (m < n) < o;
  printf("%d\n", answer);
  answer = m < (n < o);
  printf("%d\n\n", answer);
  
  printf("< and <= permutations\n");
  printf("=========================\n");
  answer = m < n <= o;
  printf("%d\n", answer);
  answer = (m < n) <= o;
  printf("%d\n", answer);
  answer = m < (n <= o);
  printf("%d\n\n", answer);
  
  printf("< and > permutations\n");
  printf("=========================\n");
  answer = m < n > o;
  printf("%d\n", answer);
  answer = (m < n) > o;
  printf("%d\n", answer);
  answer = m < (n > o);
  printf("%d\n\n", answer);
  
  printf("< and >= permutations\n");
  printf("=========================\n");
  answer = m < n >= o;
  printf("%d\n", answer);
  answer = (m < n) >= o;
  printf("%d\n", answer);
  answer = m < (n >= o);
  printf("%d\n\n", answer);
  
  printf("< and == permutations\n");
  printf("=========================\n");
  answer = m < n == o;
  printf("%d\n", answer);
  answer = (m < n) == o;
  printf("%d\n", answer);
  answer = m < (n == o);
  printf("%d\n\n", answer);
  
  printf("< and != permutations\n");
  printf("=========================\n");
  answer = m < n != o;
  printf("%d\n", answer);
  answer = (m < n) != o;
  printf("%d\n", answer);
  answer = m < (n != o);
  printf("%d\n\n", answer);
  
  printf("< and & permutations\n");
  printf("=========================\n");
  answer = m < n & o;
  printf("%d\n", answer);
  answer = (m < n) & o;
  printf("%d\n", answer);
  answer = m < (n & o);
  printf("%d\n\n", answer);
  
  printf("< and ^ permutations\n");
  printf("=========================\n");
  answer = m < n ^ o;
  printf("%d\n", answer);
  answer = (m < n) ^ o;
  printf("%d\n", answer);
  answer = m < (n ^ o);
  printf("%d\n\n", answer);
  
  printf("< and | permutations\n");
  printf("=========================\n");
  answer = m < n | o;
  printf("%d\n", answer);
  answer = (m < n) | o;
  printf("%d\n", answer);
  answer = m < (n | o);
  printf("%d\n\n", answer);
  
  printf("< and && permutations\n");
  printf("=========================\n");
  answer = m < n && o;
  printf("%d\n", answer);
  answer = (m < n) && o;
  printf("%d\n", answer);
  answer = m < (n && o);
  printf("%d\n\n", answer);
  
  printf("< and || permutations\n");
  printf("=========================\n");
  answer = m < n || o;
  printf("%d\n", answer);
  answer = (m < n) || o;
  printf("%d\n", answer);
  answer = m < (n || o);
  printf("%d\n\n", answer);
  
  printf("<= and * permutations\n");
  printf("=========================\n");
  answer = m <= n * o;
  printf("%d\n", answer);
  answer = (m <= n) * o;
  printf("%d\n", answer);
  answer = m <= (n * o);
  printf("%d\n\n", answer);
  
  printf("<= and / permutations\n");
  printf("=========================\n");
  answer = m <= n / o;
  printf("%d\n", answer);
  answer = (m <= n) / o;
  printf("%d\n", answer);
  answer = m <= (n / o);
  printf("%d\n\n", answer);
  
  printf("<= and %% permutations\n");
  printf("=========================\n");
  answer = m <= n % o;
  printf("%d\n", answer);
  answer = (m <= n) % o;
  printf("%d\n", answer);
  answer = m <= (n % o);
  printf("%d\n\n", answer);
  
  printf("<= and + permutations\n");
  printf("=========================\n");
  answer = m <= n + o;
  printf("%d\n", answer);
  answer = (m <= n) + o;
  printf("%d\n", answer);
  answer = m <= (n + o);
  printf("%d\n\n", answer);
  
  printf("<= and - permutations\n");
  printf("=========================\n");
  answer = m <= n - o;
  printf("%d\n", answer);
  answer = (m <= n) - o;
  printf("%d\n", answer);
  answer = m <= (n - o);
  printf("%d\n\n", answer);
  
  printf("<= and < permutations\n");
  printf("=========================\n");
  answer = m <= n < o;
  printf("%d\n", answer);
  answer = (m <= n) < o;
  printf("%d\n", answer);
  answer = m <= (n < o);
  printf("%d\n\n", answer);
  
  printf("<= and <= permutations\n");
  printf("=========================\n");
  answer = m <= n <= o;
  printf("%d\n", answer);
  answer = (m <= n) <= o;
  printf("%d\n", answer);
  answer = m <= (n <= o);
  printf("%d\n\n", answer);
  
  printf("<= and > permutations\n");
  printf("=========================\n");
  answer = m <= n > o;
  printf("%d\n", answer);
  answer = (m <= n) > o;
  printf("%d\n", answer);
  answer = m <= (n > o);
  printf("%d\n\n", answer);
  
  printf("<= and >= permutations\n");
  printf("=========================\n");
  answer = m <= n >= o;
  printf("%d\n", answer);
  answer = (m <= n) >= o;
  printf("%d\n", answer);
  answer = m <= (n >= o);
  printf("%d\n\n", answer);
  
  printf("<= and == permutations\n");
  printf("=========================\n");
  answer = m <= n == o;
  printf("%d\n", answer);
  answer = (m <= n) == o;
  printf("%d\n", answer);
  answer = m <= (n == o);
  printf("%d\n\n", answer);
  
  printf("<= and != permutations\n");
  printf("=========================\n");
  answer = m <= n != o;
  printf("%d\n", answer);
  answer = (m <= n) != o;
  printf("%d\n", answer);
  answer = m <= (n != o);
  printf("%d\n\n", answer);
  
  printf("<= and & permutations\n");
  printf("=========================\n");
  answer = m <= n & o;
  printf("%d\n", answer);
  answer = (m <= n) & o;
  printf("%d\n", answer);
  answer = m <= (n & o);
  printf("%d\n\n", answer);
  
  printf("<= and ^ permutations\n");
  printf("=========================\n");
  answer = m <= n ^ o;
  printf("%d\n", answer);
  answer = (m <= n) ^ o;
  printf("%d\n", answer);
  answer = m <= (n ^ o);
  printf("%d\n\n", answer);
  
  printf("<= and | permutations\n");
  printf("=========================\n");
  answer = m <= n | o;
  printf("%d\n", answer);
  answer = (m <= n) | o;
  printf("%d\n", answer);
  answer = m <= (n | o);
  printf("%d\n\n", answer);
  
  printf("<= and && permutations\n");
  printf("=========================\n");
  answer = m <= n && o;
  printf("%d\n", answer);
  answer = (m <= n) && o;
  printf("%d\n", answer);
  answer = m <= (n && o);
  printf("%d\n\n", answer);
  
  printf("<= and || permutations\n");
  printf("=========================\n");
  answer = m <= n || o;
  printf("%d\n", answer);
  answer = (m <= n) || o;
  printf("%d\n", answer);
  answer = m <= (n || o);
  printf("%d\n\n", answer);
  
  printf("> and * permutations\n");
  printf("=========================\n");
  answer = m > n * o;
  printf("%d\n", answer);
  answer = (m > n) * o;
  printf("%d\n", answer);
  answer = m > (n * o);
  printf("%d\n\n", answer);
  
  printf("> and / permutations\n");
  printf("=========================\n");
  answer = m > n / o;
  printf("%d\n", answer);
  answer = (m > n) / o;
  printf("%d\n", answer);
  answer = m > (n / o);
  printf("%d\n\n", answer);
  
  printf("> and %% permutations\n");
  printf("=========================\n");
  answer = m > n % o;
  printf("%d\n", answer);
  answer = (m > n) % o;
  printf("%d\n", answer);
  answer = m > (n % o);
  printf("%d\n\n", answer);
  
  printf("> and + permutations\n");
  printf("=========================\n");
  answer = m > n + o;
  printf("%d\n", answer);
  answer = (m > n) + o;
  printf("%d\n", answer);
  answer = m > (n + o);
  printf("%d\n\n", answer);
  
  printf("> and - permutations\n");
  printf("=========================\n");
  answer = m > n - o;
  printf("%d\n", answer);
  answer = (m > n) - o;
  printf("%d\n", answer);
  answer = m > (n - o);
  printf("%d\n\n", answer);
  
  printf("> and < permutations\n");
  printf("=========================\n");
  answer = m > n < o;
  printf("%d\n", answer);
  answer = (m > n) < o;
  printf("%d\n", answer);
  answer = m > (n < o);
  printf("%d\n\n", answer);
  
  printf("> and <= permutations\n");
  printf("=========================\n");
  answer = m > n <= o;
  printf("%d\n", answer);
  answer = (m > n) <= o;
  printf("%d\n", answer);
  answer = m > (n <= o);
  printf("%d\n\n", answer);
  
  printf("> and > permutations\n");
  printf("=========================\n");
  answer = m > n > o;
  printf("%d\n", answer);
  answer = (m > n) > o;
  printf("%d\n", answer);
  answer = m > (n > o);
  printf("%d\n\n", answer);
  
  printf("> and >= permutations\n");
  printf("=========================\n");
  answer = m > n >= o;
  printf("%d\n", answer);
  answer = (m > n) >= o;
  printf("%d\n", answer);
  answer = m > (n >= o);
  printf("%d\n\n", answer);
  
  printf("> and == permutations\n");
  printf("=========================\n");
  answer = m > n == o;
  printf("%d\n", answer);
  answer = (m > n) == o;
  printf("%d\n", answer);
  answer = m > (n == o);
  printf("%d\n\n", answer);
  
  printf("> and != permutations\n");
  printf("=========================\n");
  answer = m > n != o;
  printf("%d\n", answer);
  answer = (m > n) != o;
  printf("%d\n", answer);
  answer = m > (n != o);
  printf("%d\n\n", answer);
  
  printf("> and & permutations\n");
  printf("=========================\n");
  answer = m > n & o;
  printf("%d\n", answer);
  answer = (m > n) & o;
  printf("%d\n", answer);
  answer = m > (n & o);
  printf("%d\n\n", answer);
  
  printf("> and ^ permutations\n");
  printf("=========================\n");
  answer = m > n ^ o;
  printf("%d\n", answer);
  answer = (m > n) ^ o;
  printf("%d\n", answer);
  answer = m > (n ^ o);
  printf("%d\n\n", answer);
  
  printf("> and | permutations\n");
  printf("=========================\n");
  answer = m > n | o;
  printf("%d\n", answer);
  answer = (m > n) | o;
  printf("%d\n", answer);
  answer = m > (n | o);
  printf("%d\n\n", answer);
  
  printf("> and && permutations\n");
  printf("=========================\n");
  answer = m > n && o;
  printf("%d\n", answer);
  answer = (m > n) && o;
  printf("%d\n", answer);
  answer = m > (n && o);
  printf("%d\n\n", answer);
  
  printf("> and || permutations\n");
  printf("=========================\n");
  answer = m > n || o;
  printf("%d\n", answer);
  answer = (m > n) || o;
  printf("%d\n", answer);
  answer = m > (n || o);
  printf("%d\n\n", answer);
  
  printf(">= and * permutations\n");
  printf("=========================\n");
  answer = m >= n * o;
  printf("%d\n", answer);
  answer = (m >= n) * o;
  printf("%d\n", answer);
  answer = m >= (n * o);
  printf("%d\n\n", answer);
  
  printf(">= and / permutations\n");
  printf("=========================\n");
  answer = m >= n / o;
  printf("%d\n", answer);
  answer = (m >= n) / o;
  printf("%d\n", answer);
  answer = m >= (n / o);
  printf("%d\n\n", answer);
  
  printf(">= and %% permutations\n");
  printf("=========================\n");
  answer = m >= n % o;
  printf("%d\n", answer);
  answer = (m >= n) % o;
  printf("%d\n", answer);
  answer = m >= (n % o);
  printf("%d\n\n", answer);
  
  printf(">= and + permutations\n");
  printf("=========================\n");
  answer = m >= n + o;
  printf("%d\n", answer);
  answer = (m >= n) + o;
  printf("%d\n", answer);
  answer = m >= (n + o);
  printf("%d\n\n", answer);
  
  printf(">= and - permutations\n");
  printf("=========================\n");
  answer = m >= n - o;
  printf("%d\n", answer);
  answer = (m >= n) - o;
  printf("%d\n", answer);
  answer = m >= (n - o);
  printf("%d\n\n", answer);
  
  printf(">= and < permutations\n");
  printf("=========================\n");
  answer = m >= n < o;
  printf("%d\n", answer);
  answer = (m >= n) < o;
  printf("%d\n", answer);
  answer = m >= (n < o);
  printf("%d\n\n", answer);
  
  printf(">= and <= permutations\n");
  printf("=========================\n");
  answer = m >= n <= o;
  printf("%d\n", answer);
  answer = (m >= n) <= o;
  printf("%d\n", answer);
  answer = m >= (n <= o);
  printf("%d\n\n", answer);
  
  printf(">= and > permutations\n");
  printf("=========================\n");
  answer = m >= n > o;
  printf("%d\n", answer);
  answer = (m >= n) > o;
  printf("%d\n", answer);
  answer = m >= (n > o);
  printf("%d\n\n", answer);
  
  printf(">= and >= permutations\n");
  printf("=========================\n");
  answer = m >= n >= o;
  printf("%d\n", answer);
  answer = (m >= n) >= o;
  printf("%d\n", answer);
  answer = m >= (n >= o);
  printf("%d\n\n", answer);
  
  printf(">= and == permutations\n");
  printf("=========================\n");
  answer = m >= n == o;
  printf("%d\n", answer);
  answer = (m >= n) == o;
  printf("%d\n", answer);
  answer = m >= (n == o);
  printf("%d\n\n", answer);
  
  printf(">= and != permutations\n");
  printf("=========================\n");
  answer = m >= n != o;
  printf("%d\n", answer);
  answer = (m >= n) != o;
  printf("%d\n", answer);
  answer = m >= (n != o);
  printf("%d\n\n", answer);
  
  printf(">= and & permutations\n");
  printf("=========================\n");
  answer = m >= n & o;
  printf("%d\n", answer);
  answer = (m >= n) & o;
  printf("%d\n", answer);
  answer = m >= (n & o);
  printf("%d\n\n", answer);
  
  printf(">= and ^ permutations\n");
  printf("=========================\n");
  answer = m >= n ^ o;
  printf("%d\n", answer);
  answer = (m >= n) ^ o;
  printf("%d\n", answer);
  answer = m >= (n ^ o);
  printf("%d\n\n", answer);
  
  printf(">= and | permutations\n");
  printf("=========================\n");
  answer = m >= n | o;
  printf("%d\n", answer);
  answer = (m >= n) | o;
  printf("%d\n", answer);
  answer = m >= (n | o);
  printf("%d\n\n", answer);
  
  printf(">= and && permutations\n");
  printf("=========================\n");
  answer = m >= n && o;
  printf("%d\n", answer);
  answer = (m >= n) && o;
  printf("%d\n", answer);
  answer = m >= (n && o);
  printf("%d\n\n", answer);
  
  printf(">= and || permutations\n");
  printf("=========================\n");
  answer = m >= n || o;
  printf("%d\n", answer);
  answer = (m >= n) || o;
  printf("%d\n", answer);
  answer = m >= (n || o);
  printf("%d\n\n", answer);
  
  printf("== and * permutations\n");
  printf("=========================\n");
  answer = m == n * o;
  printf("%d\n", answer);
  answer = (m == n) * o;
  printf("%d\n", answer);
  answer = m == (n * o);
  printf("%d\n\n", answer);
  
  printf("== and / permutations\n");
  printf("=========================\n");
  answer = m == n / o;
  printf("%d\n", answer);
  answer = (m == n) / o;
  printf("%d\n", answer);
  answer = m == (n / o);
  printf("%d\n\n", answer);
  
  printf("== and %% permutations\n");
  printf("=========================\n");
  answer = m == n % o;
  printf("%d\n", answer);
  answer = (m == n) % o;
  printf("%d\n", answer);
  answer = m == (n % o);
  printf("%d\n\n", answer);
  
  printf("== and + permutations\n");
  printf("=========================\n");
  answer = m == n + o;
  printf("%d\n", answer);
  answer = (m == n) + o;
  printf("%d\n", answer);
  answer = m == (n + o);
  printf("%d\n\n", answer);
  
  printf("== and - permutations\n");
  printf("=========================\n");
  answer = m == n - o;
  printf("%d\n", answer);
  answer = (m == n) - o;
  printf("%d\n", answer);
  answer = m == (n - o);
  printf("%d\n\n", answer);
  
  printf("== and < permutations\n");
  printf("=========================\n");
  answer = m == n < o;
  printf("%d\n", answer);
  answer = (m == n) < o;
  printf("%d\n", answer);
  answer = m == (n < o);
  printf("%d\n\n", answer);
  
  printf("== and <= permutations\n");
  printf("=========================\n");
  answer = m == n <= o;
  printf("%d\n", answer);
  answer = (m == n) <= o;
  printf("%d\n", answer);
  answer = m == (n <= o);
  printf("%d\n\n", answer);
  
  printf("== and > permutations\n");
  printf("=========================\n");
  answer = m == n > o;
  printf("%d\n", answer);
  answer = (m == n) > o;
  printf("%d\n", answer);
  answer = m == (n > o);
  printf("%d\n\n", answer);
  
  printf("== and >= permutations\n");
  printf("=========================\n");
  answer = m == n >= o;
  printf("%d\n", answer);
  answer = (m == n) >= o;
  printf("%d\n", answer);
  answer = m == (n >= o);
  printf("%d\n\n", answer);
  
  printf("== and == permutations\n");
  printf("=========================\n");
  answer = m == n == o;
  printf("%d\n", answer);
  answer = (m == n) == o;
  printf("%d\n", answer);
  answer = m == (n == o);
  printf("%d\n\n", answer);
  
  printf("== and != permutations\n");
  printf("=========================\n");
  answer = m == n != o;
  printf("%d\n", answer);
  answer = (m == n) != o;
  printf("%d\n", answer);
  answer = m == (n != o);
  printf("%d\n\n", answer);
  
  printf("== and & permutations\n");
  printf("=========================\n");
  answer = m == n & o;
  printf("%d\n", answer);
  answer = (m == n) & o;
  printf("%d\n", answer);
  answer = m == (n & o);
  printf("%d\n\n", answer);
  
  printf("== and ^ permutations\n");
  printf("=========================\n");
  answer = m == n ^ o;
  printf("%d\n", answer);
  answer = (m == n) ^ o;
  printf("%d\n", answer);
  answer = m == (n ^ o);
  printf("%d\n\n", answer);
  
  printf("== and | permutations\n");
  printf("=========================\n");
  answer = m == n | o;
  printf("%d\n", answer);
  answer = (m == n) | o;
  printf("%d\n", answer);
  answer = m == (n | o);
  printf("%d\n\n", answer);
  
  printf("== and && permutations\n");
  printf("=========================\n");
  answer = m == n && o;
  printf("%d\n", answer);
  answer = (m == n) && o;
  printf("%d\n", answer);
  answer = m == (n && o);
  printf("%d\n\n", answer);
  
  printf("== and || permutations\n");
  printf("=========================\n");
  answer = m == n || o;
  printf("%d\n", answer);
  answer = (m == n) || o;
  printf("%d\n", answer);
  answer = m == (n || o);
  printf("%d\n\n", answer);
  
  printf("!= and * permutations\n");
  printf("=========================\n");
  answer = m != n * o;
  printf("%d\n", answer);
  answer = (m != n) * o;
  printf("%d\n", answer);
  answer = m != (n * o);
  printf("%d\n\n", answer);
  
  printf("!= and / permutations\n");
  printf("=========================\n");
  answer = m != n / o;
  printf("%d\n", answer);
  answer = (m != n) / o;
  printf("%d\n", answer);
  answer = m != (n / o);
  printf("%d\n\n", answer);
  
  printf("!= and %% permutations\n");
  printf("=========================\n");
  answer = m != n % o;
  printf("%d\n", answer);
  answer = (m != n) % o;
  printf("%d\n", answer);
  answer = m != (n % o);
  printf("%d\n\n", answer);
  
  printf("!= and + permutations\n");
  printf("=========================\n");
  answer = m != n + o;
  printf("%d\n", answer);
  answer = (m != n) + o;
  printf("%d\n", answer);
  answer = m != (n + o);
  printf("%d\n\n", answer);
  
  printf("!= and - permutations\n");
  printf("=========================\n");
  answer = m != n - o;
  printf("%d\n", answer);
  answer = (m != n) - o;
  printf("%d\n", answer);
  answer = m != (n - o);
  printf("%d\n\n", answer);
  
  printf("!= and < permutations\n");
  printf("=========================\n");
  answer = m != n < o;
  printf("%d\n", answer);
  answer = (m != n) < o;
  printf("%d\n", answer);
  answer = m != (n < o);
  printf("%d\n\n", answer);
  
  printf("!= and <= permutations\n");
  printf("=========================\n");
  answer = m != n <= o;
  printf("%d\n", answer);
  answer = (m != n) <= o;
  printf("%d\n", answer);
  answer = m != (n <= o);
  printf("%d\n\n", answer);
  
  printf("!= and > permutations\n");
  printf("=========================\n");
  answer = m != n > o;
  printf("%d\n", answer);
  answer = (m != n) > o;
  printf("%d\n", answer);
  answer = m != (n > o);
  printf("%d\n\n", answer);
  
  printf("!= and >= permutations\n");
  printf("=========================\n");
  answer = m != n >= o;
  printf("%d\n", answer);
  answer = (m != n) >= o;
  printf("%d\n", answer);
  answer = m != (n >= o);
  printf("%d\n\n", answer);
  
  printf("!= and == permutations\n");
  printf("=========================\n");
  answer = m != n == o;
  printf("%d\n", answer);
  answer = (m != n) == o;
  printf("%d\n", answer);
  answer = m != (n == o);
  printf("%d\n\n", answer);
  
  printf("!= and != permutations\n");
  printf("=========================\n");
  answer = m != n != o;
  printf("%d\n", answer);
  answer = (m != n) != o;
  printf("%d\n", answer);
  answer = m != (n != o);
  printf("%d\n\n", answer);
  
  printf("!= and & permutations\n");
  printf("=========================\n");
  answer = m != n & o;
  printf("%d\n", answer);
  answer = (m != n) & o;
  printf("%d\n", answer);
  answer = m != (n & o);
  printf("%d\n\n", answer);
  
  printf("!= and ^ permutations\n");
  printf("=========================\n");
  answer = m != n ^ o;
  printf("%d\n", answer);
  answer = (m != n) ^ o;
  printf("%d\n", answer);
  answer = m != (n ^ o);
  printf("%d\n\n", answer);
  
  printf("!= and | permutations\n");
  printf("=========================\n");
  answer = m != n | o;
  printf("%d\n", answer);
  answer = (m != n) | o;
  printf("%d\n", answer);
  answer = m != (n | o);
  printf("%d\n\n", answer);
  
  printf("!= and && permutations\n");
  printf("=========================\n");
  answer = m != n && o;
  printf("%d\n", answer);
  answer = (m != n) && o;
  printf("%d\n", answer);
  answer = m != (n && o);
  printf("%d\n\n", answer);
  
  printf("!= and || permutations\n");
  printf("=========================\n");
  answer = m != n || o;
  printf("%d\n", answer);
  answer = (m != n) || o;
  printf("%d\n", answer);
  answer = m != (n || o);
  printf("%d\n\n", answer);
  
  printf("& and * permutations\n");
  printf("=========================\n");
  answer = m & n * o;
  printf("%d\n", answer);
  answer = (m & n) * o;
  printf("%d\n", answer);
  answer = m & (n * o);
  printf("%d\n\n", answer);
  
  printf("& and / permutations\n");
  printf("=========================\n");
  answer = m & n / o;
  printf("%d\n", answer);
  answer = (m & n) / o;
  printf("%d\n", answer);
  answer = m & (n / o);
  printf("%d\n\n", answer);
  
  printf("& and %% permutations\n");
  printf("=========================\n");
  answer = m & n % o;
  printf("%d\n", answer);
  answer = (m & n) % o;
  printf("%d\n", answer);
  answer = m & (n % o);
  printf("%d\n\n", answer);
  
  printf("& and + permutations\n");
  printf("=========================\n");
  answer = m & n + o;
  printf("%d\n", answer);
  answer = (m & n) + o;
  printf("%d\n", answer);
  answer = m & (n + o);
  printf("%d\n\n", answer);
  
  printf("& and - permutations\n");
  printf("=========================\n");
  answer = m & n - o;
  printf("%d\n", answer);
  answer = (m & n) - o;
  printf("%d\n", answer);
  answer = m & (n - o);
  printf("%d\n\n", answer);
  
  printf("& and < permutations\n");
  printf("=========================\n");
  answer = m & n < o;
  printf("%d\n", answer);
  answer = (m & n) < o;
  printf("%d\n", answer);
  answer = m & (n < o);
  printf("%d\n\n", answer);
  
  printf("& and <= permutations\n");
  printf("=========================\n");
  answer = m & n <= o;
  printf("%d\n", answer);
  answer = (m & n) <= o;
  printf("%d\n", answer);
  answer = m & (n <= o);
  printf("%d\n\n", answer);
  
  printf("& and > permutations\n");
  printf("=========================\n");
  answer = m & n > o;
  printf("%d\n", answer);
  answer = (m & n) > o;
  printf("%d\n", answer);
  answer = m & (n > o);
  printf("%d\n\n", answer);
  
  printf("& and >= permutations\n");
  printf("=========================\n");
  answer = m & n >= o;
  printf("%d\n", answer);
  answer = (m & n) >= o;
  printf("%d\n", answer);
  answer = m & (n >= o);
  printf("%d\n\n", answer);
  
  printf("& and == permutations\n");
  printf("=========================\n");
  answer = m & n == o;
  printf("%d\n", answer);
  answer = (m & n) == o;
  printf("%d\n", answer);
  answer = m & (n == o);
  printf("%d\n\n", answer);
  
  printf("& and != permutations\n");
  printf("=========================\n");
  answer = m & n != o;
  printf("%d\n", answer);
  answer = (m & n) != o;
  printf("%d\n", answer);
  answer = m & (n != o);
  printf("%d\n\n", answer);
  
  printf("& and & permutations\n");
  printf("=========================\n");
  answer = m & n & o;
  printf("%d\n", answer);
  answer = (m & n) & o;
  printf("%d\n", answer);
  answer = m & (n & o);
  printf("%d\n\n", answer);
  
  printf("& and ^ permutations\n");
  printf("=========================\n");
  answer = m & n ^ o;
  printf("%d\n", answer);
  answer = (m & n) ^ o;
  printf("%d\n", answer);
  answer = m & (n ^ o);
  printf("%d\n\n", answer);
  
  printf("& and | permutations\n");
  printf("=========================\n");
  answer = m & n | o;
  printf("%d\n", answer);
  answer = (m & n) | o;
  printf("%d\n", answer);
  answer = m & (n | o);
  printf("%d\n\n", answer);
  
  printf("& and && permutations\n");
  printf("=========================\n");
  answer = m & n && o;
  printf("%d\n", answer);
  answer = (m & n) && o;
  printf("%d\n", answer);
  answer = m & (n && o);
  printf("%d\n\n", answer);
  
  printf("& and || permutations\n");
  printf("=========================\n");
  answer = m & n || o;
  printf("%d\n", answer);
  answer = (m & n) || o;
  printf("%d\n", answer);
  answer = m & (n || o);
  printf("%d\n\n", answer);
  
  printf("^ and * permutations\n");
  printf("=========================\n");
  answer = m ^ n * o;
  printf("%d\n", answer);
  answer = (m ^ n) * o;
  printf("%d\n", answer);
  answer = m ^ (n * o);
  printf("%d\n\n", answer);
  
  printf("^ and / permutations\n");
  printf("=========================\n");
  answer = m ^ n / o;
  printf("%d\n", answer);
  answer = (m ^ n) / o;
  printf("%d\n", answer);
  answer = m ^ (n / o);
  printf("%d\n\n", answer);
  
  printf("^ and %% permutations\n");
  printf("=========================\n");
  answer = m ^ n % o;
  printf("%d\n", answer);
  answer = (m ^ n) % o;
  printf("%d\n", answer);
  answer = m ^ (n % o);
  printf("%d\n\n", answer);
  
  printf("^ and + permutations\n");
  printf("=========================\n");
  answer = m ^ n + o;
  printf("%d\n", answer);
  answer = (m ^ n) + o;
  printf("%d\n", answer);
  answer = m ^ (n + o);
  printf("%d\n\n", answer);
  
  printf("^ and - permutations\n");
  printf("=========================\n");
  answer = m ^ n - o;
  printf("%d\n", answer);
  answer = (m ^ n) - o;
  printf("%d\n", answer);
  answer = m ^ (n - o);
  printf("%d\n\n", answer);
  
  printf("^ and < permutations\n");
  printf("=========================\n");
  answer = m ^ n < o;
  printf("%d\n", answer);
  answer = (m ^ n) < o;
  printf("%d\n", answer);
  answer = m ^ (n < o);
  printf("%d\n\n", answer);
  
  printf("^ and <= permutations\n");
  printf("=========================\n");
  answer = m ^ n <= o;
  printf("%d\n", answer);
  answer = (m ^ n) <= o;
  printf("%d\n", answer);
  answer = m ^ (n <= o);
  printf("%d\n\n", answer);
  
  printf("^ and > permutations\n");
  printf("=========================\n");
  answer = m ^ n > o;
  printf("%d\n", answer);
  answer = (m ^ n) > o;
  printf("%d\n", answer);
  answer = m ^ (n > o);
  printf("%d\n\n", answer);
  
  printf("^ and >= permutations\n");
  printf("=========================\n");
  answer = m ^ n >= o;
  printf("%d\n", answer);
  answer = (m ^ n) >= o;
  printf("%d\n", answer);
  answer = m ^ (n >= o);
  printf("%d\n\n", answer);
  
  printf("^ and == permutations\n");
  printf("=========================\n");
  answer = m ^ n == o;
  printf("%d\n", answer);
  answer = (m ^ n) == o;
  printf("%d\n", answer);
  answer = m ^ (n == o);
  printf("%d\n\n", answer);
  
  printf("^ and != permutations\n");
  printf("=========================\n");
  answer = m ^ n != o;
  printf("%d\n", answer);
  answer = (m ^ n) != o;
  printf("%d\n", answer);
  answer = m ^ (n != o);
  printf("%d\n\n", answer);
  
  printf("^ and & permutations\n");
  printf("=========================\n");
  answer = m ^ n & o;
  printf("%d\n", answer);
  answer = (m ^ n) & o;
  printf("%d\n", answer);
  answer = m ^ (n & o);
  printf("%d\n\n", answer);
  
  printf("^ and ^ permutations\n");
  printf("=========================\n");
  answer = m ^ n ^ o;
  printf("%d\n", answer);
  answer = (m ^ n) ^ o;
  printf("%d\n", answer);
  answer = m ^ (n ^ o);
  printf("%d\n\n", answer);
  
  printf("^ and | permutations\n");
  printf("=========================\n");
  answer = m ^ n | o;
  printf("%d\n", answer);
  answer = (m ^ n) | o;
  printf("%d\n", answer);
  answer = m ^ (n | o);
  printf("%d\n\n", answer);
  
  printf("^ and && permutations\n");
  printf("=========================\n");
  answer = m ^ n && o;
  printf("%d\n", answer);
  answer = (m ^ n) && o;
  printf("%d\n", answer);
  answer = m ^ (n && o);
  printf("%d\n\n", answer);
  
  printf("^ and || permutations\n");
  printf("=========================\n");
  answer = m ^ n || o;
  printf("%d\n", answer);
  answer = (m ^ n) || o;
  printf("%d\n", answer);
  answer = m ^ (n || o);
  printf("%d\n\n", answer);
  
  printf("| and * permutations\n");
  printf("=========================\n");
  answer = m | n * o;
  printf("%d\n", answer);
  answer = (m | n) * o;
  printf("%d\n", answer);
  answer = m | (n * o);
  printf("%d\n\n", answer);
  
  printf("| and / permutations\n");
  printf("=========================\n");
  answer = m | n / o;
  printf("%d\n", answer);
  answer = (m | n) / o;
  printf("%d\n", answer);
  answer = m | (n / o);
  printf("%d\n\n", answer);
  
  printf("| and %% permutations\n");
  printf("=========================\n");
  answer = m | n % o;
  printf("%d\n", answer);
  answer = (m | n) % o;
  printf("%d\n", answer);
  answer = m | (n % o);
  printf("%d\n\n", answer);
  
  printf("| and + permutations\n");
  printf("=========================\n");
  answer = m | n + o;
  printf("%d\n", answer);
  answer = (m | n) + o;
  printf("%d\n", answer);
  answer = m | (n + o);
  printf("%d\n\n", answer);
  
  printf("| and - permutations\n");
  printf("=========================\n");
  answer = m | n - o;
  printf("%d\n", answer);
  answer = (m | n) - o;
  printf("%d\n", answer);
  answer = m | (n - o);
  printf("%d\n\n", answer);
  
  printf("| and < permutations\n");
  printf("=========================\n");
  answer = m | n < o;
  printf("%d\n", answer);
  answer = (m | n) < o;
  printf("%d\n", answer);
  answer = m | (n < o);
  printf("%d\n\n", answer);
  
  printf("| and <= permutations\n");
  printf("=========================\n");
  answer = m | n <= o;
  printf("%d\n", answer);
  answer = (m | n) <= o;
  printf("%d\n", answer);
  answer = m | (n <= o);
  printf("%d\n\n", answer);
  
  printf("| and > permutations\n");
  printf("=========================\n");
  answer = m | n > o;
  printf("%d\n", answer);
  answer = (m | n) > o;
  printf("%d\n", answer);
  answer = m | (n > o);
  printf("%d\n\n", answer);
  
  printf("| and >= permutations\n");
  printf("=========================\n");
  answer = m | n >= o;
  printf("%d\n", answer);
  answer = (m | n) >= o;
  printf("%d\n", answer);
  answer = m | (n >= o);
  printf("%d\n\n", answer);
  
  printf("| and == permutations\n");
  printf("=========================\n");
  answer = m | n == o;
  printf("%d\n", answer);
  answer = (m | n) == o;
  printf("%d\n", answer);
  answer = m | (n == o);
  printf("%d\n\n", answer);
  
  printf("| and != permutations\n");
  printf("=========================\n");
  answer = m | n != o;
  printf("%d\n", answer);
  answer = (m | n) != o;
  printf("%d\n", answer);
  answer = m | (n != o);
  printf("%d\n\n", answer);
  
  printf("| and & permutations\n");
  printf("=========================\n");
  answer = m | n & o;
  printf("%d\n", answer);
  answer = (m | n) & o;
  printf("%d\n", answer);
  answer = m | (n & o);
  printf("%d\n\n", answer);
  
  printf("| and ^ permutations\n");
  printf("=========================\n");
  answer = m | n ^ o;
  printf("%d\n", answer);
  answer = (m | n) ^ o;
  printf("%d\n", answer);
  answer = m | (n ^ o);
  printf("%d\n\n", answer);
  
  printf("| and | permutations\n");
  printf("=========================\n");
  answer = m | n | o;
  printf("%d\n", answer);
  answer = (m | n) | o;
  printf("%d\n", answer);
  answer = m | (n | o);
  printf("%d\n\n", answer);
  
  printf("| and && permutations\n");
  printf("=========================\n");
  answer = m | n && o;
  printf("%d\n", answer);
  answer = (m | n) && o;
  printf("%d\n", answer);
  answer = m | (n && o);
  printf("%d\n\n", answer);
  
  printf("| and || permutations\n");
  printf("=========================\n");
  answer = m | n || o;
  printf("%d\n", answer);
  answer = (m | n) || o;
  printf("%d\n", answer);
  answer = m | (n || o);
  printf("%d\n\n", answer);
  
  printf("&& and * permutations\n");
  printf("=========================\n");
  answer = m && n * o;
  printf("%d\n", answer);
  answer = (m && n) * o;
  printf("%d\n", answer);
  answer = m && (n * o);
  printf("%d\n\n", answer);
  
  printf("&& and / permutations\n");
  printf("=========================\n");
  answer = m && n / o;
  printf("%d\n", answer);
  answer = (m && n) / o;
  printf("%d\n", answer);
  answer = m && (n / o);
  printf("%d\n\n", answer);
  
  printf("&& and %% permutations\n");
  printf("=========================\n");
  answer = m && n % o;
  printf("%d\n", answer);
  answer = (m && n) % o;
  printf("%d\n", answer);
  answer = m && (n % o);
  printf("%d\n\n", answer);
  
  printf("&& and + permutations\n");
  printf("=========================\n");
  answer = m && n + o;
  printf("%d\n", answer);
  answer = (m && n) + o;
  printf("%d\n", answer);
  answer = m && (n + o);
  printf("%d\n\n", answer);
  
  printf("&& and - permutations\n");
  printf("=========================\n");
  answer = m && n - o;
  printf("%d\n", answer);
  answer = (m && n) - o;
  printf("%d\n", answer);
  answer = m && (n - o);
  printf("%d\n\n", answer);
  
  printf("&& and < permutations\n");
  printf("=========================\n");
  answer = m && n < o;
  printf("%d\n", answer);
  answer = (m && n) < o;
  printf("%d\n", answer);
  answer = m && (n < o);
  printf("%d\n\n", answer);
  
  printf("&& and <= permutations\n");
  printf("=========================\n");
  answer = m && n <= o;
  printf("%d\n", answer);
  answer = (m && n) <= o;
  printf("%d\n", answer);
  answer = m && (n <= o);
  printf("%d\n\n", answer);
  
  printf("&& and > permutations\n");
  printf("=========================\n");
  answer = m && n > o;
  printf("%d\n", answer);
  answer = (m && n) > o;
  printf("%d\n", answer);
  answer = m && (n > o);
  printf("%d\n\n", answer);
  
  printf("&& and >= permutations\n");
  printf("=========================\n");
  answer = m && n >= o;
  printf("%d\n", answer);
  answer = (m && n) >= o;
  printf("%d\n", answer);
  answer = m && (n >= o);
  printf("%d\n\n", answer);
  
  printf("&& and == permutations\n");
  printf("=========================\n");
  answer = m && n == o;
  printf("%d\n", answer);
  answer = (m && n) == o;
  printf("%d\n", answer);
  answer = m && (n == o);
  printf("%d\n\n", answer);
  
  printf("&& and != permutations\n");
  printf("=========================\n");
  answer = m && n != o;
  printf("%d\n", answer);
  answer = (m && n) != o;
  printf("%d\n", answer);
  answer = m && (n != o);
  printf("%d\n\n", answer);
  
  printf("&& and & permutations\n");
  printf("=========================\n");
  answer = m && n & o;
  printf("%d\n", answer);
  answer = (m && n) & o;
  printf("%d\n", answer);
  answer = m && (n & o);
  printf("%d\n\n", answer);
  
  printf("&& and ^ permutations\n");
  printf("=========================\n");
  answer = m && n ^ o;
  printf("%d\n", answer);
  answer = (m && n) ^ o;
  printf("%d\n", answer);
  answer = m && (n ^ o);
  printf("%d\n\n", answer);
  
  printf("&& and | permutations\n");
  printf("=========================\n");
  answer = m && n | o;
  printf("%d\n", answer);
  answer = (m && n) | o;
  printf("%d\n", answer);
  answer = m && (n | o);
  printf("%d\n\n", answer);
  
  printf("&& and && permutations\n");
  printf("=========================\n");
  answer = m && n && o;
  printf("%d\n", answer);
  answer = (m && n) && o;
  printf("%d\n", answer);
  answer = m && (n && o);
  printf("%d\n\n", answer);
  
  printf("&& and || permutations\n");
  printf("=========================\n");
  answer = m && n || o;
  printf("%d\n", answer);
  answer = (m && n) || o;
  printf("%d\n", answer);
  answer = m && (n || o);
  printf("%d\n\n", answer);
  
  printf("|| and * permutations\n");
  printf("=========================\n");
  answer = m || n * o;
  printf("%d\n", answer);
  answer = (m || n) * o;
  printf("%d\n", answer);
  answer = m || (n * o);
  printf("%d\n\n", answer);
  
  printf("|| and / permutations\n");
  printf("=========================\n");
  answer = m || n / o;
  printf("%d\n", answer);
  answer = (m || n) / o;
  printf("%d\n", answer);
  answer = m || (n / o);
  printf("%d\n\n", answer);
  
  printf("|| and %% permutations\n");
  printf("=========================\n");
  answer = m || n % o;
  printf("%d\n", answer);
  answer = (m || n) % o;
  printf("%d\n", answer);
  answer = m || (n % o);
  printf("%d\n\n", answer);
  
  printf("|| and + permutations\n");
  printf("=========================\n");
  answer = m || n + o;
  printf("%d\n", answer);
  answer = (m || n) + o;
  printf("%d\n", answer);
  answer = m || (n + o);
  printf("%d\n\n", answer);
  
  printf("|| and - permutations\n");
  printf("=========================\n");
  answer = m || n - o;
  printf("%d\n", answer);
  answer = (m || n) - o;
  printf("%d\n", answer);
  answer = m || (n - o);
  printf("%d\n\n", answer);
  
  printf("|| and < permutations\n");
  printf("=========================\n");
  answer = m || n < o;
  printf("%d\n", answer);
  answer = (m || n) < o;
  printf("%d\n", answer);
  answer = m || (n < o);
  printf("%d\n\n", answer);
  
  printf("|| and <= permutations\n");
  printf("=========================\n");
  answer = m || n <= o;
  printf("%d\n", answer);
  answer = (m || n) <= o;
  printf("%d\n", answer);
  answer = m || (n <= o);
  printf("%d\n\n", answer);
  
  printf("|| and > permutations\n");
  printf("=========================\n");
  answer = m || n > o;
  printf("%d\n", answer);
  answer = (m || n) > o;
  printf("%d\n", answer);
  answer = m || (n > o);
  printf("%d\n\n", answer);
  
  printf("|| and >= permutations\n");
  printf("=========================\n");
  answer = m || n >= o;
  printf("%d\n", answer);
  answer = (m || n) >= o;
  printf("%d\n", answer);
  answer = m || (n >= o);
  printf("%d\n\n", answer);
  
  printf("|| and == permutations\n");
  printf("=========================\n");
  answer = m || n == o;
  printf("%d\n", answer);
  answer = (m || n) == o;
  printf("%d\n", answer);
  answer = m || (n == o);
  printf("%d\n\n", answer);
  
  printf("|| and != permutations\n");
  printf("=========================\n");
  answer = m || n != o;
  printf("%d\n", answer);
  answer = (m || n) != o;
  printf("%d\n", answer);
  answer = m || (n != o);
  printf("%d\n\n", answer);
  
  printf("|| and & permutations\n");
  printf("=========================\n");
  answer = m || n & o;
  printf("%d\n", answer);
  answer = (m || n) & o;
  printf("%d\n", answer);
  answer = m || (n & o);
  printf("%d\n\n", answer);
  
  printf("|| and ^ permutations\n");
  printf("=========================\n");
  answer = m || n ^ o;
  printf("%d\n", answer);
  answer = (m || n) ^ o;
  printf("%d\n", answer);
  answer = m || (n ^ o);
  printf("%d\n\n", answer);
  
  printf("|| and | permutations\n");
  printf("=========================\n");
  answer = m || n | o;
  printf("%d\n", answer);
  answer = (m || n) | o;
  printf("%d\n", answer);
  answer = m || (n | o);
  printf("%d\n\n", answer);
  
  printf("|| and && permutations\n");
  printf("=========================\n");
  answer = m || n && o;
  printf("%d\n", answer);
  answer = (m || n) && o;
  printf("%d\n", answer);
  answer = m || (n && o);
  printf("%d\n\n", answer);
  
  printf("|| and || permutations\n");
  printf("=========================\n");
  answer = m || n || o;
  printf("%d\n", answer);
  answer = (m || n) || o;
  printf("%d\n", answer);
  answer = m || (n || o);
  printf("%d\n\n", answer);
  
}
