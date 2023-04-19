// setup simulation parameters
config const xLen : real = 2,
             yLen : real = 2,
             nx = 31,
             ny = 31,
             nt = 50,
             sigma = 0.25,
             nu = 0.05;

const dx : real = xLen / (nx - 1),
      dy : real = yLen / (ny - 1),
      dt : real = sigma * dx * dy / nu;

// define a 2D domain and subdomain
const dom = {0..<nx, 0..<ny},
      domInner : subdomain(dom) = dom.expand(-1);

// define an array to hold the solution
var u : [dom] real;

// setup initial conditions
u = 1;
u[
  (0.5 / dx):int..<(1.0 / dx + 1):int,
  (0.5 / dy):int..<(1.0 / dy + 1):int
] = 2;

// run finite difference computation
var un = u;
for n in 0..#nt {
  u <=> un;
  forall (i, j) in domInner {
    u[i, j] = un[i, j] +
              nu * dt / dy**2 *
                (un[i-1, j] - 2 * un[i, j] + un[i+1, j]) +
              nu * dt / dx**2 *
                (un[i, j-1] - 2 * un[i, j] + un[i, j+1]);
  }
}

// print the standard deviation of the solution
const mean = (+ reduce u) / u.size,
      stdDev = sqrt((+ reduce (u - mean)**2) / u.size);

writeln((0.102424 - stdDev) < 1e-6);
