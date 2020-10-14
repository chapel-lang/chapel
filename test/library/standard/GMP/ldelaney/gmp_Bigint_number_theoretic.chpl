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

var val = a.probab_prime_p(15);
prime_parse(a, val);

val = b.probab_prime_p(15);
prime_parse(b, val);

val = c.probab_prime_p(50);
prime_parse(c, val);

writeln();

proc prime_parse(val, isProbPrime) {
  if isProbPrime == 0 then
    writeln(val, " not prime");
  if isProbPrime == 1 || isProbPrime == 2 then
    writeln(val, " prime or probably prime");
}


//
// Compute a sequence of primes
//

a.nextprime(a);
writeln(a);

a.nextprime(a);
writeln(a);

writeln();




//
// Compute GCD
//

b.set(120);
a.gcd(a, b);
writeln("gcd of a prime and an integer is ", a);

a.set(18);
a.gcd(a, b);
writeln("gcd(18, 120) = ", a);

a.gcd(b, 100);
writeln("gcd(120, 100) = ", a);

c.set(720);
d.set(1000);
a.gcdext(e, b, c, d);

writeln("gcd(", c, ", ", d, ") = ", a);
writeln(c, " * ", e, " + ", d, " * ", b, " = ", a);

writeln();



//
// Compute LCM
//

b.set(10);
c.set(15);
a.lcm(b, c);
writeln("least common multiple of ", b, " and ", c, " is ", a);

a.lcm(b, 24);
writeln("least common multiple of ", b, " and 24 is ", a);

writeln();


//
// Inverse
//
c.set(23);
val = a.invert(b, c);
writeln("the inverse of ", b, " mod ", c, " is ", a);

writeln();



//
// Factors
//

c.set(36);
b.set(2);
a.remove(c, b);
writeln(c, " without factors of ", b, " is ", a);

writeln();



//
// Factorial
//

a.fac(7);
writeln("7! = ", a);
writeln();


//
// Binomial coefficient
//

b.set(10);
a.bin(b, 7);
writeln("binomial coefficient ", b, " over 7 = ", a);

a.bin(9, 7);
writeln("binomial coefficient 9 over 7 = ", a);

writeln();


//
// Fibonacci
//

a.fib(7);
writeln("the 7th Fibonacci number is ", a);

// a.fib2(b, 6);
b.set(0);
mpz_fib2_ui(a.mpz, b.mpz, 6);
writeln("the 5th and 6th Fibonacci numbers are ", b, " and ", a);

writeln();




//
// Lucas number
//

a.lucnum(7);
writeln("the 7th Lucas number is ", a);

a.lucnum2(b, 6);
writeln("the 5th and 6th Lucas numbers are ", b, " and ", a);
