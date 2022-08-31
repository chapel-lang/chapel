/* Sieve of Eratosthenes Example
 *
 * This example program finds prime numbers up to a limit using the
 * sieve of Eratosthenes.
 *
 */

use List;

config var limit = 100; // maximum number to check primality of

proc main() {
    var primes : list(uint);
    param lowerBound = 2;
    var potentialPrimes = lowerBound..limit;
    var couldBePrime : [potentialPrimes] bool = true;

    for potentialPrime in potentialPrimes {
      if (couldBePrime[potentialPrime]) {
        primes.append(potentialPrime);
        for i in potentialPrime*2..limit by potentialPrime {
          couldBePrime[i] = false;
        }
      }
    }

    writeln(primes);
}
