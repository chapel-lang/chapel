
enum PotType {
  funcfl,
  setfl
}

type vec3    = 3*real;
type vec3int = 3*int;

config const potDir  = "pots";
config const potName = "Cu_u6.eam";
config const potType = PotType.funcfl;

config const doeam = false;

config const nxyz = 20;
config const nx   = nxyz,
             ny   = nxyz,
             nz   = nxyz;

config var   xproc = 1,
             yproc = 1,
             zproc = 1;

config const nSteps    = 100;
config const printRate = 10;
config const dt        = 1.0;
config const lat       = -1.0;
config const temp      = 600.0;
config const delta     = 0.0;

config const debug         = false;
config const debugProgress = false;

param amuInKilograms = 1.660538921e-27; // 1 amu in kilograms
param fsInSeconds    = 1.0e-15;         // 1 fs in seconds
param AngsInMeters   = 1.0e-10;         // 1 Ang in meters
param eVInJoules     = 1.602176565e-19; // 1 eV in Joules

// TODO: compile-time failure if we try and make this 'param'
// Internal mass units are eV * fs^2 / Ang^2
const amuToInternalMass : real =
      amuInKilograms * AngsInMeters * AngsInMeters
      / (fsInSeconds * fsInSeconds  * eVInJoules);

param kB_eV       = 8.6173324e-5;  // Boltmann paramant in eV's (eV/K)
param hartreeToEv = 27.21138505;   // Hartrees to eVs
param bohrToAngs  = 0.52917721092; // Bohrs to Angstroms

config param MAXATOMS  = 64;
config param POT_SHIFT = 1.0;

var ReshapedDom : domain(3);
var ReshapedLocales : [ReshapedDom] locale = setupReshapedLocales();

proc setupReshapedLocales() {
if xproc == 1 && yproc == 1 && zproc == 1 {
  use DSIUtil;
  const ranges = setupTargetLocRanges(3, Locales);
  xproc = ranges(1).size;
  yproc = ranges(2).size;
  zproc = ranges(3).size;
}
  if (xproc*yproc*zproc) != numLocales then
    halt("Number of locales must match xproc * yproc * zproc: ",
         xproc, " * ", yproc, " * ", zproc, " != ", numLocales);

  ReshapedDom     = {1..xproc, 1..yproc, 1..zproc};
  return reshape(Locales, ReshapedDom);
}


proc printConfigs() {
  writeln("Command Line Paramters:");
  writeln("  doeam               : ", doeam);
  writeln("  potDir              : ", potDir);
  writeln("  potName             : ", potName);
  writeln("  potType             : ", potType);
  writeln("  nx                  : ", nx);
  writeln("  ny                  : ", ny);
  writeln("  nz                  : ", nz);
  writeln("  xproc               : ", xproc);
  writeln("  yproc               : ", yproc);
  writeln("  zproc               : ", zproc);
  writeln("  Lattice constant    : ", lat, " Angstroms");
  writeln("  nSteps              : ", nSteps);
  writeln("  printRate           : ", printRate);
  writeln("  Time step           : ", dt, " fs");
  writeln("  Initial Temperature : ", temp, " K");
  writeln("  Initial Delta       : ", delta, " Angstroms");
  writeln("  Number of Locales   : ", numLocales);
  writeln();
}
