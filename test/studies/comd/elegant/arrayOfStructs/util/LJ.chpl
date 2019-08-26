use Configs;
use Potential;
use Simulation;

class PotentialLJ : BasePotential {
  var sigma   : real;
  var epsilon : real;

  proc init() {
    this.complete();
    sigma   = 2.315;
    epsilon = 0.167;
    mass    = 63.55 * amuToInternalMass;

    lat         = 3.615;
    latticeType = "FCC";
    cutoff      = 2.5 * sigma;
    name        = "Cu";
    atomicNo    = 29;
  }

  override proc print() {
    writeln("  Potential type   : Lennard-Jones");
    writeln("  Species name     : ", name);
    writeln("  Atomic number    : ", atomicNo);
    writef ("  Mass             : %r amu\n", mass / amuToInternalMass);
    writeln("  Lattice Type     : ", latticeType);
    writef ("  Lattice spacing  : %r Angstroms\n", lat);
    writef ("  Cutoff           : %r Angstroms\n", cutoff);
    writef ("  Epsilon          : %r eV\n", epsilon);
    writef ("  Sigma            : %r Angstroms\n", sigma);
  }

  override proc force() {
    const cut2 = cutoff**2;

    var ePot = 0.0;
    ePotential = 0.0;

    forall a in allAtoms() {
      a.f = (0.0, 0.0, 0.0);
      a.U = 0.0;
    }

    const s6 = sigma**6;
    const cut6 = s6 / (cut2**3);
    const eShift = POT_SHIFT * cut6 * (cut6 - 1.0);
    const eps4 = -4.0 * epsilon;

    forall (boxID, centerBox) in zip(Boxes.domain, Boxes) with (+ reduce ePot) do local {
      // TODO: hoist if we can serialize to avoid comms
      const Offsets = {-1..1, -1..1, -1..1};
      for off in Offsets {
        ref neigh = Boxes.localAccess[boxID + off];

        for a in centerBox.liveAtoms() {
          for n in neigh.liveAtoms() {
            const dr = a.r - n.r;
            const r2 = dot(dr, dr);

            if r2 <= cut2 && r2 > 0.0 {
              const inv = 1.0 / r2;
              const r6 = s6 * (inv**3);
              const eLocal = r6 * (r6 - 1.0) - eShift;

              const incr = 0.5 * eLocal;
              a.U += incr;
              ePot += incr;

              const fr = eps4 * r6 * inv * (12.0 * r6 - 6.0);
              a.f -= dr * fr;
            }
          }
        }
      }
    }

    ePot *= 4.0 * epsilon;
    ePotential = ePot;
  }
}
