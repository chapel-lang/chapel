
use StencilDist;

// set up simulation constants
config const xLen = 2.0,
             yLen = 2.0,
             nx = 31,
             ny = 31,
             sourceMag = 100.0,
             maxIters = 100;

const dx = xLen / (nx-1),
      dy = yLen / (ny-1);

// simulation settings
config const makePlots = false;

// define problem space
const space = stencilDist.createDomain({0..<ny, 0..<nx}, fluff=(1,1)),
      spaceInner = space.expand(-1);

proc main() {
    // define pressure and source arrays
    var p, b: [space] real = 0.0;

    // set boundary conditions
    p[..,    0] = 0;                     // p = 0 @ x = 0
    p[.., nx-1] = [j in 0..<ny] j * dy;  // p = y @ x = xLen
    p[0,    ..] = p[1, ..];              // dp/dy = 0 @ y = 0
    p[ny-1, ..] = p[ny-2, ..];           // dp/dy = 0 @ y = yLen

    // set sink/source terms
    b[3*ny/4, nx/4] = sourceMag;
    b[ny/4, 3*nx/4] = -sourceMag;

    solvePoisson(p, b);

    writeln("success!");
}

proc solvePoisson(ref p: [?d] real, const ref b: [d] real) {
    // temporary copy of p
    var pn = p;

    // solve for maxIters iterations
    for 1..maxIters {
        p <=> pn;
        pn.updateFluff();
        poissonKernel(p, pn, b);
        neumannBC(p);
    }
}

proc poissonKernel(ref p: [] real, const ref pn: [] real, const ref b: [] real) {
    forall (i, j) in spaceInner {
        p[i, j] = (
            dy**2 * (pn[i, j+1] + pn[i, j-1]) +
            dx**2 * (pn[i+1, j] + pn[i-1, j]) -
            dx**2 * dy**2 * b[i, j]
        ) / (2.0 * (dx**2 + dy**2));
    }
}

proc neumannBC(ref p: [] real) {
    p[0,    ..] = p[1, ..];    // dp/dy = 0 @ y = 0
    p[ny-1, ..] = p[ny-2, ..]; // dp/dy = 0 @ y = yLen
}
