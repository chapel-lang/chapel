// defines the computation for LJ and EAM style computations

use initMD;
use Time;

// define the interface for a force object
class Force {
  var cutforcesq, eng_vdwl : real;
  var virial, mass : real;

  var wipetime, maintime: real;

  proc Force();
  proc compute(store : bool) : void {}
}

// 'Embedded atom model'
class ForceEAM : Force  {
  var numDensity, numPotentials : int;

  // potentials as array data
  var rSpace : domain(1);
  var frhoSpace : domain(1);
  var frho : [frhoSpace] real;
  var rhor, z2r : [rSpace] real;
  
  // spline form for computation
  var deltaPotential, rdr, deltaDensity, rdrho : real;
  var splineSpace : domain(1);
  var frSplineSpace : domain(1);
  var rzSplineSpace : domain(1);
  var rhor_spline, z2r_spline : [rzSplineSpace] real;
  var frho_spline : [frSplineSpace] real;

  class FPChunk {
    var Fluff : domain(3);
    var Fp : [Fluff] [perBinSpace] real;
  }

  var FP : [LocaleGridDom] FPChunk;
              
  class Funcfl {
    var eamFile : string;
    var numDensity, numPotentials : int;
    var deltaDensity, deltaPotential, cut, mass : real;
    var zrSpace : domain(1);
    var frhoSpace : domain(1);
    var frho : [frhoSpace] real;
    var rhor, zr : [zrSpace] real;
  } 

  var funcfl : Funcfl;

  proc ForceEAM(cf : real) {
    // use the fluff domain already calculated for communication
    coforall ijk in LocaleGridDom {
      on LocaleGrid[ijk] {
        FP[ijk] = new FPChunk(Grid[ijk].Fluff);
      }
    }
    cutforcesq = cf*cf;
    coeff("Cu_u6.eam");
    init_style();
  }

  proc coeff(fname : string) {
    funcfl = new Funcfl();
    funcfl.eamFile = fname;
    var fchan = open(fname, iomode.r);
    var rd = fchan.reader();

    rd.readln(); // skip header info
    rd.read(int); // skip atomic number

    mass = rd.readln(real);

    (funcfl.numDensity, 
     funcfl.deltaDensity, 
     funcfl.numPotentials, 
     funcfl.deltaPotential, 
     funcfl.cut) = rd.readln(int,real,int,real,real);

    funcfl.frhoSpace = {1..(funcfl.numDensity+1)};
    funcfl.zrSpace = {1..(funcfl.numPotentials+1)};
    grab(rd, funcfl.frho); // verified correctness
    grab(rd, funcfl.zr);
    grab(rd, funcfl.rhor);
    cutforcesq = funcfl.cut * funcfl.cut;
  }

  proc init_style() {
    file2array();
    array2spline();
  }

  proc file2array() {
    var i, j, k, m, n : int;
    var ntypes = 1;
    var sixth : real = 1.0 / 6.0;

    var active = 0;
    var rmax, rhomax : real;
    deltaPotential = 0;
    deltaDensity = 0;
    rmax = 0;
    rhomax = 0;

    deltaPotential = max(deltaPotential, funcfl.deltaPotential);
    deltaDensity = max(deltaDensity, funcfl.deltaDensity);
    rmax = max(rmax, (funcfl.numPotentials - 1) * funcfl.deltaPotential);
    rhomax = max(rhomax, (funcfl.numDensity - 1) * funcfl.deltaDensity);

    numPotentials = (rmax / deltaPotential + .5) : int;
    numDensity = (rhomax / deltaDensity + .5) : int;

    frhoSpace = {1..(numDensity+1)};

    var r, p, cof1, cof2, cof3, cof4 : real;

    for (f,m) in zip(frho[2..], 0.. ) {
      r = m * deltaDensity;
      p = r / funcfl.deltaDensity + 1.0;
      k = p : int;
      k = min(k, funcfl.numDensity-2);
      k = max(k, 2);
      p -= k;
      p = min(p, 2.0);
      cof1 = -sixth * p * (p-1.0) * (p-2.0);
      cof2 = 0.5 * (p * p - 1.0) * (p - 2.0);
      cof3 = -.5 * p * (p+1.0) * (p - 2.0);
      cof4 = sixth * p * (p * p - 1.0);
      k += 1; // get us to 1-based chapel arrays.
      f = cof1 * funcfl.frho[k-1] + cof2 * funcfl.frho[k] + cof3 * funcfl.frho[k + 1] + cof4 * funcfl.frho[k+2];
    } // verified correct

    rSpace = {1..(numPotentials+1)};
    for (f,m) in zip(rhor[2..], 0..) {
      r = m * deltaPotential;
      p = r / funcfl.deltaPotential + 1.0;
      k = p : int;
      k = min(k, funcfl.numPotentials - 2);
      k = max(k, 2);
      p -= k;
      p = min(p, 2.0);
      cof1 = -sixth * p * (p-1.0) * (p-2.0);
      cof2 = 0.5 * (p * p - 1.0) * (p - 2.0);
      cof3 = -.5 * p * (p+1.0) * (p - 2.0);
      cof4 = sixth * p * (p * p - 1.0);
      k += 1;
      f = cof1 * funcfl.rhor[k-1] + cof2 * funcfl.rhor[k] + cof3 * funcfl.rhor[k+1] + cof4 * funcfl.rhor[k+2];
    }

    for (f,m) in zip(z2r[2..], 0..) {
      r = m * deltaPotential;
      p = r / funcfl.deltaPotential + 1.0;
      k = p : int;
      k = min(k, funcfl.numPotentials - 2);
      k = max(k, 2);
      p -= k;
      p = min(p,2.0);
      cof1 = -sixth * p * (p - 1.0) * (p - 2.0);
      cof2 = 0.5 * (p * p - 1.0) * (p - 2.0);
      cof3 = -.5 * p * (p+1.0) * (p - 2.0);
      cof4 = sixth * p * (p * p - 1.0);
      k += 1;
      var zri : real = cof1 * funcfl.zr[k-1] + cof2 * funcfl.zr[k] + cof3 * funcfl.zr[k+1] + cof4 * funcfl.zr[k+2];
      f = 27.2 * 0.529 * zri * zri;
    } 
  }

  proc array2spline() {
    rdr = 1.0 / deltaPotential;
    rdrho = 1.0 / deltaDensity;

    frSplineSpace = {0..((numDensity+1) * 7)-1};
    rzSplineSpace = {0..((numPotentials+1) * 7)-1};

    interp(numDensity, deltaDensity, frho, frho_spline);
    interp(numPotentials, deltaPotential, rhor, rhor_spline);
    interp(numPotentials, deltaPotential, z2r, z2r_spline);
  }

  // copied straight from c++, not entirely sure what it does
  proc interp(n : int, delta : real, arr : [] real, spline : [] real) {
    for (f,m) in zip(arr[2..],1..) {
      spline[m*7 + 6] = f;
    }

    spline[1*7 + 5] = spline[2*7+6] - spline[1*7+6];
    spline[2 * 7 + 5] = 0.5 * (spline[3 * 7 + 6] - spline[1 * 7 + 6]);
    spline[(n - 1) * 7 + 5] = 0.5 * (spline[n * 7 + 6] - spline[(n - 2) * 7 + 6]);
    spline[n * 7 + 5] = spline[n * 7 + 6] - spline[(n - 1) * 7 + 6];

    for m in 3..n-2 {
      spline[m * 7 + 5] = ((spline[(m - 2) * 7 + 6] - spline[(m + 2) * 7 + 6]) +
        8.0 * (spline[(m + 1) * 7 + 6] - spline[(m - 1) * 7 + 6])) / 12.0;
    }

    for m in 1..n-1 {
       spline[m * 7 + 4] = 3.0 * (spline[(m + 1) * 7 + 6] - spline[m * 7 + 6]) -
         2.0 * spline[m * 7 + 5] - spline[(m + 1) * 7 + 5];
       spline[m * 7 + 3] = spline[m * 7 + 5] + spline[(m + 1) * 7 + 5] -
         2.0 * (spline[(m + 1) * 7 + 6] - spline[m * 7 + 6]);
    }

    spline[n*7 + 4] = 0.0;
    spline[n*7 + 3] = 0.0;

    for m in 1..n {
      spline[m * 7 + 2] = spline[m * 7 + 5] / delta;
      spline[m * 7 + 1] = 2.0 * spline[m * 7 + 4] / delta;
      spline[m * 7 + 0] = 3.0 * spline[m * 7 + 3] / delta;
    }
  }

  proc grab(rd,list : [] real) {
    for a in list[2..list.domain.high] {
      a = rd.read(real);
    }
    list[1] = list[2];
  }

  proc compute(store : bool) {
    if debug then writeln("entering EAM compute...");
    var evdwl, vir : atomic real;
    virial = 0.0;
    coforall ijk in LocaleGridDom {
      on LocaleGrid[ijk] {
        const Me = Grid[ijk];
        forall (b,c) in zip(Me.Bins,Me.Count[Me.Real]) {
          for a in b[1..c] {
            a.f = (0.0,0.0,0.0);
          }
        }
      }
    }

    coforall ijk in LocaleGridDom {
      on LocaleGrid[ijk] do {
        const Me = Grid[ijk];

        for (b,p,c,r) in zip(Me.Bins,Me.Pos[Me.Real], Me.Count[Me.Real], Me.Real) {
          const exist = 1..c;
          for (a,x,i) in zip(b[exist],p[exist], exist) {
            var rhoi : atomic real;
            forall (n,j) in a.neighs[1..a.ncount] {
              const del = x - Me.Pos[n][j];
              const rsq = dot(del, del);

              if rsq < cutforcesq {
                var p = sqrt(rsq) * rdr + 1.0;
                var m : int = p : int;
                if m >= numPotentials - 1 then m = numPotentials - 1;
                p -= m;
                if p >= 1.0 then p = 1.0;
                var val = ((rhor_spline[m*7 + 3] * p + rhor_spline[m * 7 + 4]) * p + rhor_spline[m*7+5]) * p + rhor_spline[m*7+6];
                rhoi.add(val);
              }
            }

            var p = 1.0 * rhoi.read() * rdrho + 1.0;
            var m = p : int;
            m = max(1, min(m, numDensity - 1));
            p -= m;
            p = min(p, 1.0);
            FP[ijk].Fp[r][i] = (frho_spline[m*7+0] * p + frho_spline[m*7 + 1]) * p + frho_spline[m*7 + 2];

            if store {
              evdwl.add(((frho_spline[m*7+3] * p + frho_spline[m*7+4]) * p + frho_spline[m*7+5]) * p + frho_spline[m*7+6]);
            }
          }
        }
      }
    }

    coforall ijk in LocaleGridDom {
      on LocaleGrid[ijk] {
        const Me = Grid[ijk];
        for (Src, Dest, neigh) in zip(Me.SrcSlice, Me.DestSlice, Me.Neighs) {
          FP[ijk].Fp[Dest] = FP[neigh].Fp[Src];
        }
      }
    }

    coforall ijk in LocaleGridDom {
      on LocaleGrid[ijk] {
        const Me = Grid[ijk];

        for (b,p,c,rbin) in zip(Me.Bins, Me.Pos[Me.Real], Me.Count[Me.Real], Me.Real) {
          const exist = 1..c;
          for (a,x,i) in zip(b[exist],p[exist],exist) {
            var fx, fy, fz : real;
            for (n,j) in a.neighs[1..a.ncount] {
              const del = x - Me.Pos[n][j];
              const rsq = dot(del, del);

              if rsq < cutforcesq {
                const r = sqrt(rsq);
                var p = r * rdr + 1.0;
                var m : int = p : int;
                if m >= numPotentials - 1 then m = numPotentials - 1;
                p -= m;
                if p >= 1.0 then p = 1.0;

                var rhoip = (rhor_spline[m * 7 + 0] * p + rhor_spline[m*7+1]) * p + rhor_spline[m*7 + 2];
                var z2p = (z2r_spline[m * 7 + 0] * p + z2r_spline[m * 7 + 1]) * p + z2r_spline[m * 7 + 2];
                var z2 = ((z2r_spline[m * 7 + 3] * p + z2r_spline[m * 7 + 4]) * p + z2r_spline[m * 7 + 5]) * p + z2r_spline[m*7+6];

                var recip = 1.0/r;
                var phi = z2 * recip;
                var phip = z2p * recip - phi * recip;
                var psip = FP[ijk].Fp[rbin][i] * rhoip + FP[ijk].Fp[n][j] * rhoip + phip;
                var fpair = -psip * recip;

                const res = del * fpair;
                (fx, fy, fz) = (fx, fy, fz) + res;
                
                fpair *= .5;

                if store {
                  vir.add(rsq * fpair);
                  evdwl.add(.5 * phi);
                }
              }
            }
            a.f = (fx, fy, fz);
          }
        }
      }
    }
    eng_vdwl += 2.0 * evdwl.read();
    virial = vir.read();
  }
}

// Lennard-Jones potential
class ForceLJ : Force {
  proc ForceLJ(cf : real) {
    cutforcesq = cf * cf;
  }

  proc compute(store : bool) : void {
    eng_vdwl = 0;
    virial = 0;
    var fTimer : Timer;

    // wipe old forces of real AND ghost atoms
    fTimer.start();
    coforall ijk in LocaleGridDom {
      on LocaleGrid[ijk] {
        const Me = Grid[ijk];
        forall (b,c) in zip(Me.Bins,Me.Count[Me.Real]) {
          for a in b[1..c] {
            a.f = (0.0,0.0,0.0);
          }
        }
      }
    }

    // track wipetime for testing purposes
    wipetime += fTimer.elapsed();
    fTimer.stop();
    fTimer.clear();

    // for each atom, compute force between itself and its neighbors
    // can make outer loop parallel if we can make force modifications atomic
    fTimer.start();
    var engs, virs : [LocaleGridDom] real;
    coforall ijk in LocaleGridDom {
      on LocaleGrid[ijk] {
        // bring stuff onto this locale
        const Me = Grid[ijk];

        // by using an atomic variable, we can make the outer loop parallel
        var eng, vir : atomic real;

        forall (b,p,c) in zip(Me.Bins, Me.Pos[Me.Real], Me.Count[Me.Real], Me.Real) {
          for (a, x) in zip(b[1..c],p[1..c]) {
            for(n,i) in a.neighs[1..a.ncount] {
              const del = x - Me.Pos[n][i];
              const rsq = dot(del,del);
              
              if rsq < cutforcesq {
                const sr2: real = 1.0 / rsq;
                const sr6 : real = sr2 * sr2 * sr2;
                const force : real = 48.0 * sr6 * (sr6 - .5) * sr2;
                a.f += del * force;

                if store {
                  eng.add(4.0 * sr6 * (sr6 - 1.0));
                  vir.add(.5 * rsq * force);
                }
              }
            }
          }
        }

        // cut down on communication
        engs[ijk] = eng.read();
        virs[ijk] = vir.read();
      }
    }

    // update values thermo depends on
    eng_vdwl = + reduce engs;
    virial = + reduce virs;

    // higher resolution performance timings - look at the actual computation
    maintime += fTimer.elapsed();
  }
} 
