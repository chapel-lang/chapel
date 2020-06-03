use MDTypes;
use configs;

class Force {
  var cutoff : real;        // potential cutoff distance in Angstroms
  var mass : real;          // mass of atoms in internal units
  var lat : real;           // lattice spacing (in Angstroms) of unit cell
  var latticeType : string; // lattice type (FCC, BCC etc)
  var name : string;        // element name
  var atomicNumber : int;   // atomic number
  var potName : string;     // type of potential (lj/eam)

  var cutoff2 : real;

  proc compute() : void { }
  proc computeLocal() : void { }
  proc print() : void {}
  proc replicate() : unmanaged Force? { return nil; }
  proc epilogue() : void { }
}
