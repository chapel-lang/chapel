use GMP;
// Tests all of the number theoretic functions in that section of the record
// jacobi and legendre functions will be tested in a different test
var a = new BigInt(27);
var b = new BigInt(19);
var c = new BigInt("1302397");
var d = new BigInt(5);
var e = new BigInt(375);
var val = a.probab_prime_p(15);
prime_parse(val);
val = b.probab_prime_p(15);
prime_parse(val);
val = c.probab_prime_p(50);
prime_parse(val);

a.nextprime(a);
writeln(a);
a.nextprime(a);
writeln(a);

b.set(120);
a.gcd(a, b);
writeln("gcd of a prime and an integer is ", a);
a.set(18);
a.gcd(a, b);
writeln("gcd(18, 120) = ", a);
a.gcd_ui(b, 100);
writeln("gcd(120, 100) = ", a);
c.set(720);
d.set(1000);
a.gcdext(e, b, c, d);
writeln("gcd(", c, ", ", d, ") = ", a);
writeln(c, " * ", e, " + ", d, " * ", b, " = ", a);

b.set(10);
c.set(15);
a.lcm(b, c);
writeln("least common multiple of ", b, " and ", c, " is ", a);
a.lcm_ui(b, 24);
writeln("least common multiple of ", b, " and 24 is ", a);
c.set(23);
val = a.invert(b, c);
writeln("the inverse of ", b, " mod ", c, " is ", a);

c.set(36);
b.set(2);
a.remove(c, b);
writeln(c, " without factors of ", b, " is ", a);

a.fac_ui(7);
writeln("7! = ", a);
b.set(10);
a.bin_ui(b, 7);
writeln("binomial coefficient ", b, " over 7 = ", a);
a.bin_uiui(9, 7);
writeln("binomial coefficient 9 over 7 = ", a);

b.set(0);
a.fib_ui(7);
writeln("the 7th Fibonacci number is ", a);
//a.fib2_ui(b, 6);
mpz_fib2_ui(a.mpz, b.mpz, 6);
writeln("the 5th and 6th Fibonacci numbers are ", b, " and ", a);

a.lucnum_ui(7);
writeln("the 7th Lucas number is ", a);
a.lucnum2_ui(b, 6);
writeln("the 5th and 6th Lucas numbers are ", b, " and ", a);

//use this to print out the results of probab_prime_p
proc prime_parse(val){
  select val { 
    when 1 do writeln("probably prime");
    when 2 do writeln("definitely prime");
    when 0 do writeln("Not prime");
    }
}
