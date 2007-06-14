var m = 2;
var n = 3;
var o = 5;
var answer = 0;

write("* and * permutations\n");
writeln("=========================");
answer = m * n * o;
writeln(answer);
answer = (m * n) * o;
writeln(answer);
answer = m * (n * o);
writeln(answer, "\n");

write("* and / permutations\n");
writeln("=========================");
answer = m * n / o;
writeln(answer);
answer = (m * n) / o;
writeln(answer);
answer = m * (n / o);
writeln(answer, "\n");

write("* and % permutations\n");
writeln("=========================");
answer = m * n % o;
writeln(answer);
answer = (m * n) % o;
writeln(answer);
answer = m * (n % o);
writeln(answer, "\n");

write("* and + permutations\n");
writeln("=========================");
answer = m * n + o;
writeln(answer);
answer = (m * n) + o;
writeln(answer);
answer = m * (n + o);
writeln(answer, "\n");

write("* and - permutations\n");
writeln("=========================");
answer = m * n - o;
writeln(answer);
answer = (m * n) - o;
writeln(answer);
answer = m * (n - o);
writeln(answer, "\n");

write("* and < permutations\n");
writeln("=========================");
answer = m * n < o;
writeln(answer);
answer = (m * n) < o;
writeln(answer);
answer = m * (n < o);
writeln(answer, "\n");

write("* and <= permutations\n");
writeln("=========================");
answer = m * n <= o;
writeln(answer);
answer = (m * n) <= o;
writeln(answer);
answer = m * (n <= o);
writeln(answer, "\n");

write("* and > permutations\n");
writeln("=========================");
answer = m * n > o;
writeln(answer);
answer = (m * n) > o;
writeln(answer);
answer = m * (n > o);
writeln(answer, "\n");

write("* and >= permutations\n");
writeln("=========================");
answer = m * n >= o;
writeln(answer);
answer = (m * n) >= o;
writeln(answer);
answer = m * (n >= o);
writeln(answer, "\n");

write("* and == permutations\n");
writeln("=========================");
answer = m * n == o;
writeln(answer);
answer = (m * n) == o;
writeln(answer);
answer = m * (n == o);
writeln(answer, "\n");

write("* and != permutations\n");
writeln("=========================");
answer = m * n != o;
writeln(answer);
answer = (m * n) != o;
writeln(answer);
answer = m * (n != o);
writeln(answer, "\n");

write("* and & permutations\n");
writeln("=========================");
answer = m * n & o;
writeln(answer);
answer = (m * n) & o;
writeln(answer);
answer = m * (n & o);
writeln(answer, "\n");

write("* and ^ permutations\n");
writeln("=========================");
answer = m * n ^ o;
writeln(answer);
answer = (m * n) ^ o;
writeln(answer);
answer = m * (n ^ o);
writeln(answer, "\n");

write("* and | permutations\n");
writeln("=========================");
answer = m * n | o;
writeln(answer);
answer = (m * n) | o;
writeln(answer);
answer = m * (n | o);
writeln(answer, "\n");

write("* and && permutations\n");
writeln("=========================");
answer = m * n != 0 && 0 != o;
writeln(answer);
answer = (m * n) != 0 && 0 != o;
writeln(answer);
answer = m * (n != 0 && 0 != o);
writeln(answer, "\n");

write("* and || permutations\n");
writeln("=========================");
answer = m * n != 0 || 0 != o;
writeln(answer);
answer = (m * n) != 0 || 0 != o;
writeln(answer);
answer = m * (n != 0 || 0 != o);
writeln(answer, "\n");

write("/ and * permutations\n");
writeln("=========================");
answer = m / n * o;
writeln(answer);
answer = (m / n) * o;
writeln(answer);
answer = m / (n * o);
writeln(answer, "\n");

write("/ and / permutations\n");
writeln("=========================");
answer = m / n / o;
writeln(answer);
answer = (m / n) / o;
writeln(answer);

n = 10;
answer = m / (n / o);
writeln(answer, "\n");
n = 3;

write("/ and % permutations\n");
writeln("=========================");
answer = m / n % o;
writeln(answer);
answer = (m / n) % o;
writeln(answer);
answer = m / (n % o);
writeln(answer, "\n");

write("/ and + permutations\n");
writeln("=========================");
answer = m / n + o;
writeln(answer);
answer = (m / n) + o;
writeln(answer);
answer = m / (n + o);
writeln(answer, "\n");

write("/ and - permutations\n");
writeln("=========================");
answer = m / n - o;
writeln(answer);
answer = (m / n) - o;
writeln(answer);
answer = m / (n - o);
writeln(answer, "\n");

write("/ and < permutations\n");
writeln("=========================");
answer = m / n < o;
writeln(answer);
answer = (m / n) < o;
writeln(answer);
answer = m / (n < o);
writeln(answer, "\n");

write("/ and <= permutations\n");
writeln("=========================");
answer = m / n <= o;
writeln(answer);
answer = (m / n) <= o;
writeln(answer);
answer = m / (n <= o);
writeln(answer, "\n");

write("/ and > permutations\n");
writeln("=========================");
answer = m / n > o;
writeln(answer);
answer = (m / n) > o;
writeln(answer);

n = 10;
answer = m / (n > o);
writeln(answer, "\n");
n = 3;

write("/ and >= permutations\n");
writeln("=========================");
answer = m / n >= o;
writeln(answer);
answer = (m / n) >= o;
writeln(answer);

n = 10;
answer = m / (n >= o);
writeln(answer, "\n");
n = 3;

write("/ and == permutations\n");
writeln("=========================");
answer = m / n == o;
writeln(answer);
answer = (m / n) == o;
writeln(answer);

n = 5;
answer = m / (n == o);
writeln(answer, "\n");
n = 3;

write("/ and != permutations\n");
writeln("=========================");
answer = m / n != o;
writeln(answer);
answer = (m / n) != o;
writeln(answer);
answer = m / (n != o);
writeln(answer, "\n");

write("/ and & permutations\n");
writeln("=========================");
answer = m / n & o;
writeln(answer);
answer = (m / n) & o;
writeln(answer);
answer = m / (n & o);
writeln(answer, "\n");

write("/ and ^ permutations\n");
writeln("=========================");
answer = m / n ^ o;
writeln(answer);
answer = (m / n) ^ o;
writeln(answer);
answer = m / (n ^ o);
writeln(answer, "\n");

write("/ and | permutations\n");
writeln("=========================");
answer = m / n | o;
writeln(answer);
answer = (m / n) | o;
writeln(answer);
answer = m / (n | o);
writeln(answer, "\n");

write("/ and && permutations\n");
writeln("=========================");
answer = m / n != 0 && 0 != o;
writeln(answer);
answer = (m / n) != 0 && 0 != o;
writeln(answer);
answer = m / (n != 0 && 0 != o);
writeln(answer, "\n");

write("/ and || permutations\n");
writeln("=========================");
answer = m / n != 0 || 0 != o;
writeln(answer);
answer = (m / n) != 0 || 0 != o;
writeln(answer);
answer = m / (n != 0 || 0 != o);
writeln(answer, "\n");

write("% and * permutations\n");
writeln("=========================");
answer = m % n * o;
writeln(answer);
answer = (m % n) * o;
writeln(answer);
answer = m % (n * o);
writeln(answer, "\n");

write("% and / permutations\n");
writeln("=========================");
answer = m % n / o;
writeln(answer);
answer = (m % n) / o;
writeln(answer);

n = 10;
answer = m % (n / o);
writeln(answer, "\n");
n = 3;

write("% and % permutations\n");
writeln("=========================");
answer = m % n % o;
writeln(answer);
answer = (m % n) % o;
writeln(answer);
answer = m % (n % o);
writeln(answer, "\n");

write("% and + permutations\n");
writeln("=========================");
answer = m % n + o;
writeln(answer);
answer = (m % n) + o;
writeln(answer);
answer = m % (n + o);
writeln(answer, "\n");

write("% and - permutations\n");
writeln("=========================");
answer = m % n - o;
writeln(answer);
answer = (m % n) - o;
writeln(answer);
answer = m % (n - o);
writeln(answer, "\n");

write("% and < permutations\n");
writeln("=========================");
answer = m % n < o;
writeln(answer);
answer = (m % n) < o;
writeln(answer);
answer = m % (n < o);
writeln(answer, "\n");

write("% and <= permutations\n");
writeln("=========================");
answer = m % n <= o;
writeln(answer);
answer = (m % n) <= o;
writeln(answer);
answer = m % (n <= o);
writeln(answer, "\n");

write("% and > permutations\n");
writeln("=========================");
answer = m % n > o;
writeln(answer);
answer = (m % n) > o;
writeln(answer);

n = 10;
answer = m % (n > o);
writeln(answer, "\n");
n = 3;

write("% and >= permutations\n");
writeln("=========================");
answer = m % n >= o;
writeln(answer);
answer = (m % n) >= o;
writeln(answer);

n = 10;
answer = m % (n >= o);
writeln(answer, "\n");
n = 3;

write("% and == permutations\n");
writeln("=========================");
answer = m % n == o;
writeln(answer);
answer = (m % n) == o;
writeln(answer);

n = 5;
answer = m % (n == o);
writeln(answer, "\n");
n = 3;

write("% and != permutations\n");
writeln("=========================");
answer = m % n != o;
writeln(answer);
answer = (m % n) != o;
writeln(answer);
answer = m % (n != o);
writeln(answer, "\n");

write("% and & permutations\n");
writeln("=========================");
answer = m % n & o;
writeln(answer);
answer = (m % n) & o;
writeln(answer);
answer = m % (n & o);
writeln(answer, "\n");

write("% and ^ permutations\n");
writeln("=========================");
answer = m % n ^ o;
writeln(answer);
answer = (m % n) ^ o;
writeln(answer);
answer = m % (n ^ o);
writeln(answer, "\n");

write("% and | permutations\n");
writeln("=========================");
answer = m % n | o;
writeln(answer);
answer = (m % n) | o;
writeln(answer);
answer = m % (n | o);
writeln(answer, "\n");

write("% and && permutations\n");
writeln("=========================");
answer = m % n != 0 && 0 != o;
writeln(answer);
answer = (m % n) != 0 && 0 != o;
writeln(answer);
answer = m % (n != 0 && 0 != o);
writeln(answer, "\n");

write("% and || permutations\n");
writeln("=========================");
answer = m % n != 0 || 0 != o;
writeln(answer);
answer = (m % n) != 0 || 0 != o;
writeln(answer);
answer = m % (n != 0 || 0 != o);
writeln(answer, "\n");

write("+ and * permutations\n");
writeln("=========================");
answer = m + n * o;
writeln(answer);
answer = (m + n) * o;
writeln(answer);
answer = m + (n * o);
writeln(answer, "\n");

write("+ and / permutations\n");
writeln("=========================");
answer = m + n / o;
writeln(answer);
answer = (m + n) / o;
writeln(answer);
answer = m + (n / o);
writeln(answer, "\n");

write("+ and % permutations\n");
writeln("=========================");
answer = m + n % o;
writeln(answer);
answer = (m + n) % o;
writeln(answer);
answer = m + (n % o);
writeln(answer, "\n");

write("+ and + permutations\n");
writeln("=========================");
answer = m + n + o;
writeln(answer);
answer = (m + n) + o;
writeln(answer);
answer = m + (n + o);
writeln(answer, "\n");

write("+ and - permutations\n");
writeln("=========================");
answer = m + n - o;
writeln(answer);
answer = (m + n) - o;
writeln(answer);
answer = m + (n - o);
writeln(answer, "\n");

write("+ and < permutations\n");
writeln("=========================");
answer = m + n < o;
writeln(answer);
answer = (m + n) < o;
writeln(answer);
answer = m + (n < o);
writeln(answer, "\n");

write("+ and <= permutations\n");
writeln("=========================");
answer = m + n <= o;
writeln(answer);
answer = (m + n) <= o;
writeln(answer);
answer = m + (n <= o);
writeln(answer, "\n");

write("+ and > permutations\n");
writeln("=========================");
answer = m + n > o;
writeln(answer);
answer = (m + n) > o;
writeln(answer);
answer = m + (n > o);
writeln(answer, "\n");

write("+ and >= permutations\n");
writeln("=========================");
answer = m + n >= o;
writeln(answer);
answer = (m + n) >= o;
writeln(answer);
answer = m + (n >= o);
writeln(answer, "\n");

write("+ and == permutations\n");
writeln("=========================");
answer = m + n == o;
writeln(answer);
answer = (m + n) == o;
writeln(answer);
answer = m + (n == o);
writeln(answer, "\n");

write("+ and != permutations\n");
writeln("=========================");
answer = m + n != o;
writeln(answer);
answer = (m + n) != o;
writeln(answer);
answer = m + (n != o);
writeln(answer, "\n");

write("+ and & permutations\n");
writeln("=========================");
answer = m + n & o;
writeln(answer);
answer = (m + n) & o;
writeln(answer);
answer = m + (n & o);
writeln(answer, "\n");

write("+ and ^ permutations\n");
writeln("=========================");
answer = m + n ^ o;
writeln(answer);
answer = (m + n) ^ o;
writeln(answer);
answer = m + (n ^ o);
writeln(answer, "\n");

write("+ and | permutations\n");
writeln("=========================");
answer = m + n | o;
writeln(answer);
answer = (m + n) | o;
writeln(answer);
answer = m + (n | o);
writeln(answer, "\n");

write("+ and && permutations\n");
writeln("=========================");
answer = m + n != 0 && 0 != o;
writeln(answer);
answer = (m + n) != 0 && 0 != o;
writeln(answer);
answer = m + (n != 0 && 0 != o);
writeln(answer, "\n");

write("+ and || permutations\n");
writeln("=========================");
answer = m + n != 0 || 0 != o;
writeln(answer);
answer = (m + n) != 0 || 0 != o;
writeln(answer);
answer = m + (n != 0 || 0 != o);
writeln(answer, "\n");

write("- and * permutations\n");
writeln("=========================");
answer = m - n * o;
writeln(answer);
answer = (m - n) * o;
writeln(answer);
answer = m - (n * o);
writeln(answer, "\n");

write("- and / permutations\n");
writeln("=========================");
answer = m - n / o;
writeln(answer);
answer = (m - n) / o;
writeln(answer);
answer = m - (n / o);
writeln(answer, "\n");

write("- and % permutations\n");
writeln("=========================");
answer = m - n % o;
writeln(answer);
answer = (m - n) % o;
writeln(answer);
answer = m - (n % o);
writeln(answer, "\n");

write("- and + permutations\n");
writeln("=========================");
answer = m - n + o;
writeln(answer);
answer = (m - n) + o;
writeln(answer);
answer = m - (n + o);
writeln(answer, "\n");

write("- and - permutations\n");
writeln("=========================");
answer = m - n - o;
writeln(answer);
answer = (m - n) - o;
writeln(answer);
answer = m - (n - o);
writeln(answer, "\n");

write("- and < permutations\n");
writeln("=========================");
answer = m - n < o;
writeln(answer);
answer = (m - n) < o;
writeln(answer);
answer = m - (n < o);
writeln(answer, "\n");

write("- and <= permutations\n");
writeln("=========================");
answer = m - n <= o;
writeln(answer);
answer = (m - n) <= o;
writeln(answer);
answer = m - (n <= o);
writeln(answer, "\n");

write("- and > permutations\n");
writeln("=========================");
answer = m - n > o;
writeln(answer);
answer = (m - n) > o;
writeln(answer);
answer = m - (n > o);
writeln(answer, "\n");

write("- and >= permutations\n");
writeln("=========================");
answer = m - n >= o;
writeln(answer);
answer = (m - n) >= o;
writeln(answer);
answer = m - (n >= o);
writeln(answer, "\n");

write("- and == permutations\n");
writeln("=========================");
answer = m - n == o;
writeln(answer);
answer = (m - n) == o;
writeln(answer);
answer = m - (n == o);
writeln(answer, "\n");

write("- and != permutations\n");
writeln("=========================");
answer = m - n != o;
writeln(answer);
answer = (m - n) != o;
writeln(answer);
answer = m - (n != o);
writeln(answer, "\n");

write("- and & permutations\n");
writeln("=========================");
answer = m - n & o;
writeln(answer);
answer = (m - n) & o;
writeln(answer);
answer = m - (n & o);
writeln(answer, "\n");

write("- and ^ permutations\n");
writeln("=========================");
answer = m - n ^ o;
writeln(answer);
answer = (m - n) ^ o;
writeln(answer);
answer = m - (n ^ o);
writeln(answer, "\n");

write("- and | permutations\n");
writeln("=========================");
answer = m - n | o;
writeln(answer);
answer = (m - n) | o;
writeln(answer);
answer = m - (n | o);
writeln(answer, "\n");

write("- and && permutations\n");
writeln("=========================");
answer = m - n != 0 && 0 != o;
writeln(answer);
answer = (m - n) != 0 && 0 != o;
writeln(answer);
answer = m - (n != 0 && 0 != o);
writeln(answer, "\n");

write("- and || permutations\n");
writeln("=========================");
answer = m - n != 0 || 0 != o;
writeln(answer);
answer = (m - n) != 0 || 0 != o;
writeln(answer);
answer = m - (n != 0 || 0 != o);
writeln(answer, "\n");

write("< and * permutations\n");
writeln("=========================");
answer = m < n * o;
writeln(answer);
answer = (m < n) * o;
writeln(answer);
answer = m < (n * o);
writeln(answer, "\n");

write("< and / permutations\n");
writeln("=========================");
answer = m < n / o;
writeln(answer);
answer = (m < n) / o;
writeln(answer);
answer = m < (n / o);
writeln(answer, "\n");

write("< and % permutations\n");
writeln("=========================");
answer = m < n % o;
writeln(answer);
answer = (m < n) % o;
writeln(answer);
answer = m < (n % o);
writeln(answer, "\n");

write("< and + permutations\n");
writeln("=========================");
answer = m < n + o;
writeln(answer);
answer = (m < n) + o;
writeln(answer);
answer = m < (n + o);
writeln(answer, "\n");

write("< and - permutations\n");
writeln("=========================");
answer = m < n - o;
writeln(answer);
answer = (m < n) - o;
writeln(answer);
answer = m < (n - o);
writeln(answer, "\n");

write("< and < permutations\n");
writeln("=========================");
answer = m < n < o;
writeln(answer);
answer = (m < n) < o;
writeln(answer);
answer = m < (n < o);
writeln(answer, "\n");

write("< and <= permutations\n");
writeln("=========================");
answer = m < n <= o;
writeln(answer);
answer = (m < n) <= o;
writeln(answer);
answer = m < (n <= o);
writeln(answer, "\n");

write("< and > permutations\n");
writeln("=========================");
answer = m < n > o;
writeln(answer);
answer = (m < n) > o;
writeln(answer);
answer = m < (n > o);
writeln(answer, "\n");

write("< and >= permutations\n");
writeln("=========================");
answer = m < n >= o;
writeln(answer);
answer = (m < n) >= o;
writeln(answer);
answer = m < (n >= o);
writeln(answer, "\n");

write("< and == permutations\n");
writeln("=========================");
answer = m < n == o;
writeln(answer);
answer = (m < n) == o;
writeln(answer);
answer = m < (n == o);
writeln(answer, "\n");

write("< and != permutations\n");
writeln("=========================");
answer = m < n != o;
writeln(answer);
answer = (m < n) != o;
writeln(answer);
answer = m < (n != o);
writeln(answer, "\n");

write("< and & permutations\n");
writeln("=========================");
answer = m < n & o;
writeln(answer);
answer = (m < n) & o;
writeln(answer);
answer = m < (n & o);
writeln(answer, "\n");

write("< and ^ permutations\n");
writeln("=========================");
answer = m < n ^ o;
writeln(answer);
answer = (m < n) ^ o;
writeln(answer);
answer = m < (n ^ o);
writeln(answer, "\n");

write("< and | permutations\n");
writeln("=========================");
answer = m < n | o;
writeln(answer);
answer = (m < n) | o;
writeln(answer);
answer = m < (n | o);
writeln(answer, "\n");

write("< and && permutations\n");
writeln("=========================");
answer = m < n != 0 && 0 != o;
writeln(answer);
answer = (m < n) != 0 && 0 != o;
writeln(answer);
answer = m < (n != 0 && 0 != o);
writeln(answer, "\n");

write("< and || permutations\n");
writeln("=========================");
answer = m < n != 0 || 0 != o;
writeln(answer);
answer = (m < n) != 0 || 0 != o;
writeln(answer);
answer = m < (n != 0 || 0 != o);
writeln(answer, "\n");

write("<= and * permutations\n");
writeln("=========================");
answer = m <= n * o;
writeln(answer);
answer = (m <= n) * o;
writeln(answer);
answer = m <= (n * o);
writeln(answer, "\n");

write("<= and / permutations\n");
writeln("=========================");
answer = m <= n / o;
writeln(answer);
answer = (m <= n) / o;
writeln(answer);
answer = m <= (n / o);
writeln(answer, "\n");

write("<= and % permutations\n");
writeln("=========================");
answer = m <= n % o;
writeln(answer);
answer = (m <= n) % o;
writeln(answer);
answer = m <= (n % o);
writeln(answer, "\n");

write("<= and + permutations\n");
writeln("=========================");
answer = m <= n + o;
writeln(answer);
answer = (m <= n) + o;
writeln(answer);
answer = m <= (n + o);
writeln(answer, "\n");

write("<= and - permutations\n");
writeln("=========================");
answer = m <= n - o;
writeln(answer);
answer = (m <= n) - o;
writeln(answer);
answer = m <= (n - o);
writeln(answer, "\n");

write("<= and < permutations\n");
writeln("=========================");
answer = m <= n < o;
writeln(answer);
answer = (m <= n) < o;
writeln(answer);
answer = m <= (n < o);
writeln(answer, "\n");

write("<= and <= permutations\n");
writeln("=========================");
answer = m <= n <= o;
writeln(answer);
answer = (m <= n) <= o;
writeln(answer);
answer = m <= (n <= o);
writeln(answer, "\n");

write("<= and > permutations\n");
writeln("=========================");
answer = m <= n > o;
writeln(answer);
answer = (m <= n) > o;
writeln(answer);
answer = m <= (n > o);
writeln(answer, "\n");

write("<= and >= permutations\n");
writeln("=========================");
answer = m <= n >= o;
writeln(answer);
answer = (m <= n) >= o;
writeln(answer);
answer = m <= (n >= o);
writeln(answer, "\n");

write("<= and == permutations\n");
writeln("=========================");
answer = m <= n == o;
writeln(answer);
answer = (m <= n) == o;
writeln(answer);
answer = m <= (n == o);
writeln(answer, "\n");

write("<= and != permutations\n");
writeln("=========================");
answer = m <= n != o;
writeln(answer);
answer = (m <= n) != o;
writeln(answer);
answer = m <= (n != o);
writeln(answer, "\n");

write("<= and & permutations\n");
writeln("=========================");
answer = m <= n & o;
writeln(answer);
answer = (m <= n) & o;
writeln(answer);
answer = m <= (n & o);
writeln(answer, "\n");

write("<= and ^ permutations\n");
writeln("=========================");
answer = m <= n ^ o;
writeln(answer);
answer = (m <= n) ^ o;
writeln(answer);
answer = m <= (n ^ o);
writeln(answer, "\n");

write("<= and | permutations\n");
writeln("=========================");
answer = m <= n | o;
writeln(answer);
answer = (m <= n) | o;
writeln(answer);
answer = m <= (n | o);
writeln(answer, "\n");

write("<= and && permutations\n");
writeln("=========================");
answer = m <= n != 0 && 0 != o;
writeln(answer);
answer = (m <= n) != 0 && 0 != o;
writeln(answer);
answer = m <= (n != 0 && 0 != o);
writeln(answer, "\n");

write("<= and || permutations\n");
writeln("=========================");
answer = m <= n != 0 || 0 != o;
writeln(answer);
answer = (m <= n) != 0 || 0 != o;
writeln(answer);
answer = m <= (n != 0 || 0 != o);
writeln(answer, "\n");

write("> and * permutations\n");
writeln("=========================");
answer = m > n * o;
writeln(answer);
answer = (m > n) * o;
writeln(answer);
answer = m > (n * o);
writeln(answer, "\n");

write("> and / permutations\n");
writeln("=========================");
answer = m > n / o;
writeln(answer);
answer = (m > n) / o;
writeln(answer);
answer = m > (n / o);
writeln(answer, "\n");

write("> and % permutations\n");
writeln("=========================");
answer = m > n % o;
writeln(answer);
answer = (m > n) % o;
writeln(answer);
answer = m > (n % o);
writeln(answer, "\n");

write("> and + permutations\n");
writeln("=========================");
answer = m > n + o;
writeln(answer);
answer = (m > n) + o;
writeln(answer);
answer = m > (n + o);
writeln(answer, "\n");

write("> and - permutations\n");
writeln("=========================");
answer = m > n - o;
writeln(answer);
answer = (m > n) - o;
writeln(answer);
answer = m > (n - o);
writeln(answer, "\n");

write("> and < permutations\n");
writeln("=========================");
answer = m > n < o;
writeln(answer);
answer = (m > n) < o;
writeln(answer);
answer = m > (n < o);
writeln(answer, "\n");

write("> and <= permutations\n");
writeln("=========================");
answer = m > n <= o;
writeln(answer);
answer = (m > n) <= o;
writeln(answer);
answer = m > (n <= o);
writeln(answer, "\n");

write("> and > permutations\n");
writeln("=========================");
answer = m > n > o;
writeln(answer);
answer = (m > n) > o;
writeln(answer);
answer = m > (n > o);
writeln(answer, "\n");

write("> and >= permutations\n");
writeln("=========================");
answer = m > n >= o;
writeln(answer);
answer = (m > n) >= o;
writeln(answer);
answer = m > (n >= o);
writeln(answer, "\n");

write("> and == permutations\n");
writeln("=========================");
answer = m > n == o;
writeln(answer);
answer = (m > n) == o;
writeln(answer);
answer = m > (n == o);
writeln(answer, "\n");

write("> and != permutations\n");
writeln("=========================");
answer = m > n != o;
writeln(answer);
answer = (m > n) != o;
writeln(answer);
answer = m > (n != o);
writeln(answer, "\n");

write("> and & permutations\n");
writeln("=========================");
answer = m > n & o;
writeln(answer);
answer = (m > n) & o;
writeln(answer);
answer = m > (n & o);
writeln(answer, "\n");

write("> and ^ permutations\n");
writeln("=========================");
answer = m > n ^ o;
writeln(answer);
answer = (m > n) ^ o;
writeln(answer);
answer = m > (n ^ o);
writeln(answer, "\n");

write("> and | permutations\n");
writeln("=========================");
answer = m > n | o;
writeln(answer);
answer = (m > n) | o;
writeln(answer);
answer = m > (n | o);
writeln(answer, "\n");

write("> and && permutations\n");
writeln("=========================");
answer = m > n != 0 && 0 != o;
writeln(answer);
answer = (m > n) != 0 && 0 != o;
writeln(answer);
answer = m > (n != 0 && 0 != o);
writeln(answer, "\n");

write("> and || permutations\n");
writeln("=========================");
answer = m > n != 0 || 0 != o;
writeln(answer);
answer = (m > n) != 0 || 0 != o;
writeln(answer);
answer = m > (n != 0 || 0 != o);
writeln(answer, "\n");

write(">= and * permutations\n");
writeln("=========================");
answer = m >= n * o;
writeln(answer);
answer = (m >= n) * o;
writeln(answer);
answer = m >= (n * o);
writeln(answer, "\n");

write(">= and / permutations\n");
writeln("=========================");
answer = m >= n / o;
writeln(answer);
answer = (m >= n) / o;
writeln(answer);
answer = m >= (n / o);
writeln(answer, "\n");

write(">= and % permutations\n");
writeln("=========================");
answer = m >= n % o;
writeln(answer);
answer = (m >= n) % o;
writeln(answer);
answer = m >= (n % o);
writeln(answer, "\n");

write(">= and + permutations\n");
writeln("=========================");
answer = m >= n + o;
writeln(answer);
answer = (m >= n) + o;
writeln(answer);
answer = m >= (n + o);
writeln(answer, "\n");

write(">= and - permutations\n");
writeln("=========================");
answer = m >= n - o;
writeln(answer);
answer = (m >= n) - o;
writeln(answer);
answer = m >= (n - o);
writeln(answer, "\n");

write(">= and < permutations\n");
writeln("=========================");
answer = m >= n < o;
writeln(answer);
answer = (m >= n) < o;
writeln(answer);
answer = m >= (n < o);
writeln(answer, "\n");

write(">= and <= permutations\n");
writeln("=========================");
answer = m >= n <= o;
writeln(answer);
answer = (m >= n) <= o;
writeln(answer);
answer = m >= (n <= o);
writeln(answer, "\n");

write(">= and > permutations\n");
writeln("=========================");
answer = m >= n > o;
writeln(answer);
answer = (m >= n) > o;
writeln(answer);
answer = m >= (n > o);
writeln(answer, "\n");

write(">= and >= permutations\n");
writeln("=========================");
answer = m >= n >= o;
writeln(answer);
answer = (m >= n) >= o;
writeln(answer);
answer = m >= (n >= o);
writeln(answer, "\n");

write(">= and == permutations\n");
writeln("=========================");
answer = m >= n == o;
writeln(answer);
answer = (m >= n) == o;
writeln(answer);
answer = m >= (n == o);
writeln(answer, "\n");

write(">= and != permutations\n");
writeln("=========================");
answer = m >= n != o;
writeln(answer);
answer = (m >= n) != o;
writeln(answer);
answer = m >= (n != o);
writeln(answer, "\n");

write(">= and & permutations\n");
writeln("=========================");
answer = m >= n & o;
writeln(answer);
answer = (m >= n) & o;
writeln(answer);
answer = m >= (n & o);
writeln(answer, "\n");

write(">= and ^ permutations\n");
writeln("=========================");
answer = m >= n ^ o;
writeln(answer);
answer = (m >= n) ^ o;
writeln(answer);
answer = m >= (n ^ o);
writeln(answer, "\n");

write(">= and | permutations\n");
writeln("=========================");
answer = m >= n | o;
writeln(answer);
answer = (m >= n) | o;
writeln(answer);
answer = m >= (n | o);
writeln(answer, "\n");

write(">= and && permutations\n");
writeln("=========================");
answer = m >= n != 0 && 0 != o;
writeln(answer);
answer = (m >= n) != 0 && 0 != o;
writeln(answer);
answer = m >= (n != 0 && 0 != o);
writeln(answer, "\n");

write(">= and || permutations\n");
writeln("=========================");
answer = m >= n != 0 || 0 != o;
writeln(answer);
answer = (m >= n) != 0 || 0 != o;
writeln(answer);
answer = m >= (n != 0 || 0 != o);
writeln(answer, "\n");

write("== and * permutations\n");
writeln("=========================");
answer = m == n * o;
writeln(answer);
answer = (m == n) * o;
writeln(answer);
answer = m == (n * o);
writeln(answer, "\n");

write("== and / permutations\n");
writeln("=========================");
answer = m == n / o;
writeln(answer);
answer = (m == n) / o;
writeln(answer);
answer = m == (n / o);
writeln(answer, "\n");

write("== and % permutations\n");
writeln("=========================");
answer = m == n % o;
writeln(answer);
answer = (m == n) % o;
writeln(answer);
answer = m == (n % o);
writeln(answer, "\n");

write("== and + permutations\n");
writeln("=========================");
answer = m == n + o;
writeln(answer);
answer = (m == n) + o;
writeln(answer);
answer = m == (n + o);
writeln(answer, "\n");

write("== and - permutations\n");
writeln("=========================");
answer = m == n - o;
writeln(answer);
answer = (m == n) - o;
writeln(answer);
answer = m == (n - o);
writeln(answer, "\n");

write("== and < permutations\n");
writeln("=========================");
answer = m == n < o;
writeln(answer);
answer = (m == n) < o;
writeln(answer);
answer = m == (n < o);
writeln(answer, "\n");

write("== and <= permutations\n");
writeln("=========================");
answer = m == n <= o;
writeln(answer);
answer = (m == n) <= o;
writeln(answer);
answer = m == (n <= o);
writeln(answer, "\n");

write("== and > permutations\n");
writeln("=========================");
answer = m == n > o;
writeln(answer);
answer = (m == n) > o;
writeln(answer);
answer = m == (n > o);
writeln(answer, "\n");

write("== and >= permutations\n");
writeln("=========================");
answer = m == n >= o;
writeln(answer);
answer = (m == n) >= o;
writeln(answer);
answer = m == (n >= o);
writeln(answer, "\n");

write("== and == permutations\n");
writeln("=========================");
answer = m == n == o;
writeln(answer);
answer = (m == n) == o;
writeln(answer);
answer = m == (n == o);
writeln(answer, "\n");

write("== and != permutations\n");
writeln("=========================");
answer = m == n != o;
writeln(answer);
answer = (m == n) != o;
writeln(answer);
answer = m == (n != o);
writeln(answer, "\n");

write("== and & permutations\n");
writeln("=========================");
answer = m == n & o;
writeln(answer);
answer = (m == n) & o;
writeln(answer);
answer = m == (n & o);
writeln(answer, "\n");

write("== and ^ permutations\n");
writeln("=========================");
answer = m == n ^ o;
writeln(answer);
answer = (m == n) ^ o;
writeln(answer);
answer = m == (n ^ o);
writeln(answer, "\n");

write("== and | permutations\n");
writeln("=========================");
answer = m == n | o;
writeln(answer);
answer = (m == n) | o;
writeln(answer);
answer = m == (n | o);
writeln(answer, "\n");

write("== and && permutations\n");
writeln("=========================");
answer = m == n != 0 && 0 != o;
writeln(answer);
answer = (m == n) != 0 && 0 != o;
writeln(answer);
answer = m == (n != 0 && 0 != o);
writeln(answer, "\n");

write("== and || permutations\n");
writeln("=========================");
answer = m == n != 0 || 0 != o;
writeln(answer);
answer = (m == n) != 0 || 0 != o;
writeln(answer);
answer = m == (n != 0 || 0 != o);
writeln(answer, "\n");

write("!= and * permutations\n");
writeln("=========================");
answer = m != n * o;
writeln(answer);
answer = (m != n) * o;
writeln(answer);
answer = m != (n * o);
writeln(answer, "\n");

write("!= and / permutations\n");
writeln("=========================");
answer = m != n / o;
writeln(answer);
answer = (m != n) / o;
writeln(answer);
answer = m != (n / o);
writeln(answer, "\n");

write("!= and % permutations\n");
writeln("=========================");
answer = m != n % o;
writeln(answer);
answer = (m != n) % o;
writeln(answer);
answer = m != (n % o);
writeln(answer, "\n");

write("!= and + permutations\n");
writeln("=========================");
answer = m != n + o;
writeln(answer);
answer = (m != n) + o;
writeln(answer);
answer = m != (n + o);
writeln(answer, "\n");

write("!= and - permutations\n");
writeln("=========================");
answer = m != n - o;
writeln(answer);
answer = (m != n) - o;
writeln(answer);
answer = m != (n - o);
writeln(answer, "\n");

write("!= and < permutations\n");
writeln("=========================");
answer = m != n < o;
writeln(answer);
answer = (m != n) < o;
writeln(answer);
answer = m != (n < o);
writeln(answer, "\n");

write("!= and <= permutations\n");
writeln("=========================");
answer = m != n <= o;
writeln(answer);
answer = (m != n) <= o;
writeln(answer);
answer = m != (n <= o);
writeln(answer, "\n");

write("!= and > permutations\n");
writeln("=========================");
answer = m != n > o;
writeln(answer);
answer = (m != n) > o;
writeln(answer);
answer = m != (n > o);
writeln(answer, "\n");

write("!= and >= permutations\n");
writeln("=========================");
answer = m != n >= o;
writeln(answer);
answer = (m != n) >= o;
writeln(answer);
answer = m != (n >= o);
writeln(answer, "\n");

write("!= and == permutations\n");
writeln("=========================");
answer = m != n == o;
writeln(answer);
answer = (m != n) == o;
writeln(answer);
answer = m != (n == o);
writeln(answer, "\n");

write("!= and != permutations\n");
writeln("=========================");
answer = m != n != o;
writeln(answer);
answer = (m != n) != o;
writeln(answer);
answer = m != (n != o);
writeln(answer, "\n");

write("!= and & permutations\n");
writeln("=========================");
answer = m != n & o;
writeln(answer);
answer = (m != n) & o;
writeln(answer);
answer = m != (n & o);
writeln(answer, "\n");

write("!= and ^ permutations\n");
writeln("=========================");
answer = m != n ^ o;
writeln(answer);
answer = (m != n) ^ o;
writeln(answer);
answer = m != (n ^ o);
writeln(answer, "\n");

write("!= and | permutations\n");
writeln("=========================");
answer = m != n | o;
writeln(answer);
answer = (m != n) | o;
writeln(answer);
answer = m != (n | o);
writeln(answer, "\n");

write("!= and && permutations\n");
writeln("=========================");
answer = m != n != 0 && 0 != o;
writeln(answer);
answer = (m != n) != 0 && 0 != o;
writeln(answer);
answer = m != (n != 0 && 0 != o);
writeln(answer, "\n");

write("!= and || permutations\n");
writeln("=========================");
answer = m != n != 0 || 0 != o;
writeln(answer);
answer = (m != n) != 0 || 0 != o;
writeln(answer);
answer = m != (n != 0 || 0 != o);
writeln(answer, "\n");

write("& and * permutations\n");
writeln("=========================");
answer = m & n * o;
writeln(answer);
answer = (m & n) * o;
writeln(answer);
answer = m & (n * o);
writeln(answer, "\n");

write("& and / permutations\n");
writeln("=========================");
answer = m & n / o;
writeln(answer);
answer = (m & n) / o;
writeln(answer);
answer = m & (n / o);
writeln(answer, "\n");

write("& and % permutations\n");
writeln("=========================");
answer = m & n % o;
writeln(answer);
answer = (m & n) % o;
writeln(answer);
answer = m & (n % o);
writeln(answer, "\n");

write("& and + permutations\n");
writeln("=========================");
answer = m & n + o;
writeln(answer);
answer = (m & n) + o;
writeln(answer);
answer = m & (n + o);
writeln(answer, "\n");

write("& and - permutations\n");
writeln("=========================");
answer = m & n - o;
writeln(answer);
answer = (m & n) - o;
writeln(answer);
answer = m & (n - o);
writeln(answer, "\n");

write("& and < permutations\n");
writeln("=========================");
answer = m & n < o;
writeln(answer);
answer = (m & n) < o;
writeln(answer);
answer = m & (n < o);
writeln(answer, "\n");

write("& and <= permutations\n");
writeln("=========================");
answer = m & n <= o;
writeln(answer);
answer = (m & n) <= o;
writeln(answer);
answer = m & (n <= o);
writeln(answer, "\n");

write("& and > permutations\n");
writeln("=========================");
answer = m & n > o;
writeln(answer);
answer = (m & n) > o;
writeln(answer);
answer = m & (n > o);
writeln(answer, "\n");

write("& and >= permutations\n");
writeln("=========================");
answer = m & n >= o;
writeln(answer);
answer = (m & n) >= o;
writeln(answer);
answer = m & (n >= o);
writeln(answer, "\n");

write("& and == permutations\n");
writeln("=========================");
answer = m & n == o;
writeln(answer);
answer = (m & n) == o;
writeln(answer);
answer = m & (n == o);
writeln(answer, "\n");

write("& and != permutations\n");
writeln("=========================");
answer = m & n != o;
writeln(answer);
answer = (m & n) != o;
writeln(answer);
answer = m & (n != o);
writeln(answer, "\n");

write("& and & permutations\n");
writeln("=========================");
answer = m & n & o;
writeln(answer);
answer = (m & n) & o;
writeln(answer);
answer = m & (n & o);
writeln(answer, "\n");

write("& and ^ permutations\n");
writeln("=========================");
answer = m & n ^ o;
writeln(answer);
answer = (m & n) ^ o;
writeln(answer);
answer = m & (n ^ o);
writeln(answer, "\n");

write("& and | permutations\n");
writeln("=========================");
answer = m & n | o;
writeln(answer);
answer = (m & n) | o;
writeln(answer);
answer = m & (n | o);
writeln(answer, "\n");

write("& and && permutations\n");
writeln("=========================");
answer = m & n != 0 && 0 != o;
writeln(answer);
answer = (m & n) != 0 && 0 != o;
writeln(answer);
answer = m & (n != 0 && 0 != o);
writeln(answer, "\n");

write("& and || permutations\n");
writeln("=========================");
answer = m & n != 0 || 0 != o;
writeln(answer);
answer = (m & n) != 0 || 0 != o;
writeln(answer);
answer = m & (n != 0 || 0 != o);
writeln(answer, "\n");

write("^ and * permutations\n");
writeln("=========================");
answer = m ^ n * o;
writeln(answer);
answer = (m ^ n) * o;
writeln(answer);
answer = m ^ (n * o);
writeln(answer, "\n");

write("^ and / permutations\n");
writeln("=========================");
answer = m ^ n / o;
writeln(answer);
answer = (m ^ n) / o;
writeln(answer);
answer = m ^ (n / o);
writeln(answer, "\n");

write("^ and % permutations\n");
writeln("=========================");
answer = m ^ n % o;
writeln(answer);
answer = (m ^ n) % o;
writeln(answer);
answer = m ^ (n % o);
writeln(answer, "\n");

write("^ and + permutations\n");
writeln("=========================");
answer = m ^ n + o;
writeln(answer);
answer = (m ^ n) + o;
writeln(answer);
answer = m ^ (n + o);
writeln(answer, "\n");

write("^ and - permutations\n");
writeln("=========================");
answer = m ^ n - o;
writeln(answer);
answer = (m ^ n) - o;
writeln(answer);
answer = m ^ (n - o);
writeln(answer, "\n");

write("^ and < permutations\n");
writeln("=========================");
answer = m ^ n < o;
writeln(answer);
answer = (m ^ n) < o;
writeln(answer);
answer = m ^ (n < o);
writeln(answer, "\n");

write("^ and <= permutations\n");
writeln("=========================");
answer = m ^ n <= o;
writeln(answer);
answer = (m ^ n) <= o;
writeln(answer);
answer = m ^ (n <= o);
writeln(answer, "\n");

write("^ and > permutations\n");
writeln("=========================");
answer = m ^ n > o;
writeln(answer);
answer = (m ^ n) > o;
writeln(answer);
answer = m ^ (n > o);
writeln(answer, "\n");

write("^ and >= permutations\n");
writeln("=========================");
answer = m ^ n >= o;
writeln(answer);
answer = (m ^ n) >= o;
writeln(answer);
answer = m ^ (n >= o);
writeln(answer, "\n");

write("^ and == permutations\n");
writeln("=========================");
answer = m ^ n == o;
writeln(answer);
answer = (m ^ n) == o;
writeln(answer);
answer = m ^ (n == o);
writeln(answer, "\n");

write("^ and != permutations\n");
writeln("=========================");
answer = m ^ n != o;
writeln(answer);
answer = (m ^ n) != o;
writeln(answer);
answer = m ^ (n != o);
writeln(answer, "\n");

write("^ and & permutations\n");
writeln("=========================");
answer = m ^ n & o;
writeln(answer);
answer = (m ^ n) & o;
writeln(answer);
answer = m ^ (n & o);
writeln(answer, "\n");

write("^ and ^ permutations\n");
writeln("=========================");
answer = m ^ n ^ o;
writeln(answer);
answer = (m ^ n) ^ o;
writeln(answer);
answer = m ^ (n ^ o);
writeln(answer, "\n");

write("^ and | permutations\n");
writeln("=========================");
answer = m ^ n | o;
writeln(answer);
answer = (m ^ n) | o;
writeln(answer);
answer = m ^ (n | o);
writeln(answer, "\n");

write("^ and && permutations\n");
writeln("=========================");
answer = m ^ n != 0 && 0 != o;
writeln(answer);
answer = (m ^ n) != 0 && 0 != o;
writeln(answer);
answer = m ^ (n != 0 && 0 != o);
writeln(answer, "\n");

write("^ and || permutations\n");
writeln("=========================");
answer = m ^ n != 0 || 0 != o;
writeln(answer);
answer = (m ^ n) != 0 || 0 != o;
writeln(answer);
answer = m ^ (n != 0 || 0 != o);
writeln(answer, "\n");

write("| and * permutations\n");
writeln("=========================");
answer = m | n * o;
writeln(answer);
answer = (m | n) * o;
writeln(answer);
answer = m | (n * o);
writeln(answer, "\n");

write("| and / permutations\n");
writeln("=========================");
answer = m | n / o;
writeln(answer);
answer = (m | n) / o;
writeln(answer);
answer = m | (n / o);
writeln(answer, "\n");

write("| and % permutations\n");
writeln("=========================");
answer = m | n % o;
writeln(answer);
answer = (m | n) % o;
writeln(answer);
answer = m | (n % o);
writeln(answer, "\n");

write("| and + permutations\n");
writeln("=========================");
answer = m | n + o;
writeln(answer);
answer = (m | n) + o;
writeln(answer);
answer = m | (n + o);
writeln(answer, "\n");

write("| and - permutations\n");
writeln("=========================");
answer = m | n - o;
writeln(answer);
answer = (m | n) - o;
writeln(answer);
answer = m | (n - o);
writeln(answer, "\n");

write("| and < permutations\n");
writeln("=========================");
answer = m | n < o;
writeln(answer);
answer = (m | n) < o;
writeln(answer);
answer = m | (n < o);
writeln(answer, "\n");

write("| and <= permutations\n");
writeln("=========================");
answer = m | n <= o;
writeln(answer);
answer = (m | n) <= o;
writeln(answer);
answer = m | (n <= o);
writeln(answer, "\n");

write("| and > permutations\n");
writeln("=========================");
answer = m | n > o;
writeln(answer);
answer = (m | n) > o;
writeln(answer);
answer = m | (n > o);
writeln(answer, "\n");

write("| and >= permutations\n");
writeln("=========================");
answer = m | n >= o;
writeln(answer);
answer = (m | n) >= o;
writeln(answer);
answer = m | (n >= o);
writeln(answer, "\n");

write("| and == permutations\n");
writeln("=========================");
answer = m | n == o;
writeln(answer);
answer = (m | n) == o;
writeln(answer);
answer = m | (n == o);
writeln(answer, "\n");

write("| and != permutations\n");
writeln("=========================");
answer = m | n != o;
writeln(answer);
answer = (m | n) != o;
writeln(answer);
answer = m | (n != o);
writeln(answer, "\n");

write("| and & permutations\n");
writeln("=========================");
answer = m | n & o;
writeln(answer);
answer = (m | n) & o;
writeln(answer);
answer = m | (n & o);
writeln(answer, "\n");

write("| and ^ permutations\n");
writeln("=========================");
answer = m | n ^ o;
writeln(answer);
answer = (m | n) ^ o;
writeln(answer);
answer = m | (n ^ o);
writeln(answer, "\n");

write("| and | permutations\n");
writeln("=========================");
answer = m | n | o;
writeln(answer);
answer = (m | n) | o;
writeln(answer);
answer = m | (n | o);
writeln(answer, "\n");

write("| and && permutations\n");
writeln("=========================");
answer = m | n != 0 && 0 != o;
writeln(answer);
answer = (m | n) != 0 && 0 != o;
writeln(answer);
answer = m | (n != 0 && 0 != o);
writeln(answer, "\n");

write("| and || permutations\n");
writeln("=========================");
answer = m | n != 0 || 0 != o;
writeln(answer);
answer = (m | n) != 0 || 0 != o;
writeln(answer);
answer = m | (n != 0 || 0 != o);
writeln(answer, "\n");

write("&& and * permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n * o;
writeln(answer);
answer = (m != 0 && 0 != n) * o;
writeln(answer);
answer = m != 0 && 0 != (n * o);
writeln(answer, "\n");

write("&& and / permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n / o;
writeln(answer);
answer = (m != 0 && 0 != n) / o;
writeln(answer);
answer = m != 0 && 0 != (n / o);
writeln(answer, "\n");

write("&& and % permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n % o;
writeln(answer);
answer = (m != 0 && 0 != n) % o;
writeln(answer);
answer = m != 0 && 0 != (n % o);
writeln(answer, "\n");

write("&& and + permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n + o;
writeln(answer);
answer = (m != 0 && 0 != n) + o;
writeln(answer);
answer = m != 0 && 0 != (n + o);
writeln(answer, "\n");

write("&& and - permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n - o;
writeln(answer);
answer = (m != 0 && 0 != n) - o;
writeln(answer);
answer = m != 0 && 0 != (n - o);
writeln(answer, "\n");

write("&& and < permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n < o;
writeln(answer);
answer = (m != 0 && 0 != n) < o;
writeln(answer);
answer = m != 0 && 0 != (n < o);
writeln(answer, "\n");

write("&& and <= permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n <= o;
writeln(answer);
answer = (m != 0 && 0 != n) <= o;
writeln(answer);
answer = m != 0 && 0 != (n <= o);
writeln(answer, "\n");

write("&& and > permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n > o;
writeln(answer);
answer = (m != 0 && 0 != n) > o;
writeln(answer);
answer = m != 0 && 0 != (n > o);
writeln(answer, "\n");

write("&& and >= permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n >= o;
writeln(answer);
answer = (m != 0 && 0 != n) >= o;
writeln(answer);
answer = m != 0 && 0 != (n >= o);
writeln(answer, "\n");

write("&& and == permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n == o;
writeln(answer);
answer = (m != 0 && 0 != n) == o;
writeln(answer);
answer = m != 0 && 0 != (n == o);
writeln(answer, "\n");

write("&& and != permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n != o;
writeln(answer);
answer = (m != 0 && 0 != n) != o;
writeln(answer);
answer = m != 0 && 0 != (n != o);
writeln(answer, "\n");

write("&& and & permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n & o;
writeln(answer);
answer = (m != 0 && 0 != n) & o;
writeln(answer);
answer = m != 0 && 0 != (n & o);
writeln(answer, "\n");

write("&& and ^ permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n ^ o;
writeln(answer);
answer = (m != 0 && 0 != n) ^ o;
writeln(answer);
answer = m != 0 && 0 != (n ^ o);
writeln(answer, "\n");

write("&& and | permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n | o;
writeln(answer);
answer = (m != 0 && 0 != n) | o;
writeln(answer);
answer = m != 0 && 0 != (n | o);
writeln(answer, "\n");

write("&& and && permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n != 0 && 0 != o;
writeln(answer);
answer = (m != 0 && 0 != n) != 0 && 0 != o;
writeln(answer);
answer = m != 0 && 0 != (n != 0 && 0 != o);
writeln(answer, "\n");

write("&& and || permutations\n");
writeln("=========================");
answer = m != 0 && 0 != n != 0 || 0 != o;
writeln(answer);
answer = (m != 0 && 0 != n) != 0 || 0 != o;
writeln(answer);
answer = m != 0 && 0 != (n != 0 || 0 != o);
writeln(answer, "\n");

write("or and * permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n * o;
writeln(answer);
answer = (m != 0 || 0 != n) * o;
writeln(answer);
answer = m != 0 || 0 != (n * o);
writeln(answer, "\n");

write("or and / permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n / o;
writeln(answer);
answer = (m != 0 || 0 != n) / o;
writeln(answer);
answer = m != 0 || 0 != (n / o);
writeln(answer, "\n");

write("or and % permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n % o;
writeln(answer);
answer = (m != 0 || 0 != n) % o;
writeln(answer);
answer = m != 0 || 0 != (n % o);
writeln(answer, "\n");

write("or and + permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n + o;
writeln(answer);
answer = (m != 0 || 0 != n) + o;
writeln(answer);
answer = m != 0 || 0 != (n + o);
writeln(answer, "\n");

write("or and - permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n - o;
writeln(answer);
answer = (m != 0 || 0 != n) - o;
writeln(answer);
answer = m != 0 || 0 != (n - o);
writeln(answer, "\n");

write("or and < permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n < o;
writeln(answer);
answer = (m != 0 || 0 != n) < o;
writeln(answer);
answer = m != 0 || 0 != (n < o);
writeln(answer, "\n");

write("or and <= permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n <= o;
writeln(answer);
answer = (m != 0 || 0 != n) <= o;
writeln(answer);
answer = m != 0 || 0 != (n <= o);
writeln(answer, "\n");

write("or and > permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n > o;
writeln(answer);
answer = (m != 0 || 0 != n) > o;
writeln(answer);
answer = m != 0 || 0 != (n > o);
writeln(answer, "\n");

write("or and >= permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n >= o;
writeln(answer);
answer = (m != 0 || 0 != n) >= o;
writeln(answer);
answer = m != 0 || 0 != (n >= o);
writeln(answer, "\n");

write("or and == permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n == o;
writeln(answer);
answer = (m != 0 || 0 != n) == o;
writeln(answer);
answer = m != 0 || 0 != (n == o);
writeln(answer, "\n");

write("or and != permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n != o;
writeln(answer);
answer = (m != 0 || 0 != n) != o;
writeln(answer);
answer = m != 0 || 0 != (n != o);
writeln(answer, "\n");

write("or and & permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n & o;
writeln(answer);
answer = (m != 0 || 0 != n) & o;
writeln(answer);
answer = m != 0 || 0 != (n & o);
writeln(answer, "\n");

write("or and ^ permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n ^ o;
writeln(answer);
answer = (m != 0 || 0 != n) ^ o;
writeln(answer);
answer = m != 0 || 0 != (n ^ o);
writeln(answer, "\n");

write("or and | permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n | o;
writeln(answer);
answer = (m != 0 || 0 != n) | o;
writeln(answer);
answer = m != 0 || 0 != (n | o);
writeln(answer, "\n");

write("or and && permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n != 0 && 0 != o;
writeln(answer);
answer = (m != 0 || 0 != n) != 0 && 0 != o;
writeln(answer);
answer = m != 0 || 0 != (n != 0 && 0 != o);
writeln(answer, "\n");

write("or and || permutations\n");
writeln("=========================");
answer = m != 0 || 0 != n != 0 || 0 != o;
writeln(answer);
answer = (m != 0 || 0 != n) != 0 || 0 != o;
writeln(answer);
answer = m != 0 || 0 != (n != 0 || 0 != o);
writeln(answer, "\n");
