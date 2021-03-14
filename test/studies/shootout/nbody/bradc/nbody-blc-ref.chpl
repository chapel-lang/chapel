/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Albert Sidelnik
   modified by Brad Chamberlain
*/

const pi = 3.141592653589793,
      solarMass = 4 * pi**2,
      daysPerYear = 365.24;

record body {
  var pos: 3*real;
  // TODO: reference versions use this -- is it better?  var filler: real;
  var v: 3*real;
  var mass: real;

  proc offsetMomentum(p) {
    v = -p / solarMass;
  }
}

const jupiter = new body(pos = (4.84143144246472090e+00,
                                -1.16032004402742839e+00,
                                -1.03622044471123109e-01),
                         v = (1.66007664274403694e-03 * daysPerYear,
                              7.69901118419740425e-03 * daysPerYear,
                              -6.90460016972063023e-05 * daysPerYear),
                         mass = 9.54791938424326609e-04 * solarMass),
  
      saturn = new body(pos = (8.34336671824457987e+00,
                               4.12479856412430479e+00,
                               -4.03523417114321381e-01),
                        v = (-2.76742510726862411e-03 * daysPerYear,
                             4.99852801234917238e-03 * daysPerYear,
                             2.30417297573763929e-05 * daysPerYear),
                        mass = 2.85885980666130812e-04 * solarMass),

      uranus = new body(pos = (1.28943695621391310e+01,
                               -1.51111514016986312e+01,
                               -2.23307578892655734e-01),
                        v = (2.96460137564761618e-03 * daysPerYear,
                             2.37847173959480950e-03 * daysPerYear,
                             -2.96589568540237556e-05 * daysPerYear),
                        mass = 4.36624404335156298e-05 * solarMass),

      neptune = new body(pos = (1.53796971148509165e+01,
                                -2.59193146099879641e+01,
                                1.79258772950371181e-01),
                         v = (2.68067772490389322e-03 * daysPerYear,
                              1.62824170038242295e-03 * daysPerYear,
                              -9.51592254519715870e-05 * daysPerYear),
                         mass = 5.15138902046611451e-05 * solarMass),

      sun = new body(mass = solarMass);

inline proc sumOfSquares(x:_tuple) where isHomogeneousTuple(x) {
  if x.size == 1 then
    return x(0)**2;
  else
    return (x(0)**2 + sumOfSquares(chpl__tupleRest(x)));
}

record NBodySystem {
  var bodies = [sun, jupiter, saturn, uranus, neptune];
  const numbodies = bodies.size;

  proc postinit() {
    var p: 3*real;
    for b in bodies do
      p += b.v * b.mass;  // TODO: reduce?
    bodies[0].offsetMomentum(p);
  }

  proc advance(dt) {
    // TODO: Can we use a triangular iterator without hurting performance
    for i in 0..<numbodies {
      for j in i+1..<numbodies {
        ref b1 = bodies[i],
               b2 = bodies[j];

        const dpos = b1.pos - b2.pos,
              mag = dt / sqrt(sumOfSquares(dpos))**3;

          b1.v -= dpos * b2.mass * mag; // TODO: make sure scalars mult'd first?
          b2.v += dpos * b1.mass * mag;
      }
    }

    for b in bodies do
      b.pos += dt * b.v;
  }



  proc energy() {
    var e = 0.0;

    // TODO: want to use triangular iterator here too, except that we need
    // code in between the two loops
    for i in 0..<numbodies {
      ref b1 = bodies[i];

      e += 0.5 * b1.mass * sumOfSquares(b1.v);

      for j in i+1..<numbodies {
        ref b2 = bodies[j];

        e -= (b1.mass * b2.mass) / sqrt(sumOfSquares(b1.pos - b2.pos));
      }
    }

    return e;
  }
  
}

config const n = 10000;

proc main(args: [] string) {

  var bodies: NBodySystem;

  writef("%.9r\n", bodies.energy());
  for 1..n do
    bodies.advance(0.01);
  writef("%.9r\n", bodies.energy());
}
