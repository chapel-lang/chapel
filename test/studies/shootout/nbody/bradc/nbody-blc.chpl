/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   derived from the Chapel version by Albert Sidelnik and myself
*/

use Math;                     // to get access to 'pi'

config const n = 10000;       // The number of timesteps to simulate

param solarMass = 4 * pi * pi,
      daysPerYear = 365.24;


//
// a record for representing the position, velocity, and mass of
// bodies in the solar system
//
record body {
  var pos: 3*real;
  var vel: 3*real;
  var mass: real;
}

//
// the bodies that we'll be simulating
//
var bodies = (/* sun */
              new body(mass = solarMass),

              /* jupiter */
              new body(pos = ( 4.84143144246472090e+00,
                              -1.16032004402742839e+00,
                              -1.03622044471123109e-01),
                       vel = ( 1.66007664274403694e-03 * daysPerYear,
                               7.69901118419740425e-03 * daysPerYear,
                              -6.90460016972063023e-05 * daysPerYear),
                      mass =   9.54791938424326609e-04 * solarMass),

              /* saturn */
              new body(pos = ( 8.34336671824457987e+00,
                               4.12479856412430479e+00,
                              -4.03523417114321381e-01),
                       vel = (-2.76742510726862411e-03 * daysPerYear,
                               4.99852801234917238e-03 * daysPerYear,
                               2.30417297573763929e-05 * daysPerYear),
                      mass =   2.85885980666130812e-04 * solarMass),

              /* uranus */
              new body(pos = ( 1.28943695621391310e+01,
                              -1.51111514016986312e+01,
                              -2.23307578892655734e-01),
                       vel = ( 2.96460137564761618e-03 * daysPerYear,
                               2.37847173959480950e-03 * daysPerYear,
                              -2.96589568540237556e-05 * daysPerYear),
                      mass =   4.36624404335156298e-05 * solarMass),

              /* neptune */
              new body(pos = ( 1.53796971148509165e+01,
                              -2.59193146099879641e+01,
                               1.79258772950371181e-01),
                       vel = ( 2.68067772490389322e-03 * daysPerYear,
                               1.62824170038242295e-03 * daysPerYear,
                              -9.51592254519715870e-05 * daysPerYear),
                      mass =   5.15138902046611451e-05 * solarMass)
              );

param numBodies = bodies.size;

proc main() {
  initSun();                      // initialize the sun's velocity

  writef("%.9r\n", energy());     // print the initial energy

  for 1..n do                     // simulate 'n' timesteps
    advance(0.01);

  writef("%.9r\n", energy());     // print the final energy
}

//
// compute the sun's initial velocity
//
proc initSun() {
  const p = + reduce (for b in bodies do (b.vel * b.mass));
  bodies[0].vel = -p / solarMass;
}

//
// advance the positions and velocities of all the bodies
//
proc advance(dt) {
  for param i in 0..<numBodies {
    for param j in i+1..<numBodies {
      const dpos = bodies[i].pos - bodies[j].pos,
            mag = dt / sqrt(sumOfSquares(dpos))**3;

      bodies[i].vel -= dpos * bodies[j].mass * mag;
      bodies[j].vel += dpos * bodies[i].mass * mag;
    }
  }

  for param i in 0..<numBodies do
    bodies[i].pos += dt * bodies[i].vel;
}

//
// compute the energy of the bodies
//
proc energy() {
  var e = 0.0;

  for i in 0..<numBodies {
    e += 0.5 * bodies[i].mass * sumOfSquares(bodies[i].vel);
    for j in i+1..<numBodies {
      e -= (bodies[i].mass * bodies[j].mass)
           / sqrt(sumOfSquares(bodies[i].pos - bodies[j].pos));
    }
  }

  return e;
}

//
// compute the sum of squares of a 3-tuple's elements
//
inline proc sumOfSquares((x,y,z)) {
  return x**2 + y**2 + z**2;
}
