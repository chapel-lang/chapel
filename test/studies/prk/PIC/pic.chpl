/*
   Chapel's parallel PIC implementation

   Contributed by Engin Kayraklioglu (GWU)

*/

require "random_draw.h", "random_draw.c";
use Time;

// TODO: Use Random module?

/* Initialize generator */
extern proc LCG_init();
/* Generate random number */
extern proc random_draw(x: c_double): uint(64);

enum Mode {GEOMETRIC, SINUSOIDAL, LINEAR, PATCH};

use Mode;

param PRKVERSION = "2.17";

param REL_X = 0.5,
      REL_Y = 0.5,
      DT = 1.0,
      Q = 1.0,
      MASS_INV = 1.0,
      epsilon = 0.000001;

             /* Grid size */
config const L = 10,
             /* Number of particles requested */
             n = 10,
             /* Particle charge semi-increment */
             k = 1,
             /* Vertical velocity */
             m = 1,
             /* Number of iterations */
             iterations = 10,
             /* How to initialize particle distribution */
             particleMode = SINUSOIDAL,
             /* Debug output */
             debug = false,
             /* Used for verifying correctness in testing */
             correctness = false;

/* Geometric initializiation */
config const rho = 0.99;

/* Linear initialization */
config const alpha = 1.0,
             beta = 3.0;

/* Patch initialization */
config const initPatchLeft = 1,     // Reference uses: 0
             initPatchRight = 2,    // Reference uses: 200
             initPatchTop= 1,       // Reference uses: 100
             initPatchBottom = 2;   // Reference uses: 200

// TODO: 1-based indexing
const gridDomOuter = {0..#(L+1), 0..#(L+1)},
      gridDomInner = {0..#L, 0..#L};

const Qgrid = initializeGrid(L);

var particleDom = {1..0};
var particles: [particleDom] Particle;

var t = new Timer();

proc main() {

  initialize();

  printInfo();

  for niter in 0..iterations {

    if niter == 1 then t.start();

    forall particle in particles {

      const (fx, fy) = computeTotalForce(particle);
      const (ax, ay) = (fx * MASS_INV, fy * MASS_INV);

      // For debug only
      var x0, y0:real;

      if debug then
        (x0,y0) = (particle.x, particle.y);

      particle.x = mod(particle.x + particle.v_x*DT + 0.5*ax*DT*DT + L, L);
      particle.y = mod(particle.y + particle.v_y*DT + 0.5*ay*DT*DT + L, L);

      if debug then
        writeln("Force acting on particle: ", (fx,fy),
                "\n\tParticle moved from ", (x0,y0), " to ",
                                      (particle.x, particle.y));

      particle.v_x += ax * DT;
      particle.v_y += ay * DT;
    }
  }
  t.stop();

  verifyResult();
}


record Particle {
  var x: real;
  var y: real;
  var v_x: real;
  var v_y: real;
  var q: real;

  var x0: real;
  var y0: real;

  var k: int;
  var m: int;

  proc Particle(x: real, y: real, k: int, m: int) {
    this.x = x;
    this.y = y;
    this.k = k;
    this.m = m;

    const rel_x = mod(this.x, 1.0),
          rel_y = mod(this.y, 1.0),
          r1_sq = rel_y * rel_y + rel_x * rel_x,
          r2_sq = rel_y * rel_y + (1.0-rel_x) * (1.0-rel_x),
          cos_theta = rel_x/sqrt(r1_sq),
          cos_phi = (1.0-rel_x)/sqrt(r2_sq),
          base_charge = 1.0 / ((DT*DT) * Q * (cos_theta/r1_sq + cos_phi/r2_sq));

    this.v_x = 0.0;
    this.v_y = this.m/DT;

    this.q = if ((this.x:uint)%2 == 0) then (2*this.k+1) * base_charge
             else -1.0 * (2*this.k+1) * base_charge ;
    this.x0 = this.x;
    this.y0 = this.y;
  }
}


proc initialize() {
  select particleMode {
    when GEOMETRIC do initializeGeometric();
    when SINUSOIDAL do initializeSinusoidal();
    when LINEAR do initializeLinear();
    when PATCH do initializePatch();
    otherwise do halt("Unknown particle mode: ", particleMode);
  }
}


proc printInfo() {
  if !correctness {
    writeln("Parallel Research Kernels Version ", PRKVERSION);
    writeln("Chapel Particle-in-Cell execution on 2D grid");
    writeln("Max parallelism                = ", here.maxTaskPar);
    writeln("Grid Size                      = ", L);
    writeln("Number of particles requested  = ", n);
    writeln("Number of time steps           = ", iterations);
    writeln("Initialization mode            = ", particleMode);
    writeParticleModeInfo();
    writeln("Particle charge semi-increment = ", k);
    writeln("Vertical velocity              = ", m);
  }

  if !correctness {
    writeln("Number of particles placed : ", particles.size);
  }

  /* Write info specific to particleMode chosen */
  proc writeParticleModeInfo() {
    select particleMode {
      when GEOMETRIC {
        writeln("\tAttenuation factor           = ", rho);
      }
      when LINEAR {
        writeln("\tNegative Slope               = ", alpha);
        writeln("\tOffset                       = ", beta);
      }
      when PATCH {
        writeln("\tBounding box                 = ",
               (initPatchLeft, initPatchRight, initPatchTop, initPatchBottom));
      }
    }

  }
}


proc verifyResult() {
  for particle in particles {
    if !verifyParticle(particle) then
      halt("VALIDATION FAILED!");
  }

  writeln("Validation successful");

  if !correctness {
    const avgTime = t.elapsed()/iterations;
    writeln("Rate (Mparticles_moved/s): ", 1.0e-6*(n/avgTime));
  }
}


/* Initialize grid to such that even columns are +Q, odd columns are -Q */
proc initializeGrid(L) {
  var grid: [gridDomOuter] real;

  grid = -Q;

  for column in gridDomOuter.dim(1) {
    // Set even columns to +Q
    if column % 2 == 0 then
      grid[.., column] = Q;
  }

  return grid;
}


proc initializeGeometric() {

  const A = n * ((1.0-rho) / (1.0-(rho**L))) / L;

  LCG_init();

  var nPlaced = 0:uint;
  for (x,y) in gridDomInner do
    nPlaced += random_draw(getSeed(x));

  particleDom = {0..#nPlaced};

  LCG_init();

  var pIdx = 0;
  for (x,y) in gridDomInner {
    const actual_particles = random_draw(getSeed(x)):int;
    placeParticles(particles[pIdx..#actual_particles], x, y);
    pIdx += actual_particles;
  }

  inline proc getSeed(x) {
    return A * (rho**x);
  }
}


proc initializeSinusoidal() {

  const step = pi/L;

  LCG_init();

  var nPlaced = 0:uint;

  for (x,y) in gridDomInner do
    nPlaced += random_draw(getSeed(x));

  particleDom = {0..#nPlaced};

  LCG_init();

  // pIdx = pi in OpenMP code
  var pIdx = 0;
  for (x,y) in gridDomInner {
    const actual_particles = random_draw(getSeed(x)):int;
    placeParticles(particles[pIdx..#actual_particles], x, y);
    pIdx += actual_particles;
    //placeParticles(pIdx, actual_particles, x, y);
  }

  inline proc getSeed(x) {
    return 2.0*(cos(x*step)**2)*n/(L**2);
  }
}


proc initializeLinear() {

  const step = 1.0/L;
  const total_weight = beta*L-alpha*0.5*step*L*(L-1);

  var nPlaced = 0:uint; // random_draw is a uint proc

  LCG_init();

  for (x,y) in gridDomInner do
    nPlaced += random_draw(getSeed(x));

  particleDom = {0..#nPlaced};

  LCG_init();

  var pIdx = 0;
  for (x,y) in gridDomInner {
    const actual_particles = random_draw(getSeed(x)):int;
    placeParticles(particles[pIdx..#actual_particles], x, y);
    pIdx += actual_particles;
  }

  inline proc getSeed(x) {
    return n * ((beta - alpha * step * x:real)/total_weight)/L;
  }
}


proc initializePatch() {

  record bbox {
    var left: int,
        right: int,
        bottom: int,
        top: int;
  }

  const patch = new bbox(initPatchLeft,
                         initPatchRight,
                         initPatchTop,
                         initPatchBottom);

  const gridPatch = new bbox(0, (L+1), 0, (L+1));

  if badPatch(patch, gridPatch) then
    halt("Bad patch given");

  const total_cells  = (patch.right - patch.left+1)*(patch.top -
      patch.bottom+1);
  const particles_per_cell = (n/total_cells):real;

  var nPlaced = 0:uint;
  LCG_init();

  for (x,y) in gridDomInner {
    const actual_particles = random_draw(particles_per_cell);
    if !outsidePatch(x, y) then
      nPlaced += actual_particles;
  }

  particleDom = {0..#nPlaced};

  LCG_init();

  var pIdx = 0;
  for (x,y) in gridDomInner {
    // TODO without cast this creates a seg fault and overflow
    // warning with no --fast. Investigate for possible bug. Engin
    const actual_particles = random_draw(particles_per_cell):int;
    if !outsidePatch(x, y) {
      placeParticles(particles[pIdx..#actual_particles], x, y);
      pIdx += actual_particles;
      //placeParticles(pIdx, actual_particles, x, y);
    }
  }

  inline proc outsidePatch(x, y) {
    return x < patch.left || x > patch.right ||
           y < patch.bottom || y > patch.top;
  }

  proc badPatch(patch, patch_contain) {
    if patch.left >= patch.right || patch.bottom >= patch.top then
      return true;
    if patch.left < patch_contain.left ||
       patch.right > patch_contain.right then
       return true;
    if patch.bottom < patch_contain.bottom ||
       patch.top > patch_contain.top then
       return true;

    return false;
  }
}

/* Initialize particles */
inline proc placeParticles(particles, x, y) {
  [particle in particles] particle = new Particle(x + REL_X, y + REL_Y, k, m);
}


proc finishDistribution() { }


inline proc computeCoulomb(x_dist, y_dist, q1, q2) {

  const r2 = x_dist**2 + y_dist**2,
        r = sqrt(r2),
        f_coulomb = q1*q2/r2;

  const fx = f_coulomb * x_dist/r,
        fy = f_coulomb * y_dist/r;

  return (fx, fy);
}


proc computeTotalForce(p) {

  const x = floor(p.x):int,
        y = floor(p.y):int;

  // TODO: tuple representation?
  const rel_x = p.x-x,
        rel_y = p.y-y;

  // TODO: More elegant? Less shuffling data around, sum of 4 things or iteration over 0..1, 0..1 w/ + reduce
  var (tmp_fx, tmp_fy) = computeCoulomb(    rel_x,    rel_y, p.q, Qgrid[y  ,x  ]);
  var res_xy = (tmp_fx, tmp_fy);

  (tmp_fx, tmp_fy) = computeCoulomb(    rel_x,1.0-rel_y, p.q, Qgrid[y+1,x  ]);
  res_xy  += (tmp_fx, -tmp_fy);

  (tmp_fx, tmp_fy) = computeCoulomb(1.0-rel_x,    rel_y, p.q, Qgrid[y  ,x+1]);
  res_xy += (-tmp_fx, tmp_fy);

  (tmp_fx, tmp_fy) = computeCoulomb(1.0-rel_x,1.0-rel_y, p.q, Qgrid[y+1,x+1]);
  res_xy += (-tmp_fx, -tmp_fy);

  return res_xy;
}


proc verifyParticle(p) {

  const y = p.y0:int,
        x = p.x0:int;

  const disp = (iterations+1):real*(2*p.k+1);
  const x_final = if (p.q * Qgrid[y,x])>0 then p.x0+disp else p.x0-disp;
  const y_final = p.y0 + p.m * (iterations+1):real;

  const x_periodic = mod(x_final+(iterations+1):real *(2*p.k+1)*L, L);
  const y_periodic = mod(y_final+(iterations+1):real *abs(p.m)*L, L);

  if ( abs(p.x - x_periodic) > epsilon || abs(p.y - y_periodic) > epsilon) {
    writeln("Validation for particle failed");
    writeln(p.x, " ", x_periodic);
    writeln(p.y, " ", y_periodic);
    return false;
  }
  return true;
}


