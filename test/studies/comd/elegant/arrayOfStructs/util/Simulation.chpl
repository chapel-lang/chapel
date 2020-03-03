use Math;
use AccumStencilDist;

use Configs;
use Potential;
public use Util;
use EAM, LJ;

record SpeciesData {
  var name     : string; // element name
  var atomicNo : int;    // atomic number
  var mass     : real;   // mass in internal units
}

record Atom {
  var gid : int;     // global unique ID
  var species : int; // species index

  var mass : real;

  var r : vec3; // positions
  var p : vec3; // momenta
  var f : vec3; // forces
  var U : real; // potential energy
}

record Box {
  var count : int;
  var atoms : [1..MAXATOMS] Atom;

  iter liveAtoms() ref {
    for i in 1..count do yield atoms[i];
  }

  iter readAtoms() const ref {
    for i in 1..count do yield atoms[i];
  }
}

proc Box.init=(const ref other: Box) {
  this.count = other.count;
  this.atoms = other.atoms;
}

proc =(ref A : Box, B : Box) {
  A.count = B.count;
  if A.count > 0 then
    A.atoms[1..A.count] = B.atoms[1..A.count];
}

proc +=(ref A : Box, B : Box) {
  const bc = B.count;
  if A.count + bc > MAXATOMS then halt("Attempting to add too many atoms (", bc, ") to Box with ", A.count, " atoms (maximum ", MAXATOMS, ")");

  const nextEmpty = A.count + 1;
  A.atoms[nextEmpty..#bc] = B.atoms[1..bc];
  A.count += bc;
}

Timers["total"].start();

timestampMessage("Starting Initialization");

const Pot: owned BasePotential = if doeam then (new owned PotentialEAM()): owned BasePotential
                                          else (new owned PotentialLJ()): owned BasePotential;

const Species = new SpeciesData(Pot.name, Pot.atomicNo, Pot.mass);

const (globalExtent, numBoxes, boxSize, latticeConstant) = computeBoxInfo(Pot.lat, Pot.cutoff);
const invBoxSize = 1.0/boxSize;

sanityCheck(latticeConstant);

const boxDom   = {1..numBoxes(1), 1..numBoxes(2), 1..numBoxes(3)};
const boxSpace = boxDom dmapped AccumStencil(boxDom, targetLocales=ReshapedLocales,
                                             fluff=(1,1,1), periodic=true);

var Boxes : [boxSpace] Box;

const globalMin = (0.0, 0.0, 0.0);
const globalMax = globalExtent;

var ePotential : real;
var eKinetic   : real;
var numAtoms   : int;

createFccLattice(latticeConstant);
setTemperature(Configs.temp);
randomDisplacements();

redistributeAtoms();

Timers["computeForce"].start();
  Pot.force();
Timers["computeForce"].stop();

kineticEnergy();

const initialNumAtoms = numAtoms;
const initialEnergy   = (ePotential + eKinetic) / numAtoms;

//
// END OF INITIALIZATION
//

proc printSimulationDataYaml() {
  writeln("Simulation data: ");
  writef("  Total atoms        : %i\n", numAtoms);
  writef("  Min global bounds  : [ %14.10dr, %14.10dr, %14.10dr ]\n", 0.0, 0.0, 0.0);
  writef("  Max global bounds  : [ %14.10dr, %14.10dr, %14.10dr ]\n", (...globalMax));
  writeln();

  const boxFactor  = boxSize / Pot.cutoff;
  const maxOcc     = max reduce Boxes.count; 
  const totalBoxes = * reduce numBoxes;

  writeln("Decomposition data: ");
  writef("  Locales            : %6i, %6i, %6i = %6i\n", xproc, yproc, zproc, numLocales);
  writef("  Total boxes        : %6i, %6i, %6i = %6i\n", (...numBoxes), totalBoxes);
  writef("  Box size           : [ %14.10dr, %14.10dr, %14.10dr ]\n", (...boxSize));
  writef("  Box factor         : [ %14.10dr, %14.10dr, %14.10dr ]\n", (...boxFactor));
  writef("  Max Link Cell Occupancy: %i of %i\n", maxOcc, MAXATOMS);
  writeln();

  writeln("Potential data: ");
  Pot.print();
  writeln();
  writeln();

  writef("Initial energy : %14.12dr, atom count : %i \n", initialEnergy, initialNumAtoms);
  writeln();
}

proc sanityCheck(LC) {
  const min = (xproc,yproc,zproc):vec3 * 2 * Pot.cutoff;
  const size = (nx,ny,nz):vec3 * LC;

  if size < min {
    writeln("Error: Simulation too small: ", size , " < ", min);
    writeln("Increase the number of unit cells");
    exit(1);
  }

  if Pot.latticeType.toLower() != "fcc" {
    halt("Only FCC Lattice type supported, not ", Pot.latticeType);
  }
}

proc getBoxFromCoords(r : vec3) : vec3int {
  return (r * invBoxSize + (1,1,1)):vec3int;
}

proc createFccLattice(lat : real) {
  const nb = 4; // number of atoms in the basis

  const Q = 0.25;
  const T = 0.75;
  const basis : [0..#nb] vec3 = [(Q, Q, Q),
                                (Q, T, T),
                                (T, Q, T),
                                (T, T, Q)];
  var end : vec3int;
  for i in 1..3 do end(i) = Math.ceil(globalExtent(i) / lat):int;

  const latticeDom = {0..#end(1), 0..#end(2), 0..#end(3)};

  inline proc inGlobalExtent(r : vec3) {
    for param i in 1..3 {
      if r(i) < 0.0 || r(i) > globalExtent(i) then return false;
    }
    return true;
  }

  for i in latticeDom {
    for j in basis.domain {
      const r = ((i:vec3) + basis[j]) * lat;

      const gid = j + nb * (i(3) + nz * (i(2) + ny * i(1)));
      const species = 0;

      if inGlobalExtent(r) == false then continue;

      const boxIdx = getBoxFromCoords(r);

      ref box = Boxes[boxIdx];
      box.count += 1;
      box.atoms[box.count] = new Atom(gid, species, Species.mass, r);
    }
  }

  numAtoms = + reduce Boxes.count;
}

proc sumAtoms() {
  Timers["commReduce"].start();
    numAtoms = + reduce Boxes.count;
  Timers["commReduce"].stop();
}

proc setTemperature(temperature) {
  forall a in allAtoms() {
    const sigma = sqrt(kB_eV * temperature/a.mass);
    const ms = a.mass * sigma;

    var seed = mkSeed(a.gid, 123);
    a.p = (ms * gasdev(seed), ms * gasdev(seed), ms * gasdev(seed));
  }

  if temperature == 0.0 then return;

  const vZero = (0.0, 0.0, 0.0);
  setVcm(vZero);

  kineticEnergy();

  const t     = eKinetic / numAtoms / kB_eV / 1.5;
  const scale = Math.sqrt(temperature/t);

  forall a in allAtoms() {
    a.p *= scale;
  }

  kineticEnergy();
}

// Computes center of mass velocity of the system
proc computeVcm() : vec3 {
  var ret : vec3;
  var mass : real;

  forall a in allAtoms() with (+ reduce ret, + reduce mass) {
    ret += a.p;
    mass += a.mass;
  }
  ret /= mass;

  return ret;
}

proc setVcm(newVcm) {
  const oldVcm = computeVcm();
  const shift = newVcm - oldVcm;

  forall a in allAtoms() {
    a.p += a.mass * shift;
  }
}

proc kineticEnergy() {
  var kenergy = 0.0;

  forall a in allAtoms() with (+ reduce kenergy) {
    const invMass = 0.5 / a.mass;
    kenergy += dot(a.p, a.p) * invMass;
  }

  eKinetic = kenergy;
}

proc randomDisplacements() {
  forall a in allAtoms() {
    var seed = mkSeed(a.gid, 457);
    for i in 1..3 do
      a.r(i) += (2.0 * lcg61(seed) - 1.0) * Configs.delta;
  }
}

proc redistributeAtoms() {
  Timers["redistribute"].start();

  RedistTimers["updateLinkCells"].start();
    updateLinkCells();
  RedistTimers["updateLinkCells"].stop();

  RedistTimers["atomHalo"].start();
    Boxes.updateFluff();

    RedistTimers["pbc"].start();
      pbc();
    RedistTimers["pbc"].stop();
  RedistTimers["atomHalo"].stop();

  RedistTimers["sort"].start();
    sortAtomsInCell();
  RedistTimers["sort"].stop();

  Timers["redistribute"].stop();
}

iter allAtoms() ref {
  for box in Boxes {
    for a in box.liveAtoms() {
      yield a;
    }
  }
}

iter allAtoms(param tag : iterKind) ref where tag == iterKind.leader {
  for follow in Boxes.these(iterKind.leader) {
    yield follow;
  }
}

iter allAtoms(param tag : iterKind, followThis) ref where tag == iterKind.follower {
  for box in Boxes.these(iterKind.follower, followThis) {
    for a in box.liveAtoms() do yield a;
  }
}

proc timestep(nSteps : int, dt : real) {
  for i in 1..nSteps {
    advanceVelocity(0.5 * dt);
    advancePosition(dt);
    redistributeAtoms();

    Timers["computeForce"].start();
      Pot.force();
    Timers["computeForce"].stop();

    advanceVelocity(0.5 * dt);
  }

  kineticEnergy();
}

proc advanceVelocity(dt : real) {
  Timers["velocity"].start();
  forall a in allAtoms() {
    a.p += dt * a.f;
  }
  Timers["velocity"].stop();
}

proc advancePosition(dt : real) {
  Timers["position"].start();
  forall a in allAtoms() {
    const invMass = 1.0 / a.mass;
    a.r += dt * a.p * invMass;
  }
  Timers["position"].stop();
}

proc updateLinkCells() {
  // 'empty' the cached boxes
  forall (box, dir) in Boxes.boundaries() {
    box.count = 0;
  }

  // Move atoms if their position changed
  // TODO: How can we avoid a coforall here?
  coforall loc in Locales do on loc {
    for (box, srcIdx) in zip(Boxes.localSlice(Boxes.localSubdomain()), Boxes.localSubdomain()) {
      var i = 1;
      var curCount = box.count;
      while i <= curCount {
        const destIdx = getBoxFromCoords(box.atoms[i].r);
        if srcIdx != destIdx {
          moveAtom(i, srcIdx, destIdx);
          curCount -= 1;
        } else {
          i += 1;
        }
      }
    }
  }
}

proc moveAtom(srcAtomIdx, srcIdx, destIdx) {
  ref destBox = Boxes.localAccess[destIdx];
  ref srcBox = Boxes.localAccess[srcIdx];

  const destAtomIdx = destBox.count + 1;
  destBox.atoms[destAtomIdx] = srcBox.atoms[srcAtomIdx];
  destBox.count += 1;
  srcBox.count -= 1;

  if srcBox.count > 0 {
    // Copy last atom in the box into the slot of the atom we just moved
    srcBox.atoms[srcAtomIdx] = srcBox.atoms[srcBox.count+1];
  }
}

proc pbc() {
  forall (box, dir) in Boxes.boundaries() {
    for a in box.liveAtoms() {
      const shift = dir * globalExtent;
      a.r += shift;
    }
  }
}

proc sortAtomsInCell() {
  use Sort;

  record cmp {
    proc compare(a, b) {
      return a.gid - b.gid;
    }
  }

  const c = new cmp();

  // TODO: seems like we need some kind of "fluff and all" iter
  forall box in Boxes {
    sort(box.atoms[1..box.count], comparator=c);
  }
  forall (box, _) in Boxes.boundaries() {
    sort(box.atoms[1..box.count], comparator=c);
  }
}
