// All configuration parameters and constants should go here

config const usage = "";                              // usage
config const potDir = "pots";                         // potential directory
config const potName = "Cu_u6.eam";                   // potential name
config const potType = "funcfl";                      // potential type (funcfl or setfl)
config const doeam : int = 0;                         // compute eam potentials (default is LJ)
config const nx : int  = 20;                          // num of unit cells in x
config const ny : int  = 20;                          // num of unit cells in y
config const nz : int  = 20;                          // num of unit cells in z
config const xproc : int  = 1;                        // num of target locales in x
config const yproc : int  = 1;                        // num of target locales in y
config const zproc : int  = 1;                        // num of target locales in z
config const nSteps : int = 100;                      // total number of time steps
config const printRate : int = 10;                    // number of steps between output
config const dt : real = 1;                           // timestep length (fs)
config const lat : real = -1.0;                       // lattice parameter (Angstroms)
config const temp : real = 600;                       // initial temperature (K)
config const delta: real = 0;                         // initial delta (Angstroms)
config const useChplVis : int = 0;                    // whether to use chplvis or not
config const visName = "MD-Vis-0";                    // name of the chplvis file

config const replicateForce : int = 1;                // clone the force object per domain
config param MAXATOMS = 64;                           // max atoms per link cell
const atomsPerBin : domain(1) = {1..MAXATOMS};        // domain of atoms per link cell
const nb : int(32) = 4;                               // number of atoms in the basis

const amuInKilograms : real = 1.660538921e-27;        // 1 amu in kilograms
const fsInSeconds : real = 1.0e-15;                   // 1 fs in seconds
const AngsInMeters : real = 1.0e-10;                  // 1 Ang in meters
const eVInJoules : real = 1.602176565e-19;            // 1 eV in Joules
const amuToInternalMass : real =                      // Internal mass units are eV * fs^2 / Ang^2
         amuInKilograms * AngsInMeters * AngsInMeters
         / (fsInSeconds * fsInSeconds  * eVInJoules);
const kB_eV : real = 8.6173324e-5;                    // Boltmann constant in eV's (eV/K)
const hartreeToEv : real = 27.21138505;               // Hartrees to eVs
const bohrToAngs : real = 0.52917721092;              // Bohrs to Angstroms

config const simpleTimings = false; // Prints timings in a simplified format
                                    // for Chapel's testing infrastructure
