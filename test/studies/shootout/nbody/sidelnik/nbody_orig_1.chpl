/* Initial version of the N-Body problem.
 * This is just straight-forward port of the
 * C-based implementation into Chapel.
 *
 * The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org
 *
 * contributed by Albert Sidelnik
 *
*/
config var n = 1000;
param PI = 3.141592653589793;
const solar_mass = (4 * PI * PI);
param days_per_year = 365.24;

class Planet {
  var x, y, z : real;
  var vx, vy, vz : real;
  var mass : real;
}

proc advance(nbodies:int, B: [] borrowed Planet, dt: real) {
  for (b1, i) in zip(B, 0..) {
    for j in i+1..nbodies-1 {
      const b2 = B(j);
      var dx = b1.x - b2.x;
      var dy = b1.y - b2.y;
      var dz = b1.z - b2.z;
      var distance = sqrt(dx * dx + dy * dy + dz * dz);
      var mag = dt / (distance * distance * distance);
      b1.vx -= dx * b2.mass * mag;
      b1.vy -= dy * b2.mass * mag;
      b1.vz -= dz * b2.mass * mag;
      b2.vx += dx * b1.mass * mag;
      b2.vy += dy * b1.mass * mag;
      b2.vz += dz * b1.mass * mag;
    }
  }
  
  for b in B {
    b.x += dt * b.vx;
    b.y += dt * b.vy;
    b.z += dt * b.vz;
  }
}

proc energy(nbodies:int, B : [] borrowed Planet) : real {
  var e : real;
  
  for (b1, i) in zip(B, 0..) {
    e += 0.5 * b1.mass * (b1.vx * b1.vx + b1.vy * b1.vy + b1.vz * b1.vz);
    
    for j in i+1..nbodies-1 {
      const b2 = B(j);
      var dx = b1.x - b2.x;
      var dy = b1.y - b2.y;
      var dz = b1.z - b2.z;
      var distance = sqrt(dx * dx + dy * dy + dz * dz);
      e -= (b1.mass * b2.mass) / distance;
    }
  }
  return e;
}

proc offset_momentum(nbodies:int, B : [] borrowed Planet) {
  var px,py,pz : real;
  for b in B {
    px += b.vx * b.mass;
    py += b.vy * b.mass;
    pz += b.vz * b.mass;
  }
  B(0).vx = -px / solar_mass;
  B(0).vy = -py / solar_mass;
  B(0).vz = -pz / solar_mass;
}

proc main() {
  param NBODIES = 5;
  var bodies : [0..#NBODIES] borrowed Planet =
    [new Planet(0, 0, 0, 0, 0, 0, solar_mass),
     new Planet(4.84143144246472090e+00,
                -1.16032004402742839e+00,
                -1.03622044471123109e-01,
                1.66007664274403694e-03 * days_per_year,
                7.69901118419740425e-03 * days_per_year,
                -6.90460016972063023e-05 * days_per_year,
                9.54791938424326609e-04 * solar_mass),
     new Planet(8.34336671824457987e+00,
                4.12479856412430479e+00,
                -4.03523417114321381e-01,
                -2.76742510726862411e-03 * days_per_year,
                4.99852801234917238e-03 * days_per_year,
                2.30417297573763929e-05 * days_per_year,
                2.85885980666130812e-04 * solar_mass),
     new Planet(1.28943695621391310e+01,
                -1.51111514016986312e+01,
                -2.23307578892655734e-01,
                2.96460137564761618e-03 * days_per_year,
                2.37847173959480950e-03 * days_per_year,
                -2.96589568540237556e-05 * days_per_year,
                4.36624404335156298e-05 * solar_mass),
     new Planet(1.53796971148509165e+01,
                -2.59193146099879641e+01,
                1.79258772950371181e-01,
                2.68067772490389322e-03 * days_per_year,
                1.62824170038242295e-03 * days_per_year,
                -9.51592254519715870e-05 * days_per_year,
                5.15138902046611451e-05 * solar_mass)
    ];

  offset_momentum(NBODIES, bodies);
  writef("%{#.#########}\n", energy(NBODIES, bodies));
  for 1..n {
    advance(NBODIES, bodies, 0.01);
  }
  writef("%{#.#########}\n", energy(NBODIES, bodies));
}
