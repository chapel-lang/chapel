/*
  A 2D finite difference heat/diffusion equation solver

  Computation is local (single compute node) and runs
  the kernel in parallel using a `forall` loop

  Values of the `config const` variables can be modified in
  the command line (e.g., `./heat_2D --nt=100`)
*/

import Time.stopwatch;

// create a stopwatch to time kernel execution
var t = new stopwatch();
config const writeTime = false;

// declare configurable constants with default values
config const nx = 256,      // number of grid points in x
             ny = 256,      // number of grid points in y
             nt = 50,       // number of time steps
             alpha = 0.25,  // diffusion constant
             solutionStd = 0.222751; // known solution for the default parameters

// define a 2D domain and subdomain to describe the grid and its interior
const omega = {0..<nx, 0..<ny},
      omegaHat = omega.expand(-1);

// define a 2D array over the above domain
var u: [omega] real = 1.0;

// set up initial conditions
u[nx/4..nx/2, ny/4..ny/2] = 2.0;

// create a temporary copy of 'u' to store the previous time step
var un = u;

// iterate for 'nt' time steps
t.start();
for 1..nt {
  // swap arrays to prepare for next time step
  u <=> un;

  // compute the FD kernel in parallel
  forall (i, j) in omegaHat with (ref u) do
    u[i, j] = un[i, j] + alpha *
      (un[i-1, j] + un[i, j-1] + un[i+1, j] + un[i, j+1] - 4 * un[i, j]);
}
t.stop();

// print final results
const mean = (+ reduce u) / u.size,
      stdDev = sqrt((+ reduce (u - mean)**2) / u.size);

writeln(abs(solutionStd - stdDev) < 1e-6);
if writeTime then writeln("time: ", t.elapsed());
