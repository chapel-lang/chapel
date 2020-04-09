/** paramFibonacci.chpl: Calculate fibonacci numbers at compile time
 **/

config param x = 50;

proc fib(param n: int(64)) param
  return if (n <= 1) then n else fib(n-1) + fib(n-2);

writeln("Fibonacci(", x, ")=", fib(x));
