use Configs;
use Potential;
use AccumStencilDist;
use Util;
use Simulation;

private use IO;

class InterpolationObject {
  var n      : int;           // number of values in table
  var x0     : real;          // starting ordinate range
  var dx     : real;          // table spacing
  var invDx  : real;
  var nSpace : domain(1);
  var values : [nSpace] real; // the abscissa values

  proc init(n, x0, dx, buf) {
    this.n      = n;
    this.x0     = x0;
    this.dx     = dx;
    this.invDx  = 1.0/dx;
    this.nSpace = {-1..n+1};

    this.complete();

    values[0..n-1] = buf;

    values[-1]     = values[0];
    values[n]      = values[n-1];
    values[n+1]    = values[n-1];
  }

  proc interpolate(in r : real, out f : real, out df : real) {
    if r < x0 then r = x0;

    r = (r - x0) * invDx;
    var ii = floor(r):int;
    if ii > n-1 {
      ii = n-1;
      r = n / invDx;
    }

    r -= floor(r);

    const g1 = values[ii+1] - values[ii-1];
    const g2 = values[ii+2] - values[ii];

    f = values[ii] + 0.5 * r * (g1 + r * (values[ii+1] + values[ii-1] - 2.0 * values[ii]));
    df = 0.5 * (g1 + r * (g2 - g1)) * invDx;
  }
}

class PotentialEAM : BasePotential {
  var phiIO, rhoIO, fIO : unmanaged InterpolationObject;

  var rhoDom : domain(3);
  var rhoSpace = rhoDom dmapped AccumStencil(rhoDom); // basically BlockDist
  var dfSpace  = rhoDom dmapped AccumStencil(rhoDom, fluff=(1,1,1), periodic=true);
  var rhoBar  : [rhoSpace] [1..MAXATOMS] real;
  var dfEmbed : [dfSpace] [1..MAXATOMS] real;

  proc init() {
    var info : unmanaged BasePotential?;
    var p, r, f : unmanaged InterpolationObject?;

    if potType == PotType.setfl then
      (info, p, r, f) = readSetFl();
    else if potType == PotType.funcfl then
      (info, p, r, f) = readFuncFl();
    else
      halt("Unsupported pot type: ", potType);

    super.init(info!);

    this.phiIO = p!;
    this.rhoIO = r!;
    this.fIO   = f!;

    const boxInfo  = computeBoxInfo(info!.lat, info!.cutoff);
    const numBoxes = boxInfo(1);
    rhoDom         = {1..numBoxes(0), 1..numBoxes(1), 1..numBoxes(2)};

    delete info;
  }

  proc deinit() {
    delete phiIO;
    delete rhoIO;
    delete fIO;
  }
  override proc print() {
    writeln("  Potential type  : EAM");
    writeln("  Species name    : ", name);
    writeln("  Atomic number   : ", atomicNo);
    writef ("  Mass            : %r amu\n", mass / amuToInternalMass);
    writeln("  Lattice type    : ", latticeType);
    writef ("  Lattice spacing : %r Angstroms\n", lat);
    writef ("  Cutoff          : %r Angstroms\n", cutoff);
  }
}

proc readSetFl() {
  var chan = open(potDir + "/" + potName, iomode.r);
  var r = chan.reader();

  var info = new unmanaged BasePotential();
  var pIO, rIO, fIO : unmanaged InterpolationObject?;

  // Skip comments
  r.readln();
  r.readln();
  r.readln();

  const (numSpecies, species) = r.readln(int, string);
  if numSpecies != 1 then halt("CoMD only supports 1 species, given ",
                               numSpecies, " in 'setfl' file.");
  info.name = species;

  const (nrho, drho, nr, dr, rcutoff) = r.readln(int, real, int, real, real);
  info.cutoff = rcutoff;

  (info.atomicNo, info.mass, info.lat, info.latticeType) = r.readln(int, real, real, string);
  info.mass *= amuToInternalMass;

  var buf : [1..#max(nrho, nr)] real;
  const x0 = 0.0;

  // Read embedding energy F(rhobar)
  for i in 1..nrho do buf[i] = r.read(real);
  fIO = new unmanaged InterpolationObject(nrho, x0, drho, buf);

  // Read electron density rho(r)
  for i in 1..nr do buf[i] = r.read(real);
  rIO = new unmanaged InterpolationObject(nr, x0, dr, buf);

  // Read phi(r)*r and convert to phi(r)
  for i in 1..nr do buf[i] = r.read(real);
  for i in 2..nr {
    const r = x0 + i * dr;
    buf[i] /= r;
  }
  buf[1] = buf[2] + (buf[2] - buf[3]);
  pIO = new unmanaged InterpolationObject(nr, x0, dr, buf);

  return (info, pIO!, rIO!, fIO!);
}

proc readFuncFl() {
  var chan = open(potDir + "/" + potName, iomode.r);
  var r = chan.reader();

  var info = new unmanaged BasePotential();
  var pIO, rIO, fIO : unmanaged InterpolationObject?;

  // Comments
  info.name = r.readln(string);

  (info.atomicNo, info.mass, info.lat, info.latticeType) = r.readln(int, real, real, string);
  info.mass *= amuToInternalMass;

  const (nrho, drho, nr, dr, rcutoff) = r.readln(int, real, int, real, real);
  info.cutoff = rcutoff;

  var buf : [1..#max(nrho, nr)] real;
  const x0 = 0.0;

  // Read embedding energy F(rhobar)
  for i in 1..nrho do buf[i] = r.read(real);
  fIO = new unmanaged InterpolationObject(nrho, x0, drho, buf);

  // Read Z(r) and conver to phi(r)
  for i in 1..nr do buf[i] = r.read(real);
  for i in 2..nr {
    const r = x0 + (i-1) * dr;
    buf[i] *= buf[i] / r;
    buf[i] *= hartreeToEv * bohrToAngs;
  }
  buf[1] = buf[2] + (buf[2] - buf[3]);
  pIO = new unmanaged InterpolationObject(nr, x0, dr, buf);

  for i in 1..nr do buf[i] = r.read(real);
  rIO = new unmanaged InterpolationObject(nr, x0, dr, buf);

  return (info, pIO!, rIO!, fIO!);
}

proc PotentialEAM.reset() {
  forall (box, dfBox, rhoBox) in zip(Boxes, dfEmbed, rhoBar) {
    for (a, d, r) in zip(box.liveAtoms(), dfBox, rhoBox) {
      a.f = (0.0, 0.0, 0.0);
      a.U = 0.0;
      d   = 0.0;
      r   = 0.0;
    }
  }

  [(d, _) in dfEmbed.boundaries()] d = 0.0;
  [(r, _) in rhoBar.boundaries()]  r = 0.0;
}

proc PotentialEAM.computePairWise() {
  var etot : real;
  const cut2 = cutoff ** 2;

  forall (boxID, centerBox, rhoBox, dfBox) in zip(Boxes.domain, Boxes, rhoBar, dfEmbed) with (+ reduce etot) {
    const Offsets = {-1..1, -1..1, -1..1};
    for off in Offsets {
      ref neighBox = Boxes.localAccess[boxID + off];
      for (a, rhobar) in zip(centerBox.liveAtoms(), rhoBox) {
        for n in neighBox.liveAtoms() {
          const dr = a.r - n.r;
          const r2 = dot(dr, dr);

          if r2 <= cut2 && r2 > 0.0 {
            const r = Math.sqrt(r2);
            var phiTmp, dPhi, rhoTmp, dRho : real;
            phiIO.interpolate(r, phiTmp, dPhi);
            rhoIO.interpolate(r, rhoTmp, dRho);
            a.f  -= dPhi * dr / r;
            a.U  += 0.5 * phiTmp;
            etot += 0.5 * phiTmp;

            rhobar += rhoTmp;
          }
        }
      }
    }
  }

  return etot;
}

proc PotentialEAM.computeEmbeddingEnergy() {
  var etot : real;

  forall (boxID, centerBox, rhoBox, dfBox) in
         zip(Boxes.domain, Boxes, rhoBar, dfEmbed) with (+ reduce etot) {
    for (a, rhobar, dfE) in zip(centerBox.liveAtoms(), rhoBox, dfBox) {
      var f, df : real;
      fIO.interpolate(rhobar, f, df);
      dfE  = df;
      a.U  += f;
      etot += f;
    }
  }

  return etot;
}

proc PotentialEAM.computeElectronCloud() {
  const cut2 = cutoff ** 2;
  forall (boxID, centerBox, rhoBox, dfBox) in
         zip(Boxes.domain, Boxes, rhoBar, dfEmbed) {
    const Offsets = {-1..1, -1..1, -1..1};
    for off in Offsets {
      ref neighBox = Boxes.localAccess[boxID + off];
      ref neighDF  = dfEmbed.localAccess[boxID + off];

      for (a, adf) in zip(centerBox.liveAtoms(), dfBox) {
        for (n, ndf) in zip(neighBox.liveAtoms(), neighDF) {
          const dr = a.r - n.r;
          const r2 = dot(dr, dr);

          if r2 <= cut2 && r2 > 0.0 {
            const r = Math.sqrt(r2);
            var rhoTmp, dRho : real;
            rhoIO.interpolate(r, rhoTmp, dRho);

            a.f -= (adf + ndf) * dRho * dr / r;
          }
        }
      }
    }
  }
}

override proc PotentialEAM.force() {
  reset();

  const pairWiseEnergy = computePairWise();

  const embeddingEnergy = computeEmbeddingEnergy();

  dfEmbed.updateFluff();

  computeElectronCloud();

  ePotential = pairWiseEnergy + embeddingEnergy;
}
