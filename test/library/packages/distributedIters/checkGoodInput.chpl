/*
  Test to check that the distributedGuided() iterator from the DistributedIters
  module accepts all valid inputs (ranges and domains).
*/
use BlockDist;
use DistributedIters;

// Common variables.
config const n:int=1000;
var rng:range=0..n;
var A:[rng] int=0;

/*
  Test mode (iterator):

  - ``dynamic``
    The distributed dynamic load-balancing iterator.

  - ``guided``
    The distributed guided load-balancing iterator.
*/
enum iterator
{
  dynamic,
  guided
};

/*
  Used to select the test mode (which iterator to use). Defaults to ``guided``.
  See :data:`iterator` for more information.
*/
config const mode:iterator = iterator.guided;

/*
  Ranges.
*/
var testRange:range=0..n;
var testStridedRange=testRange by 2;
var testCountedRange=testRange # 5;
var testStridedCountedRange=testStridedRange # 5;
var testAlignedRange=testStridedRange align 1;

/*
  Domains.
*/
var testEmptyDomain:domain(1);
const testDomainLiteral={1..n};
var testAssociativeDomain:domain(int);
testAssociativeDomain += 3;
var testSparseDomain:sparse subdomain(testDomainLiteral);
const testBlockDistributedDomain={1..n} dmapped Block(boundingBox={1..n});

select mode
{
  when iterator.dynamic
  {
    writeln("Checking a range...");
    for i in distributedDynamic(testRange)
    do A[i] = A[i]+1;

    writeln("Checking a strided range...");
    for i in distributedDynamic(testStridedRange)
    do A[i] = A[i]+1;

    writeln("Checking a counted range...");
    for i in distributedDynamic(testCountedRange)
    do A[i] = A[i]+1;

    writeln("Checking a strided counted range...");
    for i in distributedDynamic(testStridedCountedRange)
    do A[i] = A[i]+1;

    writeln("Checking an aligned range...");
    for i in distributedDynamic(testAlignedRange)
    do A[i] = A[i]+1;

    writeln("Checking an empty domain...");
    for i in distributedDynamic(testEmptyDomain)
    do A[i] = A[i]+1;

    writeln("Checking a domain literal...");
    for i in distributedDynamic(testDomainLiteral)
    do A[i] = A[i]+1;

    writeln("Checking an associative domain...");
    for i in distributedDynamic(testAssociativeDomain)
    do A[i] = A[i]+1;

    writeln("Checking a sparse domain...");
    for i in distributedDynamic(testSparseDomain)
    do A[i] = A[i]+1;

    writeln("Checking a block-distributed domain...");
    for i in distributedDynamic(testBlockDistributedDomain)
    do A[i] = A[i]+1;
  }
  when iterator.guided
  {
    writeln("Checking a range...");
    for i in distributedGuided(testRange)
    do A[i] = A[i]+1;

    writeln("Checking a strided range...");
    for i in distributedGuided(testStridedRange)
    do A[i] = A[i]+1;

    writeln("Checking a counted range...");
    for i in distributedGuided(testCountedRange)
    do A[i] = A[i]+1;

    writeln("Checking a strided counted range...");
    for i in distributedGuided(testStridedCountedRange)
    do A[i] = A[i]+1;

    writeln("Checking an aligned range...");
    for i in distributedGuided(testAlignedRange)
    do A[i] = A[i]+1;

    writeln("Checking an empty domain...");
    for i in distributedGuided(testEmptyDomain)
    do A[i] = A[i]+1;

    writeln("Checking a domain literal...");
    for i in distributedGuided(testDomainLiteral)
    do A[i] = A[i]+1;

    writeln("Checking an associative domain...");
    for i in distributedGuided(testAssociativeDomain)
    do A[i] = A[i]+1;

    writeln("Checking a sparse domain...");
    for i in distributedGuided(testSparseDomain)
    do A[i] = A[i]+1;

    writeln("Checking a block-distributed domain...");
    for i in distributedGuided(testBlockDistributedDomain)
    do A[i] = A[i]+1;
  }
}

// EOF
