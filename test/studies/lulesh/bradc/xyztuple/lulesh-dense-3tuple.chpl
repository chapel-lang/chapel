/*
  Derived from the DARPA/Livermore Unstructured Lagrangian Explicit
  Shock Hydrodynamics (LULESH)
  https://computation.llnl.gov/casc/ShockHydro/

  Original port to Chapel by Brandon Holt (8/2011).  Further
  improvements for the sake of performance and/or generality made by
  Sung-Eun Choi (12/2011), Jeff Keasler (3/2012), and Brad Chamberlain
  (3-4/2012).


  Notes on the Initial Implementation
  -----------------------------------
   
  This implementation was designed to mirror the overall structure of
  the C++ Lulesh but use Chapel constructs where they can help make
  the code more readable, easier to maintain, or more
  'elegant'. Function names are preserved for the most part, with some
  additional helper functions, and original comments from the C++ code
  are interspersed approximately where they belong to give an idea of
  how the two codes line up. One major difference for this Chapel
  version is the use of a number of module-level variables and
  constants.


  Status:

  This code remains a work-in-progress as we gain further experience
  with it.  Proposed improvements are noted in the README in this
  directory and (in some cases) in TODO comments in the code.

 */



use Time,       // to get timing routines for benchmarking
    BlockDist,  // for block-distributed arrays
    IO;         // for file operations

use luleshInit3, luleshTupleHelp;   // to get I/O version of setting up data structures

/* The 'useBlockDist' configuration parameter says whether or not to
   block-distribute the arrays.  The default depends on the setting of
   CHPL_COMM and can be overridden on the compiler command-line using
   -suseBlockDist=[true|false] */

config param useBlockDist = (CHPL_COMM != "none");  // block-distribute arrays?


/* Configuration constants: Override defaults on executable's command-line */

config const initialEnergy = 3.948746e+7;            // initial energy value


config const showProgress = false,   // print time and dt values on each step
             debug = false,          // print various debug info
             doTiming = true,        // time the main timestep loop
             printCoords = true;     // print the final computed coordinates


/* Compile-time constants */

param XI_M        = 0x003,
      XI_M_SYMM   = 0x001,
      XI_M_FREE   = 0x002,

      XI_P        = 0x00c,
      XI_P_SYMM   = 0x004,
      XI_P_FREE   = 0x008,

      ETA_M       = 0x030,
      ETA_M_SYMM  = 0x010,
      ETA_M_FREE  = 0x020,

      ETA_P       = 0x0c0,
      ETA_P_SYMM  = 0x040,
      ETA_P_FREE  = 0x080,

      ZETA_M      = 0x300,
      ZETA_M_SYMM = 0x100,
      ZETA_M_FREE = 0x200,

      ZETA_P      = 0xc00,
      ZETA_P_SYMM = 0x400,
      ZETA_P_FREE = 0x800;


/* Set up the problem size */

const (numElems, numNodes) = initProblemSize();


/* Declare abstract problem domains */

const ElemSpace = {0..#numElems},
      NodeSpace = {0..#numNodes};


/* Declare the (potentially distributed) problem domains */

const Elems = if useBlockDist then ElemSpace dmapped Block(ElemSpace)
                              else ElemSpace,
      Nodes = if useBlockDist then NodeSpace dmapped Block(NodeSpace)
                              else NodeSpace;


                              
param numdims = 3,
            X = 0,
            Y = 1,
            Z = 2;
                              
/* The coordinates */

var xyz: [Nodes] numdims*real;
                              



/* The number of nodes per element.  In a rank-independent version,
   this could be written 2**rank */

param nodesPerElem = 8;

                                 
// We could name this, but chose not to since it doesn't add that much clarity
//
// const elemNeighbors = 0..nodesPerElem-1;


/* The element-to-node mapping */

var elemToNode: [Elems] nodesPerElem*index(Nodes);


/* the Greek variables */

var lxim, lxip, letam, letap, lzetam, lzetap: [Elems] index(Elems);


/* the X, Y, Z Symmetry values */

var XSym, YSym, ZSym: sparse subdomain(Nodes);



/* Constants */

const u_cut = 1.0e-7,           /* velocity tolerance */
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
      qqc2 = 64.0 * qqc**2,
      eosvmax = 1.0e+9,
      eosvmin = 1.0e-9,
      pmin = 0.0,               /* pressure floor */
      emin = -1.0e+15,          /* energy floor */
      dvovmax = 0.1,            /* maximum allowable volume change */
      refdens = 1.0,            /* reference density */

      deltatimemultlb = 1.1,
      deltatimemultub = 1.2,
      dtmax = 1.0e-2;           /* maximum allowable time increment */

config const stoptime = 1.0e-2,      /* end time for simulation */
             maxcycles = max(int),   /* max # of steps to take */
             dtfixed = -1.0e-7       /* fixed time increment */
            ;        


/* The list of material elements */

// NOTE: Using dense domains/arrays to represent materials which doesn't
// permit us to select an arbitrary subset
var MatElems = Elems;

iter enumerateMatElems() {
  for i in Elems do
    yield i;
}


/* Element fields */

var elemBC: [Elems] int,

    e: [Elems] real, // energy
    p: [Elems] real, // pressure

    q: [Elems] real, // q
    ql: [Elems] real, // linear term for q
    qq: [Elems] real, // quadratic term for q

    v:    [Elems] real = 1.0, //relative volume
    vnew: [Elems] real,

    volo: [Elems] real, // reference volume
    delv: [Elems] real, // m_vnew - m_v
    vdov: [Elems] real, // volume derivative over volume

    arealg: [Elems] real, // elem characteristic length

    ss: [Elems] real, // "sound speed"

    elemMass: [Elems] real; // mass


/* Nodal fields */

var xyzd: [Nodes] 3*real, // velocities

    xyzdd: [Nodes] 3*real, // acceleration

    // TODO: Can this be 3*(atomic real)?
    fx, fy, fz: [Nodes] atomic real, // forces

    nodalMass: [Nodes] real; // mass


/* Parameters */

var time = 0.0,          // current time
    deltatime = 1.0e-7,  // variable time increment
    dtcourant = 1.0e20,  // courant constraint
    dthydro = 1.0e20,    // volume change constraint

    cycle = 0;           // iteration count for simulation


proc main() {
  if debug then writeln("Lulesh -- Problem Size = ", numElems);

  initLulesh();

  var st: real;
  if doTiming then st = getCurrentTime();
  while (time < stoptime && cycle < maxcycles) {
    const iterTime = if showProgress then getCurrentTime() else 0.0;

    TimeIncrement();

    LagrangeLeapFrog();

    if debug {
      //      deprint("[[ Forces ]]", fx, fy, fz);
      deprint("[[ Positions ]]", xyz);
      deprint("[[ p, e, q ]]", p, e, q);
    }
    if showProgress then
      writef("time = %er, dt=%er%s\n", time, deltatime, 
       if doTiming then ", elapsed = " + (getCurrentTime()-iterTime):string
                   else "");
  }
  if (cycle == maxcycles) {
    writeln("Stopped early due to reaching maxnumsteps");
  }
  if doTiming {
    const et = getCurrentTime();
    writeln("Total Time: ", et-st);
    writeln("Time/Cycle: ", (et-st)/cycle);
  }
  writeln("Number of cycles: ", cycle);

  if printCoords {
    var outfile = open("coords.out", iomode.cw);
    var writer = outfile.writer();
    var fmtstrnum = if debug then "%1.9er" else "%1.4er";
    var fmtstr = fmtstrnum + " " + fmtstrnum + " " + fmtstrnum + "\n";
    for i in Nodes {
      writer.writef(fmtstr, xyz[i][X], xyz[i][Y], xyz[i][Z]);
    }
    writer.close();
    outfile.close();
  }
}


/* Initialization functions */

proc initLulesh() {
  // initialize the coordinates
  initCoordinates(xyz);

  // initialize the element to node mapping
  initElemToNodeMapping(elemToNode);

  // initialize the greek symbols
  initGreekVars(lxim, lxip, letam, letap, lzetam, lzetap);

  // initialize the symmetry plane locations
  initXSyms(XSym);
  initYSyms(YSym);
  initZSyms(ZSym);

  /* embed hexahedral elements in nodal point lattice */
  //calculated on the fly using: elemToNodes(i: index(Elems)): index(Nodes)

  // initialize the masses
  initMasses();

  // initialize the boundary conditions
  const octantCorner = initBoundaryConditions();

  // deposit the energy for Sedov Problem
  e[octantCorner] = initialEnergy;
}


proc initMasses() {
  // This is a temporary array used to accumulate masses in parallel
  // without losing updates by using 'atomic' variables
  var massAccum: [Nodes] atomic real;
  for i in Nodes {
    massAccum[i].write(0.0);
  }

  forall eli in Elems {
    var xyz_local: 8*(3*real);
    localizeNeighborNodes(eli, xyz, xyz_local);

    var volume = CalcElemVolume(xyz_local);
    volo[eli] = volume;
    elemMass[eli] = volume;

    for neighbor in elemToNodes[eli] {
      massAccum[neighbor].add(volume);
    }
  }

  // When we're done, copy the accumulated masses into nodalMass, at
  // which point the massAccum array can go away (and will at the
  // procedure's return

  for i in Nodes {
    nodalMass[i] = massAccum[i].read() / 8.0;
  }

  if debug {
    writeln("ElemMass:");
    for mass in elemMass do writeln(mass);

    writeln("NodalMass:");
    for mass in nodalMass do writeln(mass);
  }
}


proc initBoundaryConditions() {
  var surfaceNode: [Nodes] int;

  forall n in XSym do
    surfaceNode[n] = 1;
  forall n in YSym do
    surfaceNode[n] = 1;
  forall n in ZSym do
    surfaceNode[n] = 1;

  forall e in Elems do {
    var mask: int;
    for i in 0..#nodesPerElem do
      mask += surfaceNode[elemToNode[e][i]] << i;

    // TODO: make an inlined function for this little idiom? (and below)

    if ((mask & 0x0f) == 0x0f) then elemBC[e] |= ZETA_M_SYMM;
    if ((mask & 0xf0) == 0xf0) then elemBC[e] |= ZETA_P_SYMM;
    if ((mask & 0x33) == 0x33) then elemBC[e] |= ETA_M_SYMM;
    if ((mask & 0xcc) == 0xcc) then elemBC[e] |= ETA_P_SYMM;
    if ((mask & 0x99) == 0x99) then elemBC[e] |= XI_M_SYMM;
    if ((mask & 0x66) == 0x66) then elemBC[e] |= XI_P_SYMM;
  }


  //
  // We find the octant corner by looking for the element with
  // all three SYMM flags set, which will have the largest
  // integral value.  Thus, we can use a maxloc to identify it.
  //
  var (check, loc) = maxloc reduce zip(elemBC, Elems);

  if debug then writeln("Found the octant corner at: ", loc);

  if (check != (XI_M_SYMM | ETA_M_SYMM | ZETA_M_SYMM)) then
    halt("maxloc got a value of ", check, " at loc ", loc);

  // TODO: This is an example of an array that, in a distributed
  // memory code, would typically be completely local and only storing
  // the local nodes owned by the locale -- noting that some nodes
  // are logically owned by multiple locales and therefore would 
  // redundantly be stored in both locales' surfaceNode arrays -- it's
  // essentially local scratchspace that does not need to be communicated
  // or kept coherent across locales.
  //

  surfaceNode = 0;

  /* the free surfaces */

  var freeSurface: sparse subdomain(Nodes);

  // initialize the free surface
  initFreeSurface(freeSurface);

  forall n in freeSurface do
    surfaceNode[n] = 1;

  forall e in Elems do {
    var mask: int;
    for i in 0..#nodesPerElem do
      mask += surfaceNode[elemToNode[e][i]] << i;

    if ((mask & 0x0f) == 0x0f) then elemBC[e] |= ZETA_M_FREE;
    if ((mask & 0xf0) == 0xf0) then elemBC[e] |= ZETA_P_FREE;
    if ((mask & 0x33) == 0x33) then elemBC[e] |= ETA_M_FREE;
    if ((mask & 0xcc) == 0xcc) then elemBC[e] |= ETA_P_FREE;
    if ((mask & 0x99) == 0x99) then elemBC[e] |= XI_M_FREE;
    if ((mask & 0x66) == 0x66) then elemBC[e] |= XI_P_FREE;
  }

  if debug {
    writeln("elemBC:");
    for b in elemBC do writeln(b);
  }

  return loc;
}


/* Helper functions */

inline proc localizeNeighborNodes(eli: index(Elems),
                                  x: [] real, ref x_local: 8*real,
                                  y: [] real, ref y_local: 8*real,
                                  z: [] real, ref z_local: 8*real) {

  for param i in 0..nodesPerElem-1 {
    const noi = elemToNode[eli][i];
    
    x_local[i] = x[noi];
    y_local[i] = y[noi];
    z_local[i] = z[noi];
  }
}

/*
inline proc localizeNeighborNodes(eli: index(Elems),
                                  xyz: [] 3*real, ref xyz_local: 3*(8*real)) {

  for param i in 0..nodesPerElem-1 {
    const noi = elemToNode[eli][i];
    const locxyz = xyz[noi];

    for param j in 0..2 do
      xyz_local[j][i] = locxyz[j];
  }
}
*/

inline proc localizeNeighborNodes(eli: index(Elems),
                                  xyz: [] 3*real, ref xyz_local: 8*(3*real)) {

  for param i in 0..nodesPerElem-1 {
    const noi = elemToNode[eli][i];

    xyz_local[i] = xyz[noi];
  }
}

inline proc TripleProduct(xyz1, xyz2, xyz3) {
  return xyz1[X]*(xyz2[Y]*xyz3[Z] - xyz2[Z]*xyz3[Y]) + 
         xyz2[X]*(xyz1[Z]*xyz3[Y] - xyz1[Y]*xyz3[Z]) + 
         xyz3[X]*(xyz1[Y]*xyz2[Z] - xyz1[Z]*xyz2[Y]);
}


proc CalcElemVolume(xyz) {
  var dxyz61 = xyz[6] - xyz[1],
      dxyz70 = xyz[7] - xyz[0],
      dxyz63 = xyz[6] - xyz[3],
      dxyz20 = xyz[2] - xyz[0],
      dxyz50 = xyz[5] - xyz[0],
      dxyz64 = xyz[6] - xyz[4],
      dxyz31 = xyz[3] - xyz[1],
      dxyz72 = xyz[7] - xyz[2],
      dxyz43 = xyz[4] - xyz[3],
      dxyz57 = xyz[5] - xyz[7],
      dxyz14 = xyz[1] - xyz[4],
      dxyz25 = xyz[2] - xyz[5];

  var volume = TripleProduct(dxyz31 + dxyz72, dxyz63, dxyz20) +
               TripleProduct(dxyz43 + dxyz57, dxyz64, dxyz70) +
               TripleProduct(dxyz14 + dxyz25, dxyz61, dxyz50);

  return volume / 12.0;
}

proc InitStressTermsForElems(p, q, sigxxyyzz) {
  forall i in sigxxyyzz.domain {
    const pq = -p[i] - q[i];
    sigxxyyzz[i] = (pq, pq, pq);
  }
}


proc CalcElemShapeFunctionDerivatives(xyz: 8*(3*real),
                                      ref b_xyz: 8*(3*real),
                                      ref volume: real) {

  const fjxyzxi = .125 * ( (xyz[6]-xyz[0]) + (xyz[5]-xyz[3]) - (xyz[7]-xyz[1]) - (xyz[4]-xyz[2]) ),
      fjxyzet = .125 * ( (xyz[6]-xyz[0]) - (xyz[5]-xyz[3]) + (xyz[7]-xyz[1]) - (xyz[4]-xyz[2]) ),
      fjxyzze = .125 * ( (xyz[6]-xyz[0]) + (xyz[5]-xyz[3]) + (xyz[7]-xyz[1]) + (xyz[4]-xyz[2]) );

  /* compute cofactors */
  var cjxyzxi, cjxyzet, cjxyzze: 3*real;

  cjxyzxi[X] =    (fjxyzet[Y] * fjxyzze[Z]) - (fjxyzet[Z] * fjxyzze[Y]);
  cjxyzet[X] =  - (fjxyzxi[Y] * fjxyzze[Z]) + (fjxyzxi[Z] * fjxyzze[Y]);
  cjxyzze[X] =    (fjxyzxi[Y] * fjxyzet[Z]) - (fjxyzxi[Z] * fjxyzet[Y]);

  cjxyzxi[Y] =  - (fjxyzet[X] * fjxyzze[Z]) + (fjxyzet[Z] * fjxyzze[X]);
  cjxyzet[Y] =    (fjxyzxi[X] * fjxyzze[Z]) - (fjxyzxi[Z] * fjxyzze[X]);
  cjxyzze[Y] =  - (fjxyzxi[X] * fjxyzet[Z]) + (fjxyzxi[Z] * fjxyzet[X]);

  cjxyzxi[Z] =    (fjxyzet[X] * fjxyzze[Y]) - (fjxyzet[Y] * fjxyzze[X]);
  cjxyzet[Z] =  - (fjxyzxi[X] * fjxyzze[Y]) + (fjxyzxi[Y] * fjxyzze[X]);
  cjxyzze[Z] =    (fjxyzxi[X] * fjxyzet[Y]) - (fjxyzxi[Y] * fjxyzet[X]);

  /* calculate partials :
     this need only be done for l = 0,1,2,3   since , by symmetry ,
     (6,7,4,5) = - (0,1,2,3) .
  */
  b_xyz[0] =   -  cjxyzxi  -  cjxyzet  -  cjxyzze;
  b_xyz[1] =      cjxyzxi  -  cjxyzet  -  cjxyzze;
  b_xyz[2] =      cjxyzxi  +  cjxyzet  -  cjxyzze;
  b_xyz[3] =   -  cjxyzxi  +  cjxyzet  -  cjxyzze;
  b_xyz[4] = -b_xyz[2];
  b_xyz[5] = -b_xyz[3];
  b_xyz[6] = -b_xyz[0];
  b_xyz[7] = -b_xyz[1];

  /* calculate jacobian determinant (volume) */
  volume = 8.0 * sumOfProduct(fjxyzet, cjxyzet);
}


proc CalcElemNodeNormals(ref pfxyz: 8*(3*real), xyz: 8*(3*real)) {

  proc ElemFaceNormal(param n1, param n2, param n3, param n4) {
    var bisectXYZ0 = 0.5 * (xyz[n4] + xyz[n3] - xyz[n2] - xyz[n1]),
        bisectXYZ1 = 0.5 * (xyz[n3] + xyz[n2] - xyz[n4] - xyz[n1]),
        areaXYZ = (0.25 * (bisectXYZ0[Y] * bisectXYZ1[Z] - bisectXYZ0[Z] * bisectXYZ1[Y]),
                   0.25 * (bisectXYZ0[Z] * bisectXYZ1[X] - bisectXYZ0[X] * bisectXYZ1[Z]),
                   0.25 * (bisectXYZ0[X] * bisectXYZ1[Y] - bisectXYZ0[Y] * bisectXYZ1[X]));

    var rxyz: 8*(3*real); //results

    (rxyz[n1], rxyz[n2], rxyz[n3], rxyz[n4]) = (areaXYZ, areaXYZ, areaXYZ, areaXYZ);

    return rxyz;
  }

  //calculate total normal from each face (faces are made up of combinations of nodes)
  pfxyz = ElemFaceNormal(0,1,2,3) + ElemFaceNormal(0,4,5,1) +
          ElemFaceNormal(1,5,6,2) + ElemFaceNormal(2,6,7,3) +
          ElemFaceNormal(3,7,4,0) + ElemFaceNormal(4,7,6,5);
}


proc SumElemStressesToNodeForces(b_xyz: 8*(3*real),
                                 stress_xxyz:3*real,
                                 ref fxyz: 8*(3*real)) {
  for param i in 0..7 do
    fxyz[i] = -(stress_xxyz * b_xyz[i]);
}

proc CalcElemVolumeDerivative(xyz: 8*(3*real)) {

  proc VoluDer(param n0, param n1, param n2, param n3, param n4, param n5) {
    var oxyz: 3*real;
    oxyz = ((xyz[n1][Y] + xyz[n2][Y]) * (xyz[n0][Z] + xyz[n1][Z]) - (xyz[n0][Y] + xyz[n1][Y]) * (xyz[n1][Z] + xyz[n2][Z]) +
           (xyz[n0][Y] + xyz[n4][Y]) * (xyz[n3][Z] + xyz[n4][Z]) - (xyz[n3][Y] + xyz[n4][Y]) * (xyz[n0][Z] + xyz[n4][Z]) -
            (xyz[n2][Y] + xyz[n5][Y]) * (xyz[n3][Z] + xyz[n5][Z]) + (xyz[n3][Y] + xyz[n5][Y]) * (xyz[n2][Z] + xyz[n5][Z]),
    - (xyz[n1][X] + xyz[n2][X]) * (xyz[n0][Z] + xyz[n1][Z]) + (xyz[n0][X] + xyz[n1][X]) * (xyz[n1][Z] + xyz[n2][Z]) -
           (xyz[n0][X] + xyz[n4][X]) * (xyz[n3][Z] + xyz[n4][Z]) + (xyz[n3][X] + xyz[n4][X]) * (xyz[n0][Z] + xyz[n4][Z]) +
            (xyz[n2][X] + xyz[n5][X]) * (xyz[n3][Z] + xyz[n5][Z]) - (xyz[n3][X] + xyz[n5][X]) * (xyz[n2][Z] + xyz[n5][Z]),
    - (xyz[n1][Y] + xyz[n2][Y]) * (xyz[n0][X] + xyz[n1][X]) + (xyz[n0][Y] + xyz[n1][Y]) * (xyz[n1][X] + xyz[n2][X]) -
           (xyz[n0][Y] + xyz[n4][Y]) * (xyz[n3][X] + xyz[n4][X]) + (xyz[n3][Y] + xyz[n4][Y]) * (xyz[n0][X] + xyz[n4][X]) +
            (xyz[n2][Y] + xyz[n5][Y]) * (xyz[n3][X] + xyz[n5][X]) - (xyz[n3][Y] + xyz[n5][Y]) * (xyz[n2][X] + xyz[n5][X]));

    return oxyz/12.0;
  }

  var dvdxyz: 8*(3*real);

  dvdxyz[0] = VoluDer(1,2,3,4,5,7);
  dvdxyz[3] = VoluDer(0,1,2,7,4,6);
  dvdxyz[2] = VoluDer(3,0,1,6,7,5);
  dvdxyz[1] = VoluDer(2,3,0,5,6,4);
  dvdxyz[4] = VoluDer(7,6,5,0,3,1);
  dvdxyz[5] = VoluDer(4,7,6,1,0,2);
  dvdxyz[6] = VoluDer(5,4,7,2,1,3);
  dvdxyz[7] = VoluDer(6,5,4,3,2,0);

  return dvdxyz;
}

inline proc CalcElemFBHourglassForce(xyzd: 8*(3*real),
                                     hourgam: 8*(4*real),
                                     coefficient: real,
                                     ref hgfxyz: 8*(3*real)) {
  var hxyz: 4*(3*real);

  // reduction
  for param i in 0..3 {
    for param j in 0..7 {
      hxyz[i] += hourgam(j)(i) * xyzd[j];
    }
  }

  for param i in 0..7 {
    var shxyz: 3*real;
    for param j in 0..3 do
      shxyz += hourgam(i)(j) * hxyz[j];
    hgfxyz[i] = coefficient * shxyz;
  }
}


proc CalcElemCharacteristicLength(xyz, volume) {
  proc AreaFace(param p0, param p1, param p2, param p3) {
    const fxyz = (xyz[p2] - xyz[p0]) - (xyz[p3] - xyz[p1]),
          gxyz = (xyz[p2] - xyz[p0]) + (xyz[p3] - xyz[p1]),
      area = (sumOfSquare(fxyz) * sumOfSquare(gxyz)) 
             - sumOfProduct(fxyz, gxyz)**2;

    return area ;
  }

  var charLength = max(AreaFace(0, 1, 2, 3),
                       AreaFace(4, 5, 6, 7),
                       AreaFace(0, 1, 5, 4),
                       AreaFace(1, 2, 6, 5),
                       AreaFace(2, 3, 7, 6),
                       AreaFace(3, 0, 4, 7));

  return 4.0 * volume / sqrt(charLength);
}


proc CalcElemVelocityGradient(xyzvel, pfxyz, detJ, ref d3: 3*real) {
  const inv_detJ = 1.0 / detJ;

  d3 = inv_detJ * ( pfxyz[0] * (xyzvel[0]-xyzvel[6])
                  + pfxyz[1] * (xyzvel[1]-xyzvel[7])
                  + pfxyz[2] * (xyzvel[2]-xyzvel[4])
                  + pfxyz[3] * (xyzvel[3]-xyzvel[5]) );

  /* Unused?  And now off-by-one in indexing (I was too lazy to update)

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
  */

  // TODO: These values seem unused?  Could reduce d to a 3-tuple
  /*
  d[6]  = 0.5 * ( dxddy + dyddx );
  d[5]  = 0.5 * ( dxddz + dzddx );
  d[4]  = 0.5 * ( dzddy + dyddz );
  */
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


inline proc computeDTF(indx) {
  const myvdov = vdov[indx];

  if myvdov == 0.0 then
    return max(real);

  const myarealg = arealg[indx];
  var dtf = ss[indx]**2;
  if myvdov < 0.0 then
    dtf += qqc2 * myarealg**2 * myvdov**2;
  dtf = sqrt(dtf);
  dtf = myarealg / dtf;

  return dtf;
}


proc CalcCourantConstraintForElems() {
  const val = min reduce [indx in MatElems] computeDTF(indx);

  if (val != max(real)) then
    dtcourant = val;
}


inline proc calcDtHydroTmp(indx) {
  const myvdov = vdov[indx];
  if (myvdov == 0.0) then
    return max(real);
  else
    return dvovmax / (abs(myvdov)+1.0e-20);
}


proc CalcHydroConstraintForElems() {
  const val = min reduce [indx in MatElems] calcDtHydroTmp(indx);

  if (val != max(real)) then
    dthydro = val;
}


/* calculate nodal forces, accelerations, velocities, positions, with
 * applied boundary conditions and slide surface considerations */

proc CalcForceForNodes() {
  //zero out all forces
  forall x in fx do x.write(0);
  forall y in fy do y.write(0);
  forall z in fz do z.write(0);

  /* Calcforce calls partial, force, hourq */
  CalcVolumeForceForElems();

  /* Calculate Nodal Forces at domain boundaries */
  // this was commented out in C++ code, so left out here
}

var sigxxyyzz: [Elems] 3*real, 
    determ: [Elems] real;

proc CalcVolumeForceForElems() {

  /* Sum contributions to total stress tensor */
  InitStressTermsForElems(p, q, sigxxyyzz);

  /* call elemlib stress integration loop to produce nodal forces from
     material stresses. */
  IntegrateStressForElems(sigxxyyzz, determ);

  /* check for negative element volume */
  forall e in Elems {
    if determ[e] <= 0.0 then
      halt("can't have negative volume (determ[", e, "]=", determ[e], ")");
  }

  CalcHourglassControlForElems(determ);
}


proc IntegrateStressForElems(sigxxyyzz, determ) {
  forall k in Elems {
    var b_xyz: 8*(3*real);
    var xyz_local: 8*(3*real);
    localizeNeighborNodes(k, xyz, xyz_local);

    var fxyz_local: 8*(3*real);

    local {
      /* Volume calculation involves extra work for numerical consistency. */
      CalcElemShapeFunctionDerivatives(xyz_local, b_xyz, determ[k]);
    
      CalcElemNodeNormals(b_xyz, xyz_local);

      SumElemStressesToNodeForces(b_xyz, sigxxyyzz[k], fxyz_local);
    }

    for (noi, t) in elemToNodesTuple(k) {
      fx[noi].add(fxyz_local[t][X]);
      fy[noi].add(fxyz_local[t][Y]);
      fz[noi].add(fxyz_local[t][Z]);
    }
  }
}

var dvdxyz, xyz8n: [Elems] 8*(3*real);

proc CalcHourglassControlForElems(determ) {

  forall eli in Elems {
    /* Collect domain nodes to elem nodes */
    var xyz1: 8*(3*real);
    localizeNeighborNodes(eli, xyz, xyz1);
    var pfxyz: 8*(3*real);

    local {
      /* load into temporary storage for FB Hour Glass control */
      dvdxyz[eli] = CalcElemVolumeDerivative(xyz1);
    }

    xyz8n[eli]  = xyz1;

    determ[eli] = volo[eli] * v[eli];
  }

  /* Do a check for negative volumes */
  forall e in Elems {
    if v[e] <= 0.0 then
      halt("can't have negative (or zero) volume. (in Hourglass, v[", e, "]=", v[e], ")");
  }

  if hgcoef > 0.0 {
    CalcFBHourglassForceForElems(determ, xyz8n, dvdxyz);
  }
}


const gammaCoef: 4*(8*real) = // WAS: [1..4, 1..8] real = 
                (( 1.0,  1.0, -1.0, -1.0, -1.0, -1.0,  1.0,  1.0),
                 ( 1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0, -1.0),
                 ( 1.0, -1.0,  1.0, -1.0,  1.0, -1.0,  1.0, -1.0),
                 (-1.0,  1.0, -1.0,  1.0,  1.0, -1.0,  1.0, -1.0));

/* Calculates the Flanagan-Belytschko anti-hourglass force. */
proc CalcFBHourglassForceForElems(determ, xyz8n, dvdxyz) {

  /* compute the hourglass modes */
  forall eli in Elems {
    var hourgam: 8*(4*real);
    var volinv = 1.0 / determ[eli];
    var ss1, mass1, volume13: real;
    var hgfxyz: 8*(3*real);
    var coefficient: real;

    var xyzd1: 8*(3*real);

    /*
    for param i in 0..nodesPerElem-1 {
      const noi = elemToNode[eli][i];
    
      xd1[i] = xd[noi];
      yd1[i] = yd[noi];
      zd1[i] = zd[noi];
    }
    */

    localizeNeighborNodes(eli, xyzd, xyzd1);

    //    startCommDiagnostics();
    local {
      for param i in 0..3 {
        var hourmodxyz: 3*real;
        // reduction
        for param j in 0..7 {
          hourmodxyz += xyz8n[eli][j] * gammaCoef[i][j];
        }

        for param j in 0..7 {
          hourgam[j][i] = gammaCoef[i][j] - volinv * 
            (dvdxyz[eli][j][X] * hourmodxyz[X] +
             dvdxyz[eli][j][Y] * hourmodxyz[Y] +
             dvdxyz[eli][j][Z] * hourmodxyz[Z]);
        }
      }

      /* compute forces */
      /* store forces into h arrays (force arrays) */
      ss1 = ss[eli];
      mass1 = elemMass[eli];
      volume13 = cbrt(determ[eli]);

      coefficient = - hgcoef * 0.01 * ss1 * mass1 / volume13;

      CalcElemFBHourglassForce(xyzd1, hourgam, coefficient, hgfxyz);
    } // end local
    //    stopCommDiagnostics();
    //    writeln(getCommDiagnostics());

    const myElemToNode = elemToNode[eli];
    for param i in 0..nodesPerElem-1 {
      const noi = myElemToNode[i];

      fx[noi].add(hgfxyz[i][X]);
      fy[noi].add(hgfxyz[i][Y]);
      fz[noi].add(hgfxyz[i][Z]);
    }
  }
}


proc CalcAccelerationForNodes() {
  forall noi in Nodes do local {
      const mass = nodalMass[noi];
      xyzdd[noi] = (fx[noi].read() / mass, fy[noi].read() / mass, fz[noi].read() / mass);
    }
}


proc ApplyAccelerationBoundaryConditionsForNodes() {
  // TODO: Shouldn't we be able to write these as follows?
  //
  // xdd[XSym] = 0.0;
  // ydd[YSym] = 0.0;
  // zdd[ZSym] = 0.0;
  
  forall x in XSym do xyzdd[x][X] = 0.0;
  forall y in YSym do xyzdd[y][Y] = 0.0;
  forall z in ZSym do xyzdd[z][Z] = 0.0;
}


proc CalcVelocityForNodes(dt: real, u_cut: real) {
  forall i in Nodes do local {
    var xyzdtmp = xyzd[i] + xyzdd[i] * dt;
    if abs(xyzdtmp[X]) < u_cut then xyzdtmp[X] = 0.0;
    if abs(xyzdtmp[Y]) < u_cut then xyzdtmp[Y] = 0.0;
    if abs(xyzdtmp[Z]) < u_cut then xyzdtmp[Z] = 0.0;
    xyzd[i] = xyzdtmp;
  }
}


proc CalcPositionForNodes(dt: real) {
  forall ijk in Nodes do
    xyz[ijk] += xyzd[ijk] * dt;
}

  var dxxyyzz: [Elems] 3*real;

proc CalcLagrangeElements() {

  CalcKinematicsForElems(dxxyyzz, deltatime);

  // element loop to do some stuff not included in the elemlib function.
  forall k in Elems do local {
    vdov[k] = sum(dxxyyzz[k]);
    var vdovthird = vdov[k] / 3.0;
    dxxyyzz[k] -= vdovthird;
  }

  // See if any volumes are negative, and take appropriate action.
  forall e in Elems {
    if vnew[e] <= 0.0 then
      halt("can't have negative volume (vnew[", e, "]=", vnew[e], ")");
  }
}


proc CalcKinematicsForElems(dxxyyzz, const dt: real) {
  // loop over all elements
  forall k in Elems {
    var b_xyz: 8*(3*real),
        d3: 3*real,
        detJ: real;
    var volume, relativeVolume: real;

    //get nodal coordinates from global arrays and copy into local arrays
    var xyz_local: 8*(3*real);
    localizeNeighborNodes(k, xyz, xyz_local);

    //get nodal velocities from global arrays and copy into local arrays
    var xyzd_local: 8*(3*real);
    localizeNeighborNodes(k, xyzd, xyzd_local);
    var dt2 = 0.5 * dt; //wish this was local, too...

    local {
      //volume calculations
      volume = CalcElemVolume(xyz_local);
      relativeVolume = volume / volo[k];
      vnew[k] = relativeVolume;
      delv[k] = relativeVolume - v[k];

      //set characteristic length
      arealg[k] = CalcElemCharacteristicLength(xyz_local, volume);

      for param i in 0..7 {
        xyz_local[i] -= dt2 * xyzd_local[i];
      }

      CalcElemShapeFunctionDerivatives(xyz_local, b_xyz, detJ);

      CalcElemVelocityGradient(xyzd_local, b_xyz, detJ, d3);

    }

    // put velocity gradient quantities into their global arrays.
    dxxyyzz[k] = d3;
  }
}


// sungeun: Temporary array reused throughout
/* velocity gradient */
var delv_xi, delv_eta, delv_zeta: [Elems] real;
/* position gradient */
var delx_xi, delx_eta, delx_zeta: [Elems] real;

proc CalcQForElems() {
  // MONOTONIC Q option

  /* Calculate velocity gradients */
  CalcMonotonicQGradientsForElems(delv_xi, delv_eta, delv_zeta, 
                                  delx_xi, delx_eta, delx_zeta);

  /* Transfer velocity gradients in the first order elements */
  /* problem->commElements->Transfer(CommElements::monoQ) ; */
  CalcMonotonicQForElems(delv_xi, delv_eta, delv_zeta,
                         delx_xi, delx_eta, delx_zeta);

  /* Don't allow excessive artificial viscosity */
  forall e in Elems {
    if q[e] > qstop then
      halt("Excessive artificial viscosity!  (q[", e, "]=", q[e], ")");
  }
}


// sungeun: Temporary array reused throughout
var vnewc: [MatElems] real;

/* Expose all of the variables needed for material evaluation */
proc ApplyMaterialPropertiesForElems() {

  forall i in MatElems do vnewc[i] = vnew[i];

  if eosvmin != 0.0 then
    [c in vnewc] if c < eosvmin then c = eosvmin;

  if eosvmax != 0.0 then
    [c in vnewc] if c > eosvmax then c = eosvmax;


  // old comment: The following loop should compute min/max reductions;
  // currently, race-y

  forall matelm in MatElems {
    var vc = v[matelm];
    if eosvmin != 0.0 && vc < eosvmin then vc = eosvmin;
    if eosvmax != 0.0 && vc > eosvmax then vc = eosvmax;
    if vc <= 0.0 {
      writeln("Volume error (in ApplyMaterialPropertiesForElems).");
      exit(1);
    }
  }

  EvalEOSForElems(vnewc);
}


proc UpdateVolumesForElems() {
  forall i in Elems do local {
    var tmpV = vnew[i];
    if abs(tmpV-1.0) < v_cut then tmpV = 1.0;
    v[i] = tmpV;
  }
}


proc CalcMonotonicQGradientsForElems(delv_xi, delv_eta, delv_zeta, 
                                     delx_xi, delx_eta, delx_zeta) {
  forall eli in Elems {
    const ptiny = 1.0e-36;
    var xyzl: 8*(3*real);
    localizeNeighborNodes(eli, xyz, xyzl);
    var xyzvl: 8*(3*real);
    localizeNeighborNodes(eli, xyzd, xyzvl);

    local {
      var vol = volo[eli] * vnew[eli],
          norm = 1.0 / (vol + ptiny);
      var axyz, dxyzv: 3*real;

      var dxyzj = -0.25 * ((xyzl[0]+xyzl[1]+xyzl[5]+xyzl[4]) - (xyzl[3]+xyzl[2]+xyzl[6]+xyzl[7])),
      
          dxyzi =  0.25 * ((xyzl[1]+xyzl[2]+xyzl[6]+xyzl[5]) - (xyzl[0]+xyzl[3]+xyzl[7]+xyzl[4])),
      
          dxyzk =  0.25 * ((xyzl[4]+xyzl[5]+xyzl[6]+xyzl[7]) - (xyzl[0]+xyzl[1]+xyzl[2]+xyzl[3]));

      /* find delvk and delxk ( i cross j ) */

      axyz = (dxyzi[Y]*dxyzj[Z] - dxyzi[Z]*dxyzj[Y],
              dxyzi[Z]*dxyzj[X] - dxyzi[X]*dxyzj[Z],
              dxyzi[X]*dxyzj[Y] - dxyzi[Y]*dxyzj[X]);

      delx_zeta[eli] = vol / sqrt(sumOfSquare(axyz) + ptiny);

      axyz *= norm;

      dxyzv = 0.25 * ((xyzvl[4]+xyzvl[5]+xyzvl[6]+xyzvl[7]) - (xyzvl[0]+xyzvl[1]+xyzvl[2]+xyzvl[3]));

      delv_zeta[eli] = sumOfProduct(axyz,dxyzv);

      /* find delxi and delvi ( j cross k ) */

      axyz = (dxyzj[Y]*dxyzk[Z] - dxyzj[Z]*dxyzk[Y],
              dxyzj[Z]*dxyzk[X] - dxyzj[X]*dxyzk[Z],
              dxyzj[X]*dxyzk[Y] - dxyzj[Y]*dxyzk[X]);

      delx_xi[eli] = vol / sqrt(sumOfSquare(axyz) + ptiny);

      axyz *= norm;

      dxyzv = 0.25 * ((xyzvl[1]+xyzvl[2]+xyzvl[6]+xyzvl[5]) - (xyzvl[0]+xyzvl[3]+xyzvl[7]+xyzvl[4]));

      delv_xi[eli] = sumOfProduct(axyz,dxyzv);

      /* find delxj and delvj ( k cross i ) */

      axyz = (dxyzk[Y]*dxyzi[Z] - dxyzk[Z]*dxyzi[Y],
              dxyzk[Z]*dxyzi[X] - dxyzk[X]*dxyzi[Z],
              dxyzk[X]*dxyzi[Y] - dxyzk[Y]*dxyzi[X]);

      delx_eta[eli] = vol / sqrt(sumOfSquare(axyz) + ptiny);

      axyz *= norm;

      dxyzv = -0.25 * ((xyzvl[0]+xyzvl[1]+xyzvl[5]+xyzvl[4]) - (xyzvl[3]+xyzvl[2]+xyzvl[6]+xyzvl[7]));

      delv_eta[eli] = sumOfProduct(axyz, dxyzv);
    } /* local */
  } /* forall eli */
}


proc CalcMonotonicQForElems(delv_xi, delv_eta, delv_zeta, 
                            delx_xi, delx_eta, delx_zeta) {
  //got rid of call through to "CalcMonotonicQRegionForElems"

  forall i in MatElems {
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

    if delvm  < phieta                        then phieta = delvm;
    if delvp  < phieta                        then phieta = delvp;
    if phieta < 0.0                                then phieta = 0.0;
    if phieta > monoq_max_slope        then phieta = monoq_max_slope;

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

    if delvm   < phizeta          then phizeta = delvm;
    if delvp   < phizeta         then phizeta = delvp;
    if phizeta < 0.0                 then phizeta = 0.0;
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


  var e_old, delvc, p_old, q_old, compression, compHalfStep, 
    qq_old, ql_old, work, p_new, e_new, q_new, bvc, pbvc: [Elems] real;

proc EvalEOSForElems(vnewc) {
  const rho0 = refdens;

  /* compress data, minimal set */
  forall i in MatElems {
    e_old[i]  = e[i];
    delvc[i]  = delv[i];
    p_old[i]  = p[i];
    q_old[i]  = q[i];
    qq_old[i] = qq[i];
    ql_old[i] = ql[i];
  }

  forall i in Elems do local {
    compression[i] = 1.0 / vnewc[i] - 1.0;
    var vchalf = vnewc[i] - delvc[i] * 0.5;
    compHalfStep[i] = 1.0 / vchalf - 1.0;
  }

  /* Check for v > eosvmax or v < eosvmin */
  // (note: I think this was already checked for in calling function!)
  if eosvmin != 0.0 {
    forall i in Elems {
      if vnewc[i] <= eosvmin then compHalfStep[i] = compression[i];
    }
  }
  if eosvmax != 0.0 {
    forall i in Elems {
      if vnewc[i] >= eosvmax {
        p_old[i] = 0.0;
        compression[i] = 0.0;
        compHalfStep[i] = 0.0;
      }
    }
  }

  CalcEnergyForElems(p_new, e_new, q_new, bvc, pbvc, 
                     p_old, e_old, q_old, compression, compHalfStep, 
                     vnewc, work, delvc, qq_old, ql_old);

  forall i in MatElems {
    p[i] = p_new[i];
    e[i] = e_new[i];
    q[i] = q_new[i];
  }

  CalcSoundSpeedForElems(vnewc, rho0, e_new, p_new, pbvc, bvc);
}


  var pHalfStep: [MatElems] real;

proc CalcEnergyForElems(p_new, e_new, q_new, bvc, pbvc,
                        p_old, e_old, q_old, compression, compHalfStep, 
                        vnewc, work, delvc, qq_old, ql_old) {
  // TODO: might need to move these consts into foralls or global
  // Otherwise, they live on Locale0 and everyone else has to do 
  // remote reads.  OR: Check if they're remote value forwarded.
  const rho0 = refdens; 
  const sixth = 1.0 / 6.0;

  forall i in Elems {
    e_new[i] = e_old[i] - 0.5 * delvc[i] * (p_old[i] + q_old[i]) + 0.5 * work[i];
    if e_new[i] < emin then e_new[i] = emin;
  }

  CalcPressureForElems(pHalfStep, bvc, pbvc, e_new, compHalfStep, 
                       vnewc, pmin, p_cut, eosvmax);

  forall i in Elems {
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
  forall i in Elems {
    e_new[i] += 0.5 * work[i];
    if abs(e_new[i] < e_cut) then e_new[i] = 0.0;
    if e_new[i] < emin then e_new[i] = emin;
  }

  CalcPressureForElems(p_new, bvc, pbvc, e_new, compression, vnewc, pmin, p_cut,
                       eosvmax);

  forall i in Elems {
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


  forall i in Elems do local {
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
  // TODO: Open question: If we had multiple materials, should (a) ss
  // be zeroed and accumulated into, and (b) updated atomically to
  // avoid losing updates?  (Jeff will go back and think on this)
  //
  forall i in MatElems {
    var ssTmp = (pbvc[i] * enewc[i] + vnewc[i]**2 * bvc[i] * pnewc[i]) / rho0;
    if ssTmp <= 1.111111e-36 then ssTmp = 1.111111e-36;
    ss[i] = sqrt(ssTmp);
  }
}


iter elemToNodes(elem) {
  for param i in 0..nodesPerElem-1 do
    yield elemToNode[elem][i];
}
                                 
iter elemToNodesTuple(e) {
  for i in 0..#nodesPerElem do
    yield (elemToNode[e][i], i);
}


proc deprint(title:string, x:[?D] real, y:[D]real, z:[D]real) {
  writeln(title);
  for i in D {
    writef("%3n %3.4er %3.4er %3.4er\n", i, x[i], y[i], z[i]);
  }
}

proc deprint(title:string, xyz:[?D] 3*real) {
  writeln(title);
  for i in D {
    writef("%3n %3.4er %3.4er %3.4er\n", i, xyz[i][X], xyz[i][Y], xyz[i][Z]);
  }
}


