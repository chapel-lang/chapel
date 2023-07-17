/*
 * Sieve of Eratosthenes Example
 *
 * This example program finds prime numbers up to a limit using the
 * sieve of Eratosthenes.
 */

use List;

config var limit : uint = 1000; // maximum number to include in the sieve

proc main() {
  // list of confirmed primes
  var primes : list(uint);
  // range of potential prime numbers
  const sieveRange = 2..limit;
  // flags for known composite (sieved out) numbers
  var knownComposite : [sieveRange] bool = false;

  // repeatedly add the first potential prime to our primes list, then exclude
  // all following multiples
  for potentialPrime in sieveRange {
    if (!knownComposite[potentialPrime]) {
      primes.pushBack(potentialPrime);
      for i in potentialPrime*2..limit by potentialPrime {
        knownComposite[i] = true;
      }
    }
  }

  writeln(primes);
  writeln("Count of primes up to ", limit, ": ", primes.size);
}
