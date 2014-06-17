var m = 2;
var n = 3;
var o = 5;
var answer = 0;

var fls = false;
var tru = true;
var answerb = false;

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
answerb = fls & tru && rettru();
writeln(answerb);
answerb = (fls & tru) && rettru();
writeln(answerb);
answerb = fls & (tru && rettru());
writeln(answerb, "\n");

write("& and || permutations\n");
writeln("=========================");
answerb = fls & fls || tru;
writeln(answerb);
answerb = (fls & fls) || tru;
writeln(answerb);
answerb = fls & (fls || tru);
writeln(answerb, "\n");

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
answerb = tru ^ fls && rettru();
writeln(answerb);
answerb = (tru ^ fls) && rettru();
writeln(answerb);
answerb = tru ^ (fls && rettru());
writeln(answerb, "\n");

write("^ and || permutations\n");
writeln("=========================");
answerb = tru ^ tru || rettru();
writeln(answerb);
answerb = (tru ^ tru) || rettru();
writeln(answerb);
answerb = tru ^ (tru || rettru());
writeln(answerb, "\n");

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
answerb = tru | fls && retfls();
writeln(answerb);
answerb = (tru | fls) && retfls();
writeln(answerb);
answerb = tru | (fls && retfls());
writeln(answerb, "\n");

write("| and || permutations\n");
writeln("=========================");
answerb = tru | fls || rettru();
writeln(answerb);
answerb = (tru | fls) || rettru();
writeln(answerb);
answerb = tru | (fls || rettru());
writeln(answerb, "\n");

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
answerb = fls && fls & rettru();
writeln(answerb);
answerb = (fls && fls) & rettru();
writeln(answerb);
answerb = fls && (fls & rettru());
writeln(answerb, "\n");

write("&& and ^ permutations\n");
writeln("=========================");
answerb = fls && fls ^ tru;
writeln(answerb);
answerb = (fls && fls) ^ tru;
writeln(answerb);
answerb = fls && (fls ^ tru);
writeln(answerb, "\n");

write("&& and | permutations\n");
writeln("=========================");
answerb = fls && fls | tru;
writeln(answerb);
answerb = (fls && fls) | tru;
writeln(answerb);
answerb = fls && (fls | tru);
writeln(answerb, "\n");

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
answerb = tru || tru & fls;
writeln(answerb);
answerb = (tru || tru) & fls;
writeln(answerb);
answerb = tru || (tru & fls);
writeln(answerb, "\n");

write("or and ^ permutations\n");
writeln("=========================");
answerb = tru || tru ^ tru;
writeln(answerb);
answerb = (tru || tru) ^ tru;
writeln(answerb);
answerb = tru || (tru ^ tru);
writeln(answerb, "\n");

// BLC: I can't figure out how to distinguish this case
write("or and | permutations\n");
writeln("=========================");
answerb = fls || fls | tru;
writeln(answerb);
answerb = (fls || fls) | tru;
writeln(answerb);
answerb = fls || (fls | tru);
writeln(answerb, "\n");

// BLC: I can't figure out how to distinguish this case
write("or and && permutations\n");
writeln("=========================");
answerb = fls || tru && fls;
writeln(answerb);
answerb = (fls || tru) && fls;
writeln(answerb);
answerb = fls || (tru && fls);
writeln(answerb, "\n");

// BLC: I can't figure out how to distinguish this case
write("or and || permutations\n");
writeln("=========================");
answerb = fls || fls || fls;
writeln(answerb);
answerb = (fls || fls) || fls;
writeln(answerb);
answerb = fls || (fls || fls);
writeln(answerb, "\n");

proc rettru() {
  writeln("In rettru");
  return true;
}

proc retfls() {
  writeln("In retfls");
  return false;
}
