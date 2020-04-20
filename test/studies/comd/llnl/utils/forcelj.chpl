use helpers;
use setup;
use force;
use VisualDebug;

class ForceLJ : Force {
  const POT_SHIFT : real = 1.0;

  var sigma : real;
  var epsilon : real;

  var sigma2 : real;
  var s6 : real;
  var rCut6 : real;
  var eShift : real;
  var epsilon2 : real;
  var epsilon4 : real;

  proc init() {
    const sigma = 2.315;
    const locCutoff = 2.5*sigma;

    var locCutoff2 = locCutoff * locCutoff;
    super.init(cutoff = locCutoff,
               mass = 63.55 * amuToInternalMass,
               lat = 3.615,
               latticeType = "FCC",
               name = "Cu",
               atomicNumber = 29,
               potName = "Lennard-Jones",
               cutoff2 = locCutoff2);
    this.sigma = sigma;
    epsilon  = 0.167;
    sigma2 = sigma * sigma;
    s6 = sigma2*sigma2*sigma2;
    rCut6 = s6 / (locCutoff2*locCutoff2*locCutoff2);
    eShift = POT_SHIFT * rCut6 * (rCut6 - 1.0);
    epsilon2 = 2.0 * epsilon;
    epsilon4 = 4.0 * epsilon;
  }

  inline proc compute(a : real3, b : real3, inout fij, inout pij) {
    var r = a - b;
    var d2 = dot(r, r);

    if( d2 > cutoff2 || d2 <= 0.0 ) then return;

    var r2 = 1/d2;
    var rs6 = s6 * (r2*r2*r2);

    fij += (epsilon4 * rs6 * r2 * (12.0*rs6 - 6.0)) * r;
    pij += epsilon2 * (rs6 * (rs6 - 1.0) - eShift);
  }

  override proc compute() : void {
if useChplVis then tagVdebug("computeLJForce");
    coforall ijk in locDom {
      on locGrid[ijk] {
        const MyDom = Grid[ijk]!;
        const force = MyDom.force : ForceLJ;
        const neighs = {-1..1, -1..1, -1..1};
        coforall (box, f, pe, boxIdx) in zip(MyDom.cells[MyDom.localDom], MyDom.f, MyDom.pe, MyDom.localDom) {
          for i in 1..box.count {
            f(i)  = (0.0, 0.0, 0.0);
            pe(i) = 0.0;
          }

          for n in neighs {
            const ref nBox = MyDom.cells[boxIdx + n];
            for i in 1..box.count {
              var fi:real3, pi:real;
              for j in 1..nBox.count {
                force.compute(box.atoms(i).r, nBox.atoms(j).r, fi, pi);
              }
              f(i)  += fi;
              pe(i) += pi;
            }
          }
        }
      }
    }
if useChplVis then pauseVdebug();
  }

  override proc computeLocal() : void {
if useChplVis then tagVdebug("computeLJForce");
    coforall ijk in locDom {
      on locGrid[ijk] {
        const MyDom = Grid[ijk]!;
        const force = MyDom.force : ForceLJ;
local {
        const neighs = {-1..1, -1..1, -1..1};
        coforall (box, f, pe, boxIdx) in zip(MyDom.cells[MyDom.localDom], MyDom.f, MyDom.pe, MyDom.localDom) {
          for i in 1..box.count {
            f(i)  = (0.0, 0.0, 0.0);
            pe(i) = 0.0;
          }

          for n in neighs {
            const ref nBox = MyDom.cells[boxIdx + n];
            for i in 1..box.count {
              var fi:real3, pi:real;
              for j in 1..nBox.count {
                force.compute(box.atoms(i).r, nBox.atoms(j).r, fi, pi);
              }
              f(i)  += fi;
              pe(i) += pi;
            }
          }
        }
}
      }
    }
if useChplVis then pauseVdebug();
  }
    
  override proc print() : void {
    writeln("Potential Data:");
    writeln("   Potential type   : ", potName);
    writeln("   Species name     : ", name);
    writeln("   Atomic number    : ", atomicNumber);
    writeln("   Mass             : ", mass/amuToInternalMass, " amu");
    writeln("   Lattice type     : ", latticeType);
    writeln("   Lattice spacing  : ", lat, " Angstroms");
    writeln("   Cutoff           : ", cutoff, " Angstroms");
    writeln("   Epsilon          : ", epsilon, " eV");
    writeln("   Sigma            : ", sigma, " Angstroms");
  }

  override proc replicate() : unmanaged ForceLJ? {
    var temp = new unmanaged ForceLJ();
    temp.cutoff = this.cutoff;
    temp.mass = this.mass;
    temp.lat = this.lat;
    temp.latticeType = this.latticeType;
    temp.name = this.name;
    temp.atomicNumber = this.atomicNumber;
    temp.potName = this.potName;
    temp.sigma = this.sigma;
    temp.epsilon = this.epsilon;
    temp.sigma2 = this.sigma2;
    temp.cutoff2 = this.cutoff2;
    temp.s6 = this.s6;
    temp.rCut6 = this.rCut6;
    temp.eShift = this.eShift;
    temp.epsilon2 = this.epsilon2;
    temp.epsilon4 = this.epsilon4;
    return temp;
  }
}
