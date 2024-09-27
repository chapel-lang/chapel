/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Damian McGuckin
   derived from the Chapel version by Brad Chamberlain
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
  var pos: 4*real;
  var vel: 4*real;
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
                              -1.03622044471123109e-01, 0.0),
                       vel = ( 1.66007664274403694e-03 * daysPerYear,
                               7.69901118419740425e-03 * daysPerYear,
                              -6.90460016972063023e-05 * daysPerYear, 0.0),
                      mass =   9.54791938424326609e-04 * solarMass),

              /* saturn */
              new body(pos = ( 8.34336671824457987e+00,
                               4.12479856412430479e+00,
                              -4.03523417114321381e-01, 0.0),
                       vel = (-2.76742510726862411e-03 * daysPerYear,
                               4.99852801234917238e-03 * daysPerYear,
                               2.30417297573763929e-05 * daysPerYear, 0.0),
                      mass =   2.85885980666130812e-04 * solarMass),

              /* uranus */
              new body(pos = ( 1.28943695621391310e+01,
                              -1.51111514016986312e+01,
                              -2.23307578892655734e-01, 0.0),
                       vel = ( 2.96460137564761618e-03 * daysPerYear,
                               2.37847173959480950e-03 * daysPerYear,
                              -2.96589568540237556e-05 * daysPerYear, 0.0),
                      mass =   4.36624404335156298e-05 * solarMass),

              /* neptune */
              new body(pos = ( 1.53796971148509165e+01,
                              -2.59193146099879641e+01,
                               1.79258772950371181e-01, 0.0),
                       vel = ( 2.68067772490389322e-03 * daysPerYear,
                               1.62824170038242295e-03 * daysPerYear,
                              -9.51592254519715870e-05 * daysPerYear, 0.0),
                      mass =   5.15138902046611451e-05 * solarMass)
              );

param numBodies = bodies.size;    // the number of bodies being simulated

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
  var p = bodies[0].vel; // initially all zeros

  for param i in 1..<numBodies {
    p -= bodies[i].vel * bodies[i].mass;
  }
  bodies[0].vel = p / solarMass;
}

//
// advance the positions and velocities of all the bodies
//
proc advance(dt) {
  param n = numBodies-1;

  for param i in 0..n-1 {
    const p = bodies[i].pos; // rip 'pos' out of loop
    const m = bodies[i].mass; // rip 'mass' out of loop
    var v = bodies[i].vel; // accumulate within loop

    for param j in i+1..n {
      const dp = p - bodies[j].pos;
      const dpsq = sumOfSquares(dp);
      const mag = dt / (dpsq * sqrt(dpsq));

      v -= dp * bodies[j].mass * mag;
      bodies[j].vel += dp * m * mag;
    }
	bodies[i].vel = v;
    bodies[i].pos += dt * v;
  }
  bodies[n].pos += dt * bodies[n].vel;
}

//
// compute the energy of the bodies
//
proc energy() {
  var e = 0.0;

  for param i in 0..<numBodies {
    const p = bodies[i].pos; // rip 'pos' out of loop
    var _e = 0.5 * sumOfSquares(bodies[i].vel);

    for param j in i+1..<numBodies {
      _e -= bodies[j].mass / sqrt(sumOfSquares(p - bodies[j].pos));
    }
    e = fma(_e, bodies[i].mass, e); // rip common multiplier out of loop
  }
  return e;
}

inline proc sumOfSquares(v)
{
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
