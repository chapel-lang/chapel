//
// Demonstrate using accessors to update the fields in a range.
//

var r = 1..10;
var r2 = r by 2;
r2.low = 0;  // Does not change alignment.

writeln(r2);  // 0..10 by 2 align 1

// Add one index without changing the alignment.
r2.high += r2.stride;
writeln(r2);

// Now change the alignment.
r2.alignment = 0;
writeln(r2);  // 0..12 by 2 [align 0]

r2.stride = 3; // Now I'd rather count by threes.
writeln(r2);  // 0..12 by 3

// Sieve of Eratosthenes.
var ASpace = 2..100;
var A : [ASpace] bool;  // True if the matching index is a prime.
for i in ASpace do A[i] = true;
var ra = ASpace;
for i in 2..10 {
  ra.low = i; ra.stride = i; // ra.alignment = 0;
  for j in ra do if j > i then A[i] = false;
}
writeln("The primes up to 100 are:");
for i in ASpace do
  if A[i] then write(" ", i);
writeln();

var r3 = r2[..9 by 2];  // Has ambiguous alignment.
r3.aligned = true;   // Assert that it is aligned now.
for i in r3 do write(" ", i);
writeln();
