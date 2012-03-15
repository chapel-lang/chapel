/*

  Livermore Unstructured Lagrangian Explicit Shock Hydrodynamics (LULESH)
  https://computation.llnl.gov/casc/ShockHydro/

  Version 1.0.0

  Originally ported by Brandon Holt (8/2011)

  Notes on the Initial Implementation
  -----------------------------------
   
  This implementation is complete in that it produces nearly identical
  results to the C++ implementation. Verifying this was done by
  putting identical printouts of various arrays each time step and
  diff'ing the outputs. Some minor variations were observed (for
  extremely tiny values on the order of 10^-40, or in the last digit
  of a floating point value).  These variations are one-off and most
  often are gone the next iteration and do not appear to propagate or
  accumulate, so they are assumed to be variations in the order that
  operations are carried out.

  This implementation was designed to mirror the overall structure of
  the C++ Lulesh but use Chapel constructs where they can help make
  the code more readable, easier to maintain, or more
  'elegant'. Function names are preserved for the most part, with some
  additional helper functions, and original comments from the C++ code
  are interspersed approximately where they belong to give an idea of
  how the two codes line up. One major difference for this Chapel
  version is the use of a number of module-level variables and
  constants.

  Notes on Performance Improvements
  ---------------------------------
  [sungeun@cray.com 12/2011]

  - Replaced many consts with params because they are compile time
    constants.  In many cases the compiler should be able to detect
    this, but currently does not.

  - Replaced Chapel reductions over small tuples with explicit fused
    param loops (~6x performance improvement).  It's possible that
    using --dataParMinGranularity would have helped with performance
    here.

  - Similarly, replaced whole array operations with explicitly fused
    forall loops.

  - As much as possible, under-specified formal types (domain) of
    array formal arguments.  This avoids an expensive reindexing
    operation.

  - Replaced zippered foralls using elemToNodes() and a small static
    domain with a non-zippered forall using an iterator that returns a
    tuple.

  - Removed 'in' intent since it is currently more expensive than
    const.

  - Removed unnecessary locks (sync vars).

  - Inlined some functions.

 */

use Time;

/* Compile-time constants */

config param showProgress = false;
config param debug = false;
config param doTiming = true;
config param printArrays = false;

param XI_M        = 0x003;
param XI_M_SYMM   = 0x001;
param XI_M_FREE   = 0x002;

param XI_P        = 0x00c;
param XI_P_SYMM   = 0x004;
param XI_P_FREE   = 0x008;

param ETA_M       = 0x030;
param ETA_M_SYMM  = 0x010;
param ETA_M_FREE  = 0x020;

param ETA_P       = 0x0c0;
param ETA_P_SYMM  = 0x040;
param ETA_P_FREE  = 0x080;

param ZETA_M      = 0x300;
param ZETA_M_SYMM = 0x100;
param ZETA_M_FREE = 0x200;

param ZETA_P      = 0xc00;
param ZETA_P_SYMM = 0x400;
param ZETA_P_FREE = 0x800;

config param edgeElems = 45; 
param edgeNodes = edgeElems + 1;

/* Setup Problem Domain*/

//ranges
const Elems = 0..#edgeElems;
const Nodes = 0..#edgeNodes;

//domains
const ElemSpace = [Elems, Elems, Elems];
const NodeSpace = [Nodes, Nodes, Nodes];
const NodeFace  = [Nodes, Nodes];

//distributions
use BlockDist;
config param useBlockDist = false;

const ElemDist = if useBlockDist then ElemSpace dmapped Block(ElemSpace) else
                                        ElemSpace;
const NodeDist = if useBlockDist then NodeSpace dmapped Block(NodeSpace) else
                                        NodeSpace;
/* Types */
type Eid = index(ElemSpace);
type Nid = index(NodeSpace);

/* Constants */
const gammaCoef: [1..4, 1..8] real = 
		(( 1.0,  1.0, -1.0, -1.0, -1.0, -1.0,  1.0,  1.0),
		 ( 1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0, -1.0),
		 ( 1.0, -1.0,  1.0, -1.0,  1.0, -1.0,  1.0, -1.0),
		 (-1.0,  1.0, -1.0,  1.0,  1.0, -1.0,  1.0, -1.0));

const u_cut = 1.0e-7,       /* velocity tolerance */
  hgcoef = 3.0,             /* hourglass control */
  qstop = 1.0e+12,          /* excessive q indicator */
  monoq_max_slope = 1.0,
  monoq_limiter_mult = 2.0,
  e_cut = 1.0e-7,           /* energy tolerance */
  p_cut = 1.0e-7,           /* pressure tolerance */
  ss4o3 = 4.0/3.0,
  q_cut = 1.0e-7,           /* q tolerance */
  v_cut = 1.0e-10,          /* relative volume tolerance */
  qlc_monoq = 0.5,          /* linear term coef for q */
  qqc_monoq = 2.0/3.0,      /* quadratic term coef for q */
  qqc = 2.0, 
  eosvmax = 1.0e+9,
  eosvmin = 1.0e-9,
  pmin = 0.0,               /* pressure floor */
  emin = -1.0e+15,          /* energy floor */
  dvovmax = 0.1,            /* maximum allowable volume change */
  refdens = 1.0,            /* reference density */

  dtfixed = -1.0e-7,        /* fixed time increment */
  deltatimemultlb = 1.1,
  deltatimemultub = 1.2,
  dtmax = 1.0e-2;           /* maximum allowable time increment */

config const stoptime = 1.0e-2;        /* end time for simulation */

var matElemlist: [ElemDist] Eid,

  elemBC: [ElemDist] int,

  e: [ElemDist] real, //energy
  p: [ElemDist] real, //pressure

  q:  [ElemDist] real, //q
  ql: [ElemDist] real, //linear term for q
  qq: [ElemDist] real, //quadratic term for q

  v:     [ElemDist] real = 1.0, //relative volume
  vnew: [ElemDist] real,

  volo: [ElemDist] real, //reference volume
  delv: [ElemDist] real, //m_vnew - m_v
  vdov: [ElemDist] real, //volume derivative over volume

  arealg: [ElemDist] real, //elem characteristic length

  ss: [ElemDist] real, //"sound speed"

  elemMass: [ElemDist] real, //mass

  x: [NodeDist] real, //coordinates
  y: [NodeDist] real,
  z: [NodeDist] real,

  xd: [NodeDist] real, //velocities
  yd: [NodeDist] real,
  zd: [NodeDist] real,

  xdd: [NodeDist] real, //acceleration
  ydd: [NodeDist] real,
  zdd: [NodeDist] real,

  fx: [NodeDist] real, //forces
  fy: [NodeDist] real,
  fz: [NodeDist] real,

  nodalMass: [NodeDist] real, //mass

  // Parameters
  time = 0.0,          /* current time */
  deltatime = 1.0e-7,  /* variable time increment */
  dtcourant = 1.0e20,  /* courant constraint */
  dthydro = 1.0e20,    /* volume change constraint */

  cycle = 0;           /* iteration count for simulation */

	
proc main() {
  if debug then writeln("Lulesh -- Problem Size = ", edgeElems);

  LuleshData();
  if debug then testInit();

  //deposit energy for Sedov Problem
  e[ElemSpace.low] = 3.948746e+7;

  var st: real;
  if doTiming then st = getCurrentTime();
  while (time < stoptime) {
    TimeIncrement();

    LagrangeLeapFrog();

    if debug {
      deprint("[[ Forces ]]", fx, fy, fz);
      deprint("[[ Positions ]]", x, y, z);
      deprint("[[ p, e, q ]]", p, e, q);
    }
    if showProgress then writeln("time = ", format("%e", time), 
                                 ", dt=", format("%e", deltatime));
  }
  if doTiming {
    const et = getCurrentTime();
    writeln("Total Time: ", et-st);
  }
  if printArrays {
    for i in NodeDist {
      if debug {
        writeln(format("%3d",NodeDist.indexOrder(i)), ": ", 
                format("%1.9e", x[i]), " ", 
                format("%1.9e", y[i]), " ", 
                format("%1.9e", z[i]));
      } else {
        writeln(format("%3d",NodeDist.indexOrder(i)), ": ", 
                format("%1.4e", x[i]), " ", 
                format("%1.4e", y[i]), " ", 
                format("%1.4e", z[i]));
      }
    }
  }
}

// Initialization functions
proc LuleshData() {
  /* initialize nodal coordinates */
  initializeNodalCoordinates();

  /* embed hexehedral elements in nodal point lattice */
  //calculated on the fly using: elemToNodes(i: Eid): Nid

  /* Create a material IndexSet (entire domain same material for now) */
  forall (mat, i) in (matElemlist, matElemlist.domain) do mat = i;

  /* initialize field data */
  initializeFieldData();

  /* set up symmetry nodesets */
  //done on the fly with iterators

  /* set up elemement connectivity information */
  //calculated on the fly using functions with the same name as the arrays

  /* set up boundary condition information */
  setupBoundaryConditions();
}

proc initializeNodalCoordinates() {
  forall i in NodeDist do local {
      const initialWidth = 1.125;
      x[i] = initialWidth * i[3]:real / edgeElems:real;
      y[i] = initialWidth * i[2]:real / edgeElems:real;
      z[i] = initialWidth * i[1]:real / edgeElems:real;
    }
}

proc initializeFieldData() {
  forall eli in ElemDist {
    var x_local, y_local, z_local: 8*real;
    localizeNeighborNodes(eli, x, x_local, y, y_local, z, z_local);

    var volume = CalcElemVolume(x_local, y_local, z_local);
    volo[eli] = volume;
    elemMass[eli] = volume;
  }
  //now calculate node masses as average of element masses
  // (need to do this in NodeDist loop to make sure each is accumulated safely)
  forall (noi, mass) in (NodeDist, nodalMass) {
    mass = 0.0;
    for eli in nodeToElems(noi) do mass += elemMass[eli];
    mass /= 8.0;
  }
}

proc setupBoundaryConditions() {
  elemBC[..,..,Elems.low ] |= XI_M_SYMM;
  elemBC[..,..,Elems.high] |= XI_P_FREE;
  elemBC[..,Elems.low ,..] |= ETA_M_SYMM;
  elemBC[..,Elems.high,..] |= ETA_P_FREE;
  elemBC[Elems.low ,..,..] |= ZETA_M_SYMM;
  elemBC[Elems.high,..,..] |= ZETA_P_FREE;
}

// Helper functions
inline proc
localizeNeighborNodes(eli: Eid,
                      x: [] real, inout x_local: 8*real,
                      y: [] real, inout y_local: 8*real,
                      z: [] real, inout z_local: 8*real) {
  for (noi, t) in elemToNodesTuple(eli) {
    x_local[t] = x[noi];
    y_local[t] = y[noi];
    z_local[t] = z[noi];
  }
}

inline
proc TripleProduct(x1, y1, z1, x2, y2, z2, x3, y3, z3) {
  return x1*(y2*z3 - z2*y3) + x2*(z1*y3 - y1*z3) + x3*(y1*z2 - z1*y2);
}

proc CalcElemVolume(x, y, z) {
  var dx61 = x[7] - x[2],
    dy61 = y[7] - y[2],
    dz61 = z[7] - z[2],

    dx70 = x[8] - x[1],
    dy70 = y[8] - y[1],
    dz70 = z[8] - z[1],

    dx63 = x[7] - x[4],
    dy63 = y[7] - y[4],
    dz63 = z[7] - z[4],

    dx20 = x[3] - x[1],
    dy20 = y[3] - y[1],
    dz20 = z[3] - z[1],

    dx50 = x[6] - x[1],
    dy50 = y[6] - y[1],
    dz50 = z[6] - z[1],

    dx64 = x[7] - x[5],
    dy64 = y[7] - y[5],
    dz64 = z[7] - z[5],

    dx31 = x[4] - x[2],
    dy31 = y[4] - y[2],
    dz31 = z[4] - z[2],

    dx72 = x[8] - x[3],
    dy72 = y[8] - y[3],
    dz72 = z[8] - z[3],

    dx43 = x[5] - x[4],
    dy43 = y[5] - y[4],
    dz43 = z[5] - z[4],

    dx57 = x[6] - x[8],
    dy57 = y[6] - y[8],
    dz57 = z[6] - z[8],

    dx14 = x[2] - x[5],
    dy14 = y[2] - y[5],
    dz14 = z[2] - z[5],

    dx25 = x[3] - x[6],
    dy25 = y[3] - y[6],
    dz25 = z[3] - z[6];

  var volume = 
    TripleProduct(dx31 + dx72, dx63, dx20,
                  dy31 + dy72, dy63, dy20,
                  dz31 + dz72, dz63, dz20) +
    TripleProduct(dx43 + dx57, dx64, dx70,
                  dy43 + dy57, dy64, dy70,
                  dz43 + dz57, dz64, dz70) +
    TripleProduct(dx14 + dx25, dx61, dx50,
                  dy14 + dy25, dy61, dy50,
                  dz14 + dz25, dz61, dz50);

  return volume / 12.0;
}

proc InitStressTermsForElems(p, q, sigxx, sigyy, sigzz: [?D] real) {
  forall i in D {
    sigxx[i] = -p[i] - q[i];
    sigyy[i] = -p[i] - q[i];
    sigzz[i] = -p[i] - q[i];
  }
}


proc CalcElemShapeFunctionDerivatives(x: 8*real, y: 8*real, z: 8*real, 
                                      out b_x: 8*real,
                                      out b_y: 8*real,
                                      out b_z: 8*real, 
                                      out volume: real) {

  var fjxxi = .125 * ( (x[7]-x[1]) + (x[6]-x[4]) - (x[8]-x[2]) - (x[5]-x[3]) ),
    fjxet = .125 * ( (x[7]-x[1]) - (x[6]-x[4]) + (x[8]-x[2]) - (x[5]-x[3]) ),
    fjxze = .125 * ( (x[7]-x[1]) + (x[6]-x[4]) + (x[8]-x[2]) + (x[5]-x[3]) ),

    fjyxi = .125 * ( (y[7]-y[1]) + (y[6]-y[4]) - (y[8]-y[2]) - (y[5]-y[3]) ),
    fjyet = .125 * ( (y[7]-y[1]) - (y[6]-y[4]) + (y[8]-y[2]) - (y[5]-y[3]) ),
    fjyze = .125 * ( (y[7]-y[1]) + (y[6]-y[4]) + (y[8]-y[2]) + (y[5]-y[3]) ),

    fjzxi = .125 * ( (z[7]-z[1]) + (z[6]-z[4]) - (z[8]-z[2]) - (z[5]-z[3]) ),
    fjzet = .125 * ( (z[7]-z[1]) - (z[6]-z[4]) + (z[8]-z[2]) - (z[5]-z[3]) ),
    fjzze = .125 * ( (z[7]-z[1]) + (z[6]-z[4]) + (z[8]-z[2]) + (z[5]-z[3]) );

  /* compute cofactors */
  var cjxxi =    (fjyet * fjzze) - (fjzet * fjyze),
    cjxet =  - (fjyxi * fjzze) + (fjzxi * fjyze),
    cjxze =    (fjyxi * fjzet) - (fjzxi * fjyet),

    cjyxi =  - (fjxet * fjzze) + (fjzet * fjxze),
    cjyet =    (fjxxi * fjzze) - (fjzxi * fjxze),
    cjyze =  - (fjxxi * fjzet) + (fjzxi * fjxet),

    cjzxi =    (fjxet * fjyze) - (fjyet * fjxze),
    cjzet =  - (fjxxi * fjyze) + (fjyxi * fjxze),
    cjzze =    (fjxxi * fjyet) - (fjyxi * fjxet);

  /* calculate partials :
     this need only be done for l = 0,1,2,3   since , by symmetry ,
     (6,7,4,5) = - (0,1,2,3) .
  */
  b_x[1] =   -  cjxxi  -  cjxet  -  cjxze;
  b_x[2] =      cjxxi  -  cjxet  -  cjxze;
  b_x[3] =      cjxxi  +  cjxet  -  cjxze;
  b_x[4] =   -  cjxxi  +  cjxet  -  cjxze;
  b_x[5] = -b_x[3];
  b_x[6] = -b_x[4];
  b_x[7] = -b_x[1];
  b_x[8] = -b_x[2];

  b_y[1] =   -  cjyxi  -  cjyet  -  cjyze;
  b_y[2] =      cjyxi  -  cjyet  -  cjyze;
  b_y[3] =      cjyxi  +  cjyet  -  cjyze;
  b_y[4] =   -  cjyxi  +  cjyet  -  cjyze;
  b_y[5] = -b_y[3];
  b_y[6] = -b_y[4];
  b_y[7] = -b_y[1];
  b_y[8] = -b_y[2];

  b_z[1] =   -  cjzxi  -  cjzet  -  cjzze;
  b_z[2] =      cjzxi  -  cjzet  -  cjzze;
  b_z[3] =      cjzxi  +  cjzet  -  cjzze;
  b_z[4] =   -  cjzxi  +  cjzet  -  cjzze;
  b_z[5] = -b_z[3];
  b_z[6] = -b_z[4];
  b_z[7] = -b_z[1];
  b_z[8] = -b_z[2];

  /* calculate jacobian determinant (volume) */
  volume = 8.0 * ( fjxet * cjxet + fjyet * cjyet + fjzet * cjzet);
}

proc CalcElemNodeNormals(out pfx: 8*real, out pfy: 8*real, out pfz: 8*real, 
                         x: 8*real, y: 8*real, z: 8*real) {

  proc ElemFaceNormal(param n1, param n2, param n3, param n4) {
    var bisectX0 = 0.5 * (x[n4] + x[n3] - x[n2] - x[n1]),
      bisectY0 = 0.5 * (y[n4] + y[n3] - y[n2] - y[n1]),
      bisectZ0 = 0.5 * (z[n4] + z[n3] - z[n2] - z[n1]),
      bisectX1 = 0.5 * (x[n3] + x[n2] - x[n4] - x[n1]),
      bisectY1 = 0.5 * (y[n3] + y[n2] - y[n4] - y[n1]),
      bisectZ1 = 0.5 * (z[n3] + z[n2] - z[n4] - z[n1]),
      areaX    = 0.25 * (bisectY0 * bisectZ1 - bisectZ0 * bisectY1),
      areaY    = 0.25 * (bisectZ0 * bisectX1 - bisectX0 * bisectZ1),
      areaZ    = 0.25 * (bisectX0 * bisectY1 - bisectY0 * bisectX1);

    var rx, ry, rz: 8*real; //results
    (rx[n1], rx[n2], rx[n3], rx[n4]) = (areaX, areaX, areaX, areaX);
    (ry[n1], ry[n2], ry[n3], ry[n4]) = (areaY, areaY, areaY, areaY);
    (rz[n1], rz[n2], rz[n3], rz[n4]) = (areaZ, areaZ, areaZ, areaZ);
    return (rx, ry, rz);
  }

  //calculate total normal from each face (faces are made up of combinations of nodes)
  (pfx, pfy, pfz) = ElemFaceNormal(1,2,3,4) + ElemFaceNormal(1,5,6,2) +
    ElemFaceNormal(2,6,7,3) + ElemFaceNormal(3,7,8,4) +
    ElemFaceNormal(4,8,5,1) + ElemFaceNormal(5,8,7,6);
}

proc SumElemStressesToNodeForces(b_x: 8*real, b_y: 8*real, b_z: 8*real, 
                                 stress_xx:real,
                                 stress_yy:real,
                                 stress_zz: real, 
                                 out fx: 8*real,
                                 out fy: 8*real,
                                 out fz: 8*real) {
  for param i in 1..8 {
    fx[i] = -(stress_xx * b_x[i]);
    fy[i] = -(stress_yy * b_y[i]);
    fz[i] = -(stress_zz * b_z[i]);
  }
}

proc CalcElemVolumeDerivative(x: 8*real, y: 8*real, z: 8*real) {

  proc VoluDer(param n0, param n1, param n2, param n3, param n4, param n5) {
    var ox, oy, oz: real;
    ox =
      (y[n1] + y[n2]) * (z[n0] + z[n1]) - (y[n0] + y[n1]) * (z[n1] + z[n2]) +
      (y[n0] + y[n4]) * (z[n3] + z[n4]) - (y[n3] + y[n4]) * (z[n0] + z[n4]) -
      (y[n2] + y[n5]) * (z[n3] + z[n5]) + (y[n3] + y[n5]) * (z[n2] + z[n5]);
    oy =
      - (x[n1] + x[n2]) * (z[n0] + z[n1]) + (x[n0] + x[n1]) * (z[n1] + z[n2]) -
      (x[n0] + x[n4]) * (z[n3] + z[n4]) + (x[n3] + x[n4]) * (z[n0] + z[n4]) +
      (x[n2] + x[n5]) * (z[n3] + z[n5]) - (x[n3] + x[n5]) * (z[n2] + z[n5]);
    oz =
      - (y[n1] + y[n2]) * (x[n0] + x[n1]) + (y[n0] + y[n1]) * (x[n1] + x[n2]) -
      (y[n0] + y[n4]) * (x[n3] + x[n4]) + (y[n3] + y[n4]) * (x[n0] + x[n4]) +
      (y[n2] + y[n5]) * (x[n3] + x[n5]) - (y[n3] + y[n5]) * (x[n2] + x[n5]);
    return (ox/12.0, oy/12.0, oz/12.0);
  }

  var dvdx, dvdy, dvdz: 8*real;
  (dvdx[1], dvdy[1], dvdz[1]) = VoluDer(2,3,4,5,6,8);
  (dvdx[4], dvdy[4], dvdz[4]) = VoluDer(1,2,3,8,5,7);
  (dvdx[3], dvdy[3], dvdz[3]) = VoluDer(4,1,2,7,8,6);
  (dvdx[2], dvdy[2], dvdz[2]) = VoluDer(3,4,1,6,7,5);
  (dvdx[5], dvdy[5], dvdz[5]) = VoluDer(8,7,6,1,4,2);
  (dvdx[6], dvdy[6], dvdz[6]) = VoluDer(5,8,7,2,1,3);
  (dvdx[7], dvdy[7], dvdz[7]) = VoluDer(6,5,8,3,2,4);
  (dvdx[8], dvdy[8], dvdz[8]) = VoluDer(7,6,5,4,3,1);
  return (dvdx, dvdy, dvdz);
}

inline proc
CalcElemFBHourglassForce(xd: 8*real, yd: 8*real, zd: 8*real,
                         hourgam: 8*(4*real),
                         coefficient: real,
                         out hgfx: 8*real,
                         out hgfy: 8*real,
                         out hgfz: 8*real) {
  var hx, hy, hz: 4*real;
  // reduction
  for param i in 1..4 {
    for param j in 1..8 {
      hx[i] += hourgam(j)(i) * xd[j];
      hy[i] += hourgam(j)(i) * yd[j];
      hz[i] += hourgam(j)(i) * zd[j];
    }
  }
  for param i in 1..8 {
    var shx, shy, shz: real;
    for param j in 1..4 {
      shx += hourgam(i)(j) * hx[j];
      shy += hourgam(i)(j) * hy[j];
      shz += hourgam(i)(j) * hz[j];
    }
    hgfx[i] = coefficient * shx;
    hgfy[i] = coefficient * shy;
    hgfz[i] = coefficient * shz;
  }
}

proc CalcElemCharacteristicLength(x, y, z, volume) {
  proc AreaFace(param p0, param p1, param p2, param p3) {
    var fx = (x[p2] - x[p0]) - (x[p3] - x[p1]),
      fy = (y[p2] - y[p0]) - (y[p3] - y[p1]),
      fz = (z[p2] - z[p0]) - (z[p3] - z[p1]),
      gx = (x[p2] - x[p0]) + (x[p3] - x[p1]),
      gy = (y[p2] - y[p0]) + (y[p3] - y[p1]),
      gz = (z[p2] - z[p0]) + (z[p3] - z[p1]),
      area =
      (fx * fx + fy * fy + fz * fz) *
      (gx * gx + gy * gy + gz * gz) -
      (fx * gx + fy * gy + fz * gz) *
      (fx * gx + fy * gy + fz * gz);
    return area ;
  }
  var charLength = max(AreaFace(1, 2, 3, 4),
                       AreaFace(5, 6, 7, 8),
                       AreaFace(1, 2, 6, 5),
                       AreaFace(2, 3, 7, 6),
                       AreaFace(3, 4, 8, 7),
                       AreaFace(4, 1, 5, 8));

  return 4.0 * volume / sqrt(charLength);
}

proc CalcElemVelocityGradient(xvel, yvel, zvel, pfx,  pfy, pfz,
                              detJ, out d: 6*real) {
  const inv_detJ = 1.0 / detJ;
	
  d[1] = inv_detJ * ( pfx[1] * (xvel[1]-xvel[7])
                      + pfx[2] * (xvel[2]-xvel[8])
                      + pfx[3] * (xvel[3]-xvel[5])
                      + pfx[4] * (xvel[4]-xvel[6]) );
  d[2] = inv_detJ * ( pfy[1] * (yvel[1]-yvel[7])
                      + pfy[2] * (yvel[2]-yvel[8])
                      + pfy[3] * (yvel[3]-yvel[5])
                      + pfy[4] * (yvel[4]-yvel[6]) );
  d[3] = inv_detJ * ( pfz[1] * (zvel[1]-zvel[7])
                      + pfz[2] * (zvel[2]-zvel[8])
                      + pfz[3] * (zvel[3]-zvel[5])
                      + pfz[4] * (zvel[4]-zvel[6]) );

  var dyddx  = inv_detJ * ( pfx[1] * (yvel[1]-yvel[7])
                            + pfx[2] * (yvel[2]-yvel[8])
                            + pfx[3] * (yvel[3]-yvel[5])
                            + pfx[4] * (yvel[4]-yvel[6]) ),

    dxddy  = inv_detJ * ( pfy[1] * (xvel[1]-xvel[7])
                          + pfy[2] * (xvel[2]-xvel[8])
                          + pfy[3] * (xvel[3]-xvel[5])
                          + pfy[4] * (xvel[4]-xvel[6]) ),

    dzddx  = inv_detJ * ( pfx[1] * (zvel[1]-zvel[7])
                          + pfx[2] * (zvel[2]-zvel[8])
                          + pfx[3] * (zvel[3]-zvel[5])
                          + pfx[4] * (zvel[4]-zvel[6]) ),

    dxddz  = inv_detJ * ( pfz[1] * (xvel[1]-xvel[7])
                          + pfz[2] * (xvel[2]-xvel[8])
                          + pfz[3] * (xvel[3]-xvel[5])
                          + pfz[4] * (xvel[4]-xvel[6]) ),

    dzddy  = inv_detJ * ( pfy[1] * (zvel[1]-zvel[7])
                          + pfy[2] * (zvel[2]-zvel[8])
                          + pfy[3] * (zvel[3]-zvel[5])
                          + pfy[4] * (zvel[4]-zvel[6]) ),

    dyddz  = inv_detJ * ( pfz[1] * (yvel[1]-yvel[7])
                          + pfz[2] * (yvel[2]-yvel[8])
                          + pfz[3] * (yvel[3]-yvel[5])
                          + pfz[4] * (yvel[4]-yvel[6]) );
  d[6]  = 0.5 * ( dxddy + dyddx );
  d[5]  = 0.5 * ( dxddz + dzddx );
  d[4]  = 0.5 * ( dzddy + dyddz );
}

proc CalcPressureForElems(p_new: [?D] real, bvc, pbvc, 
                          e_old, compression, vnewc,
                          pmin: real, p_cut: real, eosvmax: real) {

  forall i in D do local {
      const c1s = 2.0 / 3.0;
      bvc[i] = c1s * (compression[i] + 1.0);
      pbvc[i] = c1s;
  }

  forall i in D {
    p_new[i] = bvc[i] * e_old[i];

    if abs(p_new[i]) < p_cut then p_new[i] = 0.0;
    if vnewc[i] >= eosvmax then p_new[i] = 0.0; //impossible?
    if p_new[i] < pmin then p_new[i] = pmin;
  }
}

iter symmX() { for (i,j) in NodeFace do yield (i, j, 0); }
iter symmY() { for (i,j) in NodeFace do yield (i, 0, j); }
iter symmZ() { for (i,j) in NodeFace do yield (0, i, j); }

inline proc lxim((x,y,z): Eid): Eid {
  var i = x*edgeElems*edgeElems + y*edgeElems + z - 1;
  if i < 0 then return ElemSpace.low;
  return (i/(edgeElems*edgeElems), i%(edgeElems*edgeElems)/edgeElems, i % edgeElems);
}
inline proc lxip((x,y,z): Eid): Eid {
  var i = x*edgeElems*edgeElems + y*edgeElems + z + 1;
  if i == edgeElems**3 then return ElemSpace.high;
  return (i/(edgeElems*edgeElems), i%(edgeElems*edgeElems)/edgeElems, i % edgeElems);
}
inline proc letam((x,y,z): Eid): Eid {
  if x == 0 && y == 0 then return (x,             y, z);
  else if y == 0 then return      (x-1, edgeElems-1, z);
  else return                     (x,           y-1, z);
}
inline proc letap((x,y,z): Eid): Eid {
  if x == edgeElems-1 && y == edgeElems-1 then return (x,     y, z);
  else if y == edgeElems-1 then return                (x+1,   0, z);
  else return                                         (x,   y+1, z);
}
inline proc lzetam((x,y,z): Eid): Eid {
  if x == 0 then return (x,   y, z);
  else return           (x-1, y, z);
}
inline proc lzetap((x,y,z): Eid): Eid {
  if x == edgeElems-1 then return (x,   y, z);
  else return                     (x+1, y, z);
}


proc TimeIncrement() {
  var targetdt = stoptime - time;

  if dtfixed <= 0.0 && cycle != 0 { //don't do this the first cycle
    var olddt = deltatime,
      newdt = 1.0e20;

    if dtcourant < newdt then newdt = dtcourant / 2.0;
    if dthydro < newdt then   newdt = 2.0/3.0 * dthydro;

    var ratio = newdt / olddt;
    if ratio >= 1.0 {
      if ratio < deltatimemultlb then      newdt = olddt;
      else if ratio > deltatimemultub then newdt = olddt * deltatimemultub;
    }
    if newdt > dtmax then newdt = dtmax;

    deltatime = newdt;
  }
  /* TRY TO PREVENT VERY SMALL SCALING ON THE NEXT CYCLE */
  if targetdt > deltatime && targetdt < (4.0/3.0 * deltatime) {
    targetdt = 2.0/3.0 * deltatime;
  }
  if targetdt < deltatime then deltatime = targetdt;

  time += deltatime;
  cycle += 1;
}

inline proc LagrangeLeapFrog() {
  /* calculate nodal forces, accelerations, velocities, positions, with
   * applied boundary conditions and slide surface considerations */
  LagrangeNodal();

  /* calculate element quantities (i.e. velocity gradient & q), and update
   * material states */
  LagrangeElements();

  CalcTimeConstraintsForElems();
}

inline proc LagrangeNodal() {
  CalcForceForNodes();

  CalcAccelerationForNodes();

  ApplyAccelerationBoundaryConditionsForNodes();

  CalcVelocityForNodes(deltatime, u_cut);

  CalcPositionForNodes(deltatime);

}

inline proc LagrangeElements() {
  CalcLagrangeElements();

  /* Calculate Q.  (Monotonic q option requires communication) */
  CalcQForElems();

  ApplyMaterialPropertiesForElems();

  UpdateVolumesForElems();
}

inline proc CalcTimeConstraintsForElems() {
  /* evaluate time constraint */
  CalcCourantConstraintForElems();

  /* check hydro constraint */
  CalcHydroConstraintForElems();
}

proc CalcCourantConstraintForElems() {
  var dtcourant_tmp = 1.0e+20;
  var courant_elem: Eid;
  var qqc2 = 64.0 * qqc**2;
  var active = false;

  forall (i,indx) in (ElemDist,matElemlist) {
    var dtf = ss[indx]**2;
    if vdov[indx] < 0.0 {
      dtf += qqc2 * arealg[indx]**2 * vdov[indx]**2;
    }
    dtf = sqrt(dtf);
    dtf = arealg[indx] / dtf;

    /* determine minimum timestep with its corresponding elem */
    if vdov[indx] != 0.0 && dtf < dtcourant_tmp {
      dtcourant_tmp = dtf;
      courant_elem = indx;
      active = true;
    }
  }
  /* Don't try to register a time constraint if none of the elements were active */
  if active then dtcourant = dtcourant_tmp;
}

proc CalcHydroConstraintForElems() {
  var dthydro_tmp = 1.0e+20;
  var dthydro_elem: Eid;
  var active = false;

  forall (i,indx) in (ElemDist,matElemlist) {
    if vdov[indx] != 0.0 {
      var dtdvov = dvovmax / (abs(vdov[indx])+1.0e-20);
      if dthydro_tmp > dtdvov {
        dthydro_tmp = dtdvov;
        dthydro_elem = indx;
        active = true;
      }
    }
  }
  if active then dthydro = dthydro_tmp;
}

proc CalcForceForNodes() {
  /* calculate nodal forces, accelerations, velocities, positions, with
   * applied boundary conditions and slide surface considerations */
  //zero out all forces (array assignment)
  fx = 0;
  fy = 0;
  fz = 0;

  /* Calcforce calls partial, force, hourq */
  CalcVolumeForceForElems();

  /* Calculate Nodal Forces at domain boundaries */
  // this was commented out in C++ code, so left out here
}

proc CalcVolumeForceForElems() {
  var sigxx, sigyy, sigzz, determ: [ElemDist] real;

  /* Sum contributions to total stress tensor */
  InitStressTermsForElems(p, q, sigxx, sigyy, sigzz);

  /* call elemlib stress integration loop to produce nodal forces from material stresses. */
  IntegrateStressForElems(sigxx, sigyy, sigzz, determ);

  //check for negative element volume
  if ( || reduce (determ <= 0.0) ) == true {
    writeln("determ:\n", determ);
    writeln("Error: can't have negative volume."); exit(1);
  }

  CalcHourglassControlForElems(determ);
}

proc IntegrateStressForElems(sigxx, sigyy, sigzz, determ) {
  forall k in ElemDist {
    var b_x, b_y, b_z: 8*real;
    var x_local, y_local, z_local: 8*real;
    localizeNeighborNodes(k, x, x_local, y, y_local, z, z_local);

    var fx_local, fy_local, fz_local: 8*real;

    local {
      /* Volume calculation involves extra work for numerical consistency. */
      CalcElemShapeFunctionDerivatives(x_local, y_local, z_local, 
                                       b_x, b_y, b_z, determ[k]);
    
      CalcElemNodeNormals(b_x, b_y, b_z, x_local, y_local, z_local);

      SumElemStressesToNodeForces(b_x, b_y, b_z, sigxx[k], sigyy[k], sigzz[k], 
                                  fx_local, fy_local, fz_local);
    }
		
    for (noi, t) in elemToNodesTuple(k) {
      fx[noi] += fx_local[t];
      fy[noi] += fy_local[t];
      fz[noi] += fz_local[t];
    }
  }
}

proc CalcHourglassControlForElems(determ: [ElemDist] real) {
  var dvdx, dvdy, dvdz, x8n, y8n, z8n: [ElemDist] 8*real;

  forall eli in ElemDist {
    //Collect domain nodes to elem nodes
    var x1, y1, z1: 8*real;
    localizeNeighborNodes(eli, x, x1, y, y1, z, z1);
    var pfx, pfy, pfz: 8*real;

    local {
      /* load into temporary storage for FB Hour Glass control */
      (dvdx[eli], dvdy[eli], dvdz[eli]) = CalcElemVolumeDerivative(x1, y1, z1);
    }

    x8n[eli]  = x1;
    y8n[eli]  = y1;
    z8n[eli]  = z1;

    determ[eli] = volo[eli] * v[eli];
  }
	
  /* Do a check for negative volumes */
  if ( || reduce (v <= 0.0) ) == true {
    writeln("v:\n", v);
    writeln("Error: can't have negative (or zero) volume. (in Hourglass)");
    exit(1);
  }

  if hgcoef > 0.0 {
    CalcFBHourglassForceForElems(determ, x8n, y8n, z8n, dvdx, dvdy, dvdz);
  }
}

proc CalcFBHourglassForceForElems(determ,
                                  x8n, y8n, z8n, dvdx, dvdy, dvdz) {
  /* Calculates the Flanagan-Belytschko anti-hourglass force. */

  /* compute the hourglass modes */
  forall eli in ElemDist {
    var hourgam: 8*(4*real);
    var volinv = 1.0 / determ[eli];
    var ss1, mass1, volume13: real;
    var hgfx, hgfy, hgfz: 8*real;
    var coefficient: real;

    var xd1, yd1, zd1: 8*real;
    localizeNeighborNodes(eli, xd, xd1, yd, yd1, zd, zd1);

    /* TODO [sungeun]: Brandon seemed to think that this should all be local */
    // local {
      for param i in 1..4 {
        var hourmodx, hourmody, hourmodz: real;
        // reduction
        for param j in 1..8 {
          hourmodx += x8n[eli][j] * gammaCoef[i,j];
          hourmody += y8n[eli][j] * gammaCoef[i,j];
          hourmodz += z8n[eli][j] * gammaCoef[i,j];
        }

        for param j in 1..8 {
          hourgam(j)(i) = gammaCoef[i,j] - volinv * 
            (dvdx[eli][j] * hourmodx +
             dvdy[eli][j] * hourmody +
             dvdz[eli][j] * hourmodz);
        }
      }

      /* compute forces */
      /* store forces into h arrays (force arrays) */
      ss1 = ss[eli];
      mass1 = elemMass[eli];
      volume13 = cbrt(determ[eli]);

      coefficient = - hgcoef * 0.01 * ss1 * mass1 / volume13;

      CalcElemFBHourglassForce(xd1, yd1, zd1, hourgam, coefficient, hgfx, hgfy, hgfz);
      // }

    for (noi,i) in elemToNodesTuple(eli) {
      fx[noi] += hgfx[i];
      fy[noi] += hgfy[i];
      fz[noi] += hgfz[i];
    }
  }
}

proc CalcAccelerationForNodes() {
  forall noi in NodeDist do local {
      xdd[noi] = fx[noi] / nodalMass[noi];
      ydd[noi] = fy[noi] / nodalMass[noi];
      zdd[noi] = fz[noi] / nodalMass[noi];
    }
}

proc ApplyAccelerationBoundaryConditionsForNodes() {
  /*TODO [holtbg]: symm iterators not parallel yet
    using cobegin to at least make each symm go independently */
  cobegin {
    for i in symmX() do xdd[i] = 0.0;
    for i in symmY() do ydd[i] = 0.0;
    for i in symmZ() do zdd[i] = 0.0;
  }
}

proc CalcVelocityForNodes(dt: real, u_cut: real) {
  forall i in NodeDist do local {
      var xdtmp = xd[i] + xdd[i] * dt,
        ydtmp = yd[i] + ydd[i] * dt,
        zdtmp = zd[i] + zdd[i] * dt;
      if abs(xdtmp) < u_cut then xdtmp = 0.0;
      if abs(ydtmp) < u_cut then ydtmp = 0.0;
      if abs(zdtmp) < u_cut then zdtmp = 0.0;
      xd[i] = xdtmp;
      yd[i] = ydtmp;
      zd[i] = zdtmp;
    }
}

proc CalcPositionForNodes(dt: real) {
  forall ijk in NodeDist {
    x[ijk] += xd[ijk] * dt;
    y[ijk] += yd[ijk] * dt;
    z[ijk] += zd[ijk] * dt;
  }
}

// sungeun: Temporary array reused throughout
proc CalcLagrangeElements() {
var dxx, dyy, dzz: [ElemDist] real;

  CalcKinematicsForElems(dxx, dyy, dzz, deltatime);

  // element loop to do some stuff not included in the elemlib function.
  forall k in ElemDist do local {
      vdov[k] = dxx[k] + dyy[k] + dzz[k];
      var vdovthird = vdov[k] / 3.0;
      dxx[k] -= vdovthird;
      dyy[k] -= vdovthird;
      dzz[k] -= vdovthird;
    }

  // See if any volumes are negative, and take appropriate action.
  if ( || reduce (vnew <= 0.0) ) == true {
    writeln("Error: can't have negative volume."); exit(1);
  }
}

proc CalcKinematicsForElems(dxx, dyy, dzz, const dt: real) {
  // loop over all elements
  forall k in ElemDist {
    var b_x, b_y, b_z: 8*real,
      d: 6*real,
      detJ: real;
    var volume, relativeVolume: real;

    //get nodal coordinates from global arrays and copy into local arrays
    var x_local, y_local, z_local: 8*real;
    localizeNeighborNodes(k, x, x_local, y, y_local, z, z_local);

    //get nodal velocities from global arrays and copy into local arrays
    var xd_local, yd_local, zd_local: 8*real;
    localizeNeighborNodes(k, xd, xd_local, yd, yd_local, zd, zd_local);
    var dt2 = 0.5 * dt; //wish this was local, too...

    local {
      //volume calculations
      volume = CalcElemVolume(x_local, y_local, z_local);
      relativeVolume = volume / volo[k];
      vnew[k] = relativeVolume;
      delv[k] = relativeVolume - v[k];

      //set characteristic length
      arealg[k] = CalcElemCharacteristicLength(x_local, y_local, z_local, volume);

      for param i in 1..8 {
        x_local[i] -= dt2 * xd_local[i];
        y_local[i] -= dt2 * yd_local[i];
        z_local[i] -= dt2 * zd_local[i];
      }

      CalcElemShapeFunctionDerivatives(x_local, y_local, z_local, b_x, b_y, b_z, detJ);

      CalcElemVelocityGradient(xd_local, yd_local, zd_local, b_x, b_y, b_z, detJ, d);

    }
    // put velocity gradient quantities into their global arrays.
    dxx[k] = d[1];
    dyy[k] = d[2];
    dzz[k] = d[3];
  }
}

// sungeun: Temporary array reused throughout
/* velocity gradient */
var delv_xi, delv_eta, delv_zeta: [ElemDist] real;
/* position gradient */
var delx_xi, delx_eta, delx_zeta: [ElemDist] real;
proc CalcQForElems() {
  // MONOTONIC Q option

  /* Calculate velocity gradients */
  CalcMonotonicQGradientsForElems(delv_xi, delv_eta, delv_zeta, 
                                  delx_xi, delx_eta, delx_zeta);

  /* Transfer veloctiy gradients in the first order elements */
  /* problem->commElements->Transfer(CommElements::monoQ) ; */
  CalcMonotonicQForElems(delv_xi, delv_eta, delv_zeta,
                         delx_xi, delx_eta, delx_zeta);

  /* Don't allow excessive artificial viscosity */
  if ( || reduce (q > qstop) ) == true {
    writeln("qstop = ", qstop);
    writeln("q:\n", q);
    writeln("Excessive artificial viscosity!");
    exit(1);
  }
}

// sungeun: Temporary array reused throughout
var vnewc: [ElemDist] real;
proc ApplyMaterialPropertiesForElems() {
  /* Expose all of the variables needed for material evaluation */

  forall i in ElemDist do vnewc[i] = vnew[matElemlist[i]];

  if eosvmin != 0.0 {
    [c in vnewc] if c < eosvmin then c = eosvmin;
  }
  if eosvmax != 0.0 {
    [c in vnewc] if c > eosvmax then c = eosvmax;
  }

  //does this actually do anything?
  forall eli in ElemDist {
    var vc = v[matElemlist[eli]];
    if eosvmin != 0.0 && vc < eosvmin then vc = eosvmin;
    if eosvmax != 0.0 && vc > eosvmax then vc = eosvmax;
    if vc <= 0.0 {
      writeln("Volume error (in ApplyMaterialProperiesForElems).");
      exit(1);
    }
  }

  EvalEOSForElems(vnewc);
}

proc UpdateVolumesForElems() {
  forall i in ElemDist do local {
    var tmpV = vnew[i];
    if abs(tmpV-1.0) < v_cut then tmpV = 1.0;
    v[i] = tmpV;
  }
}

proc CalcMonotonicQGradientsForElems(delv_xi, delv_eta, delv_zeta, 
                                     delx_xi, delx_eta, delx_zeta) {
  forall eli in ElemDist {
    const ptiny = 1.0e-36;
    var xl, yl, zl: 8*real;
    localizeNeighborNodes(eli, x, xl, y, yl, z, zl);
    var xvl, yvl, zvl: 8*real;
    localizeNeighborNodes(eli, xd, xvl, yd, yvl, zd, zvl);

    local {
      var vol = volo[eli] * vnew[eli],
        norm = 1.0 / (vol + ptiny);
      var ax, ay, az, dxv, dyv, dzv: real;

      var dxj = -0.25 * ((xl[1]+xl[2]+xl[6]+xl[5]) - (xl[4]+xl[3]+xl[7]+xl[8])),
        dyj = -0.25 * ((yl[1]+yl[2]+yl[6]+yl[5]) - (yl[4]+yl[3]+yl[7]+yl[8])),
        dzj = -0.25 * ((zl[1]+zl[2]+zl[6]+zl[5]) - (zl[4]+zl[3]+zl[7]+zl[8])),
      
        dxi =  0.25 * ((xl[2]+xl[3]+xl[7]+xl[6]) - (xl[1]+xl[4]+xl[8]+xl[5])),
        dyi =  0.25 * ((yl[2]+yl[3]+yl[7]+yl[6]) - (yl[1]+yl[4]+yl[8]+yl[5])),
        dzi =  0.25 * ((zl[2]+zl[3]+zl[7]+zl[6]) - (zl[1]+zl[4]+zl[8]+zl[5])),
      
        dxk =  0.25 * ((xl[5]+xl[6]+xl[7]+xl[8]) - (xl[1]+xl[2]+xl[3]+xl[4])),
        dyk =  0.25 * ((yl[5]+yl[6]+yl[7]+yl[8]) - (yl[1]+yl[2]+yl[3]+yl[4])),
        dzk =  0.25 * ((zl[5]+zl[6]+zl[7]+zl[8]) - (zl[1]+zl[2]+zl[3]+zl[4]));

      /* find delvk and delxk ( i cross j ) */

      ax = dyi*dzj - dzi*dyj;
      ay = dzi*dxj - dxi*dzj;
      az = dxi*dyj - dyi*dxj;

      delx_zeta[eli] = vol / sqrt(ax*ax + ay*ay + az*az + ptiny);

      ax *= norm;
      ay *= norm;
      az *= norm;

      dxv = 0.25 * ((xvl[5]+xvl[6]+xvl[7]+xvl[8]) - (xvl[1]+xvl[2]+xvl[3]+xvl[4]));
      dyv = 0.25 * ((yvl[5]+yvl[6]+yvl[7]+yvl[8]) - (yvl[1]+yvl[2]+yvl[3]+yvl[4]));
      dzv = 0.25 * ((zvl[5]+zvl[6]+zvl[7]+zvl[8]) - (zvl[1]+zvl[2]+zvl[3]+zvl[4]));

      delv_zeta[eli] = ax*dxv + ay*dyv + az*dzv;

      /* find delxi and delvi ( j cross k ) */

      ax = dyj*dzk - dzj*dyk;
      ay = dzj*dxk - dxj*dzk;
      az = dxj*dyk - dyj*dxk;

      delx_xi[eli] = vol / sqrt(ax*ax + ay*ay + az*az + ptiny) ;

      ax *= norm;
      ay *= norm;
      az *= norm;

      dxv = 0.25 * ((xvl[2]+xvl[3]+xvl[7]+xvl[6]) - (xvl[1]+xvl[4]+xvl[8]+xvl[5]));
      dyv = 0.25 * ((yvl[2]+yvl[3]+yvl[7]+yvl[6]) - (yvl[1]+yvl[4]+yvl[8]+yvl[5]));
      dzv = 0.25 * ((zvl[2]+zvl[3]+zvl[7]+zvl[6]) - (zvl[1]+zvl[4]+zvl[8]+zvl[5]));

      delv_xi[eli] = ax*dxv + ay*dyv + az*dzv ;

      /* find delxj and delvj ( k cross i ) */

      ax = dyk*dzi - dzk*dyi;
      ay = dzk*dxi - dxk*dzi;
      az = dxk*dyi - dyk*dxi;

      delx_eta[eli] = vol / sqrt(ax*ax + ay*ay + az*az + ptiny) ;

      ax *= norm;
      ay *= norm;
      az *= norm;

      dxv = -0.25 * ((xvl[1]+xvl[2]+xvl[6]+xvl[5]) - (xvl[4]+xvl[3]+xvl[7]+xvl[8]));
      dyv = -0.25 * ((yvl[1]+yvl[2]+yvl[6]+yvl[5]) - (yvl[4]+yvl[3]+yvl[7]+yvl[8]));
      dzv = -0.25 * ((zvl[1]+zvl[2]+zvl[6]+zvl[5]) - (zvl[4]+zvl[3]+zvl[7]+zvl[8]));

      delv_eta[eli] = ax*dxv + ay*dyv + az*dzv ;
    } /* local */
  } /* forall eli */
}

proc CalcMonotonicQForElems(delv_xi, delv_eta, delv_zeta, 
                            delx_xi, delx_eta, delx_zeta) {
  //got rid of call through to "CalcMonotonicQRegionForElems"

  forall (eli,i) in (ElemDist,matElemlist) {
    const ptiny = 1.0e-36;
    const bcMask = elemBC[i];
    var norm, delvm, delvp: real;

    /* phixi */
    norm = 1.0 / (delv_xi[i] + ptiny);

    select bcMask & XI_M {
      when 0         do delvm = delv_xi[lxim(i)];
      when XI_M_SYMM do delvm = delv_xi[i];
      when XI_M_FREE do delvm = 0.0;
      }
    select bcMask & XI_P {
      when 0         do delvp = delv_xi[lxip(i)];
      when XI_P_SYMM do delvp = delv_xi[i];
      when XI_P_FREE do delvp = 0.0;
      }

    delvm *= norm;
    delvp *= norm;

    var phixi = 0.5 * (delvm + delvp);

    delvm *= monoq_limiter_mult;
    delvp *= monoq_limiter_mult;

    if delvm < phixi           then phixi = delvm;
    if delvp < phixi           then phixi = delvp;
    if phixi < 0               then phixi = 0.0;
    if phixi > monoq_max_slope then phixi = monoq_max_slope;

    /* phieta */
    norm = 1.0 / (delv_eta[i] + ptiny);

    select bcMask & ETA_M {
      when 0          do delvm = delv_eta[letam[i]];
      when ETA_M_SYMM do delvm = delv_eta[i];      
      when ETA_M_FREE do delvm = 0.0;      
      }
    select bcMask & ETA_P {
      when 0          do delvp = delv_eta[letap[i]];
      when ETA_P_SYMM do delvp = delv_eta[i];      
      when ETA_P_FREE do delvp = 0.0;      
      }

    delvm = delvm * norm;
    delvp = delvp * norm;

    var phieta = 0.5 * (delvm + delvp);

    delvm *= monoq_limiter_mult;
    delvp *= monoq_limiter_mult;

    if delvm  < phieta			then phieta = delvm;
    if delvp  < phieta			then phieta = delvp;
    if phieta < 0.0				then phieta = 0.0;
    if phieta > monoq_max_slope	then phieta = monoq_max_slope;

    /*  phizeta     */
    norm = 1.0 / (delv_zeta[i] + ptiny);

    select bcMask & ZETA_M {
      when 0           do delvm = delv_zeta[lzetam[i]];
      when ZETA_M_SYMM do delvm = delv_zeta[i];       
      when ZETA_M_FREE do delvm = 0.0;        
      }
    select bcMask & ZETA_P {
      when 0           do delvp = delv_zeta[lzetap[i]];
      when ZETA_P_SYMM do delvp = delv_zeta[i];       
      when ZETA_P_FREE do delvp = 0.0;        
      }

    delvm = delvm * norm;
    delvp = delvp * norm;

    var phizeta = 0.5 * (delvm + delvp);

    delvm *= monoq_limiter_mult;
    delvp *= monoq_limiter_mult;

    if delvm   < phizeta 		 then phizeta = delvm;
    if delvp   < phizeta		 then phizeta = delvp;
    if phizeta < 0.0			 then phizeta = 0.0;
    if phizeta > monoq_max_slope then phizeta = monoq_max_slope;

    /* Remove length scale */
    var qlin, qquad: real;
    if vdov[i] > 0.0 {
      qlin  = 0.0;
      qquad = 0.0;
    } else {
      var delvxxi   = delv_xi[i]   * delx_xi[i],
        delvxeta  = delv_eta[i]  * delx_eta[i],
        delvxzeta = delv_zeta[i] * delx_zeta[i];

      if delvxxi   > 0.0 then delvxxi   = 0.0;
      if delvxeta  > 0.0 then delvxeta  = 0.0;
      if delvxzeta > 0.0 then delvxzeta = 0.0;

      var rho = elemMass[i] / (volo[i] * vnew[i]);

      qlin = -qlc_monoq * rho *
        ( delvxxi   * (1.0 - phixi) +
          delvxeta  * (1.0 - phieta) +
          delvxzeta * (1.0 - phizeta));

      qquad = qqc_monoq * rho *
        ( delvxxi**2   * (1.0 - phixi**2) +
          delvxeta**2  * (1.0 - phieta**2) +
          delvxzeta**2 * (1.0 - phizeta**2));
    }
    qq[i] = qquad;
    ql[i] = qlin;

  }
}

proc EvalEOSForElems(vnewc) {
  const rho0 = refdens;

  var e_old, delvc, p_old, q_old, compression, compHalfStep, 
    qq_old, ql_old, work, p_new, e_new, q_new, bvc, pbvc: [ElemDist] real;

  /* compress data, minimal set */
  forall (i,zidx) in (ElemDist,matElemlist) {
    e_old[i]  = e[zidx];
    delvc[i]  = delv[zidx];
    p_old[i]  = p[zidx];
    q_old[i]  = q[zidx];
    qq_old[i] = qq[zidx];
    ql_old[i] = ql[zidx];
  }

  forall i in ElemDist do local {
    compression[i] = 1.0 / vnewc[i] - 1.0;
    var vchalf = vnewc[i] - delvc[i] * 0.5;
    compHalfStep[i] = 1.0 / vchalf - 1.0;
  }

  /* Check for v > eosvmax or v < eosvmin */
  // (note: I think this was already checked for in calling function!)
  if eosvmin != 0.0 {
    forall i in ElemDist {
      if vnewc[i] <= eosvmin then compHalfStep[i] = compression[i];
    }
  }
  if eosvmax != 0.0 {
    forall i in ElemDist {
      if vnewc[i] >= eosvmax {
        p_old[i] = 0.0;
        compression[i] = 0.0;
        compHalfStep[i] = 0.0;
      }
    }
  }

  //work = 0.0;	//unnecessary, defaults to 0

  CalcEnergyForElems(p_new, e_new, q_new, bvc, pbvc, 
                     p_old, e_old, q_old, compression, compHalfStep, 
                     vnewc, work, delvc, qq_old, ql_old);

  forall (i,zidx) in (ElemDist,matElemlist) {
    p[zidx] = p_new[i];
    e[zidx] = e_new[i];
    q[zidx] = q_new[i];
  }

  CalcSoundSpeedForElems(vnewc, rho0, e_new, p_new, pbvc, bvc);
}

proc CalcEnergyForElems(p_new, e_new, q_new, bvc, pbvc,
                        p_old, e_old, q_old, compression, compHalfStep, 
                        vnewc, work, delvc, qq_old, ql_old) {
  /* TODO [holtbg]: might need to move these consts into foralls or global
     Otherwise, they live on Locale0 and everyone else has to do 
     remote reads */
  /* TODO [sungeun]: Check if these are remote value forwarded. */
  const rho0 = refdens; 
  const sixth = 1.0 / 6.0;
  var pHalfStep: [ElemDist] real;

  forall i in ElemDist {
    e_new[i] = e_old[i] - 0.5 * delvc[i] * (p_old[i] + q_old[i]) + 0.5 * work[i];
    if e_new[i] < emin then e_new[i] = emin;
  }

  CalcPressureForElems(pHalfStep, bvc, pbvc, e_new, compHalfStep, 
                       vnewc, pmin, p_cut, eosvmax);

  forall i in ElemDist {
    const vhalf = 1.0 / (1.0 + compHalfStep[i]);

    if delvc[i] > 0.0 {
      q_new[i] = 0.0;
    } else {
      var ssc = ( pbvc[i] * e_new[i] + vhalf**2 * bvc[i] * pHalfStep[i]) / rho0;
      if ssc <= 0.0 then ssc = 0.333333e-36;
      else ssc = sqrt(ssc);
      q_new[i] = ssc * ql_old[i] + qq_old[i];
    }

    e_new[i] += 0.5 * delvc[i]
      * (3.0*(p_old[i] + q_old[i]) - 4.0*(pHalfStep[i] + q_new[i]));
  }
  forall i in ElemDist {
    e_new[i] += 0.5 * work[i];
    if abs(e_new[i] < e_cut) then e_new[i] = 0.0;
    if e_new[i] < emin then e_new[i] = emin;
  }

  CalcPressureForElems(p_new, bvc, pbvc, e_new, compression, vnewc, pmin, p_cut, eosvmax);

  forall i in ElemDist {
    var q_tilde:real;

    if delvc[i] > 0.0 {
      q_tilde = 0.0;
    } else {
      var ssc = ( pbvc[i] * e_new[i] + vnewc[i]**2 * bvc[i] * p_new[i] ) / rho0;
      if ssc <= 0.0 then ssc = 0.333333e-36;
      else ssc = sqrt(ssc);
      q_tilde = ssc * ql_old[i] + qq_old[i];
    }

    e_new[i] -= (7.0*(p_old[i] + q_old[i]) 
                 - 8.0*(pHalfStep[i] + q_new[i]) 
                 + (p_new[i] + q_tilde)) * delvc[i] * sixth;
    if abs(e_new[i]) < e_cut then e_new[i] = 0.0;
    if e_new[i] < emin then e_new[i] = emin;
  }

  CalcPressureForElems(p_new, bvc, pbvc, e_new, compression, vnewc, pmin, p_cut, eosvmax);

  forall i in ElemDist do local {
      if delvc[i] <= 0.0 {
        var ssc = ( pbvc[i] * e_new[i] + vnewc[i]**2 * bvc[i] * p_new[i] ) / rho0;
        if ssc <= 0.0 then ssc = 0.333333e-36;
        else ssc = sqrt(ssc);
        q_new[i] = ssc * ql_old[i] + qq_old[i];
        if abs(q_new[i]) < q_cut then q_new[i] = 0.0;
      }
    }
}

proc CalcSoundSpeedForElems(vnewc, rho0:real, enewc, pnewc, pbvc, bvc) {
  forall (i,iz) in (ElemDist,matElemlist) {
    var ssTmp = (pbvc[i] * enewc[i] + vnewc[i]**2 * bvc[i] * pnewc[i]) / rho0;
    if ssTmp <= 1.111111e-36 then ssTmp = 1.111111e-36;
    ss[iz] = sqrt(ssTmp);
  }
}

// useful iterators
iter tensor(r1:range, r2:range, r3:range) {
  for i in r1 {
    for j in r2 {
      for k in r3 {
        yield (i, j, k);
      }
    }
  }
}

iter elemToNodesTuple(i: Eid) {
  //(need to make sure these are in the right order, so hard-coded)
  yield ((i[1]  , i[2]  , i[3]  ), 1);
  yield ((i[1]  , i[2]  , i[3]+1), 2);
  yield ((i[1]  , i[2]+1, i[3]+1), 3);
  yield ((i[1]  , i[2]+1, i[3]  ), 4);
  yield ((i[1]+1, i[2]  , i[3]  ), 5);
  yield ((i[1]+1, i[2]  , i[3]+1), 6);
  yield ((i[1]+1, i[2]+1, i[3]+1), 7);
  yield ((i[1]+1, i[2]+1, i[3]  ), 8);
}

iter nodeToElems(i: Nid): Eid {
  var min = ( if i[1] > 0 then i[1]-1 else i[1],
              if i[2] > 0 then i[2]-1 else i[2],
              if i[3] > 0 then i[3]-1 else i[3] ),
      max = ( if i[1] < edgeElems then i[1] else i[1]-1,
              if i[2] < edgeElems then i[2] else i[2]-1,
              if i[3] < edgeElems then i[3] else i[3]-1 );
  for (x,y,z) in tensor(min[1]..max[1], min[2]..max[2], min[3]..max[3]) {
    yield (x,y,z);
  }
}

// test & debug stuff
proc testInit() {
  writeln("Coordinates:");
  for i in NodeSpace {
    writeln("(",format("%g",x[i]),", ",format("%g",y[i]),", ",format("%g",z[i]),")");
  }
  writeln("ElemMass:");
  for mass in elemMass do writeln(mass);
  writeln("NodalMass:");
  for mass in nodalMass do writeln(mass);
  writeln("symm:");
  for (x,y,z) in (symmX(), symmY(), symmZ()) {
    writeln((NodeSpace.indexOrder(x), NodeSpace.indexOrder(y), NodeSpace.indexOrder(z)));
  }
  write("lxim:");
  for i in ElemSpace do write(" ", ElemSpace.indexOrder(lxim(i)));
  write("\nlxip:");
  for i in ElemSpace do write(" ", ElemSpace.indexOrder(lxip(i)));
  writeln();
  write("letam:");
  for i in ElemSpace do write(" ", ElemSpace.indexOrder(letam(i)));
  writeln();
  write("letap:");
  for i in ElemSpace do write(" ", ElemSpace.indexOrder(letap(i)));
  writeln();
  write("lzetam:");
  for i in ElemSpace do write(" ", ElemSpace.indexOrder(lzetam(i)));
  writeln();
  write("lzetap:");
  for i in ElemSpace do write(" ", ElemSpace.indexOrder(lzetap(i)));
  writeln();
  writeln("elemBC:");
  for b in elemBC do writeln(b);

  writeln("done with initialization");
}

proc deprint(title:string, x:[?D] real, y:[D]real, z:[D]real) {
  writeln(title);
  for i in D[0..0,..,..] {
    writeln(format("%3d",D.indexOrder(i)), ": ", 
            format("%3.4e", x[i]), " ", 
            format("%3.4e", y[i]), " ", 
            format("%3.4e", z[i]));
  }
}

proc deprint(title:string, A: [?D] real) {
  writeln(title);
  writeln([a in A] format("%g",a), " ");
}

