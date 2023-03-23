use BigInteger;
use GMP;

// Tests all of the number theoretic functions contained in the record
// the jacobi and legendre functions will be tested elsewhere

var a   = new bigint(27);
var b   = new bigint(19);
var c   = new bigint("1302397");
var d   = new bigint(5);
var e   = new bigint(375);



//
// Prime tester
//

var val = a.probablyPrime(15);
prime_parse(a, val);

val = b.probablyPrime(15);
prime_parse(b, val);

val = c.probablyPrime(50);
prime_parse(c, val);

writeln();

proc prime_parse(val, isProbPrime) {
  use primality;
  if isProbPrime == notPrime then
    writeln(val, " not prime");
  if isProbPrime == maybePrime || isProbPrime == isPrime then
    writeln(val, " prime or probably prime");
}


//
// Compute a sequence of primes
//

nextprime(a, a);
writeln(a);

nextprime(a, a);
writeln(a);

writeln();




//
// Compute GCD
//

b.set(120);
gcd(a, a, b);
writeln("gcd of a prime and an integer is ", a);

a.set(18);
gcd(a, a, b);
writeln("gcd(18, 120) = ", a);

gcd(a, b, 100);
writeln("gcd(120, 100) = ", a);

c.set(720);
d.set(1000);
gcd(a, c, d, e, b);

writeln("gcd(", c, ", ", d, ") = ", a);
writeln(c, " * ", e, " + ", d, " * ", b, " = ", a);

writeln();



//
// Compute LCM
//

b.set(10);
c.set(15);
lcm(a, b, c);
writeln("least common multiple of ", b, " and ", c, " is ", a);

lcm(a, b, 24);
writeln("least common multiple of ", b, " and 24 is ", a);

writeln();


//
// Inverse
//
c.set(23);
invert(a, b, c);
writeln("the inverse of ", b, " mod ", c, " is ", a);

writeln();



//
// Factors
//

c.set(36);
b.set(2);
removeFactor(a, c, b);
writeln(c, " without factors of ", b, " is ", a);

writeln();



//
// Factorial
//

fac(a, 7);
writeln("7! = ", a);
writeln();


//
// Binomial coefficient
//

b.set(10);
bin(a, b, 7);
writeln("binomial coefficient ", b, " over 7 = ", a);

bin(a, 9, 7);
writeln("binomial coefficient 9 over 7 = ", a);

writeln();


//
// Fibonacci
//

fib(a, 7);
writeln("the 7th Fibonacci number is ", a);


b.set(0);
fib2(a, b, 6);
writeln("the 5th and 6th Fibonacci numbers are ", b, " and ", a);

writeln();




//
// Lucas number
//

lucnum(a, 7);
writeln("the 7th Lucas number is ", a);

lucnum2(a, b, 6);
writeln("the 5th and 6th Lucas numbers are ", b, " and ", a);
