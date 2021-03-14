public use Time;

use Configs;
public use Potential;

record SubTimer {
  var D : domain(string);
  var A : [D] Timer;

  proc init(names : string...) {
    this.complete();
    for n in names do D.add(n);
  }

  proc this(idx:string) ref {
    return A[idx];
  }

  iter these() {
    for idx in D {
      yield (idx, A[idx]);
    }
  }

  proc print() {
    for name in D {
      writeln(name, ": ", A[name].elapsed());
    }
  }
}

var Timers = new SubTimer("total", "commReduce", "redistribute",
                          "computeForce", "position", "velocity");

var RedistTimers = new SubTimer("atomHalo", "updateLinkCells", "pbc", "sort");

inline proc dot(A:vec3, B:vec3) {
  return A(0)*B(0) + A(1)*B(1) + A(2)*B(2);
}

proc dateString() {
  use DateTime;
  return datetime.now().strftime("%F, %T");
}

proc timestampMessage(msg : string) {
  use DateTime;
  const timeStr = datetime.now().ctime();
  writeln(timeStr, ": ", msg);
  writeln();
}

proc computeBoxInfo(PotLat, PotCutoff) {
  const latticeConstant = if lat < 0.0 then PotLat else lat;

  const globalExtent = (nx,ny,nz):vec3 * latticeConstant;
  const numBoxes     = (globalExtent / PotCutoff):vec3int;
  const boxSize      = globalExtent / numBoxes;

  return (globalExtent, numBoxes, boxSize, latticeConstant);
}

/**
  * Use the Box-Muller method to sample a Gaussian distribution with
  * zero mean and unit variance.  To ensure the same input seed always
  * generates the same returned value we do not use the standard
  * technique of saving one of the two generated randoms for the next
  * call.
  *
  * :arg seed: Seed for generator.
  * :type seed: int
  *
  * :returns: A pseudo-random number in the interval (-infinity, infinity).
  * :rtype: real
 **/
proc gasdev(inout seed: uint(64)): real {
  var rsq, v1, v2: real;

  do
  {
    v1 = 2.0*lcg61(seed) - 1.0;
    v2 = 2.0*lcg61(seed) - 1.0;
    rsq = v1*v1+v2*v2;
  } while (rsq >= 1.0 || rsq == 0.0);

  return v2*sqrt(-2.0*log(rsq) / rsq);
}


/**
  *  A 61-bit prime modulus linear congruential generator with
  * modulus = 2^61 -1.
  *
  *
  * :arg seed: Seed for generator. Modified by reference.
  * :type seed: int
  *
  * :returns: A pseudo-random number in the interval [0, 1].
  * :rtype: real
 **/
proc lcg61(ref seed: uint(64)): real {
  const convertToDouble:real = 1.0 / (2305843009213693951: uint(64));

  seed *= 437799614237992725: uint(64);
  seed %= 2305843009213693951: uint(64);

  return seed:real * convertToDouble;
}


/**
  * Forms a 64-bit seed for lcg61 from the combination of 2 32-bit Knuth
  * multiplicative hashes, then runs off 10 values to pass up the worst
  * of the early low-bit correlations.
  *
  * :arg id: An id number such as an atom gid that is unique to
  * each entity that requires random numbers.
  *
  * :arg callSite: A unique number for each call site in the code
  * that needs to generate random seeds.  Using a different value for
  * callSite allows different parts of the code to obtain different
  * random streams for the same id.
  *
  * :returns: A 64-bit seed that is unique to the id and call site.
  * :rtype: uint(64)
 **/
proc mkSeed(id: uint(32), callSite: uint(32)): uint(64) {
  var mult : uint(32) = 2654435761;
  var s1: uint(32) = id * mult;
  var s2: uint(32) = (id + callSite) * mult;

  var hex: uint(64) = 0x100000000:uint(64);
  var iSeed: uint(64) = ((hex * (s1:uint(64))) + s2:uint(64));

  for j in 1..10 do lcg61(iSeed);

  return iSeed;
}

proc mkSeed(id:integral, callSite:integral) {
  return mkSeed(id.safeCast(uint(32)), callSite.safeCast(uint(32)));
}
