use StencilDist;
use Random;

config const nx, ny = 2048,
       nt = 100,
       alpha = 0.25;

config const writeFull = false;

const space = stencilDist.createDomain({0..<ny, 0..<nx}, fluff=(1,1)),
      spaceInner = space.expand(-1);

var u, un: [space] real = 1.0;

fillRandom(un, min=0.0, max=1.0, seed=17);

un.updateFluff();

forall (i, j) in spaceInner {
  u[i, j] =
    un[i, j] + alpha * (
      -4 * un[i, j] +
      un[i+1, j] +
      un[i-1, j] +
      un[i, j+1] +
      un[i, j-1]
      );
}

if writeFull then
  writeln(u);
else
  writeln(+ reduce u);
