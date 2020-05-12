use MDTypes;
use configs;
use helpers;
use setup;
use force;
use BlockDist;
use VisualDebug;

private use IO;

class InterpolationObject {
  var n : int;
  var x0 : real;
  var dx : real;
  var invDx : real;
  var nSpace : domain(1);
  var values : [nSpace] real;

  proc init(n:int, x0:real, dx:real, values:[] real) {
    this.n = n;
    this.x0 = x0;
    this.dx = dx;
    this.invDx = 1.0/dx;
    this.nSpace = -1..n+1;
    this.complete();

    var nSpaceInner : domain(1) = 0..n-1;

    this.values[nSpaceInner] = values[nSpaceInner];
    this.values(-1) = this.values(0);
    this.values(n) = this.values(n-1);
    this.values(n+1) = this.values(n-1);
  }

  proc interpolate(in r:real, out f:real, out df:real) {
    if ( r < x0 ) then r = x0;

    r = (r - x0) * invDx;
    var ii:int = floor(r) : int;

    if (ii > n-1) {
      ii = n-1;
      r = n / invDx;
    }
    // reset r to fractional distance
    r = r - floor(r);

    var v : 4*real;
    for i in -1..2 do v(i+1) = values(ii+i);

    const g1 : real = v(2) - v(0);
    const g2 : real = v(3) - v(1);

    f  = v(1) + 0.5*r*(g1 + r*(v(2) + v(0) - 2.0*v(1)));
    df = 0.5*(g1 + r*(g2-g1))*invDx;

//    var g1 : real = values(ii+1) - values(ii-1);
//    var g2 : real = values(ii+2) - values(ii);

//    var f  : real = values(ii) + 0.5*r*(g1 + r*(values(ii+1) + values(ii-1) - 2.0*values(ii)));
//    var df : real = 0.5*(g1 + r*(g2-g1))*invDx;
  }

  proc replicate() : unmanaged InterpolationObject {
    return new unmanaged InterpolationObject(this.n, this.x0, this.dx, this.values);
  }
}

type RList = MAXATOMS*real;

class EAMFaceArr {
  var d : domain(3);
  var a : [d] [1..MAXATOMS] real;
}

class EAMDomain {
  var localDom  : domain(3);
  var halo      = localDom.expand(1);
  var dfEmbed   : [halo] RList;
  var rhoBar    : [localDom] [1..MAXATOMS] real;
  var neighDom  : domain(1) = {1..6};
  var neighs    : [neighDom] int3;
  //TODO: This temporary buffer was introduced to eliminate a spike during initial
  // atom redistribute process. Figure out what causes that and eliminate this extra buffer
  // var temps     : [neighDom] EAMFaceArr;
  var srcSlice  : [neighDom] domain(3);
  var destSlice : [neighDom] domain(3);
//  var ticker    = new Ticker("     eamcommPull");   // ticker for halo exchange
}

class EAMPot {
  var eamDom : [locDom] unmanaged EAMDomain;
}

class ForceEAM : Force {

  var eamPot : unmanaged EAMPot?;
  var phiIO, rhoIO, fIO : unmanaged InterpolationObject?;

  proc init() {}

  proc init(potDir:string, potFile:string, potType:string) {
    this.complete();

    this.potName = "EAM";
    var input_file = potDir + "/" + potFile;
    var fchan: file;
    try! open(input_file, iomode.r);

    if (potType == "setfl") then eamReadSetfl(fchan);
    else if (potType == "funcfl") then eamReadFuncfl(fchan);
    else {
      var errMsg : string = "Potential type" + potType + " not supported. Fatal error";
      throwError(errMsg);
    }
    this.eamPot = nil;
  }

  override proc epilogue() : void {
if useChplVis then tagVdebug("setupEAMForce");
    const boxSpace = {1..numBoxes(0), 1..numBoxes(1), 1..numBoxes(2)};
    const distSpace = boxSpace dmapped Block(boundingBox=boxSpace, targetLocales=locGrid);
    var eamDom : [locDom] unmanaged EAMDomain?;
    coforall ijk in locDom {
      on locGrid[ijk] {
        const MyLocDom = distSpace.localSubdomain();
        var MyEAMDom = new unmanaged EAMDomain(localDom = MyLocDom);
        eamDom[ijk] = MyEAMDom;

        const lDh = locDom.high;
        const bSh = boxSpace.high;

local {
        const ref localDom = MyEAMDom.localDom;
        const ref halo = MyEAMDom.halo;
        const ref neighDom = MyEAMDom.neighDom;
        ref srcSlice = MyEAMDom.srcSlice;
        ref destSlice = MyEAMDom.destSlice;
        ref neighs = MyEAMDom.neighs;
        // ref temps = MyEAMDom.temps;

        var xyz : [neighDom] int3;
        xyz[1] = (-1,0,0);
        xyz[2] = ( 1,0,0);
        xyz[3] = (0,-1,0);
        xyz[4] = (0, 1,0);
        xyz[5] = (0,0,-1);
        xyz[6] = (0,0, 1);

        for i in 1..2 do destSlice[i] = localDom.interior(xyz[i]).exterior(xyz[i]);
        for i in 3..4 do destSlice[i] = localDom.interior(xyz[i]).expand(xyz[2]).exterior(xyz[i]);
        for i in 5..6 do destSlice[i] = halo.interior(xyz[i]);

        //for (dest, src, t, neigh, nOff) in zip(destSlice, srcSlice, temps, neighs, xyz) {
        for (dest, src, neigh, nOff) in zip(destSlice, srcSlice, neighs, xyz) {
          src = dest;
          var neighbor = ijk + nOff;
          var srcOff = (0,0,0);
          for i in 0..2 {
            if(neighbor(i) < 0) {
              //neighbor(i) = locDom.high(i);
              //srcOff(i) = boxSpace.high(i);
              neighbor(i) = lDh(i);
              srcOff(i) = bSh(i);
            }
            //else if(neighbor(i) > locDom.high(i)) {
            else if(neighbor(i) > lDh(i)) {
              neighbor(i) = 0;
              //srcOff(i) = -boxSpace.high(i);
              srcOff(i) = -bSh(i);
            }
          }
          src = src.translate(srcOff);
          //t = new EAMFaceArr(d=src);
          neigh = neighbor;
        }
}
      }
    }
    this.eamPot = new unmanaged EAMPot(eamDom!);
if useChplVis then pauseVdebug();
  }

  inline proc throwError(errMsg:string) : void {
    stderr.writeln(errMsg);
    exit(0);
  }

  inline proc eamReadFuncfl(fchan) {
    var r = fchan.reader();

    // line 1
    var name = r.readln(string, string);
    this.name = name(1);

    // line 2
    var mass_amu : real;
    (this.atomicNumber, mass_amu, this.lat, this.latticeType) = r.readln(int, real, real, string);
    this.mass = mass_amu * amuToInternalMass;

    // line 3
    var nRho, nR : int;
    var dRho, dR : real;
    var cutoff : real;
    (nRho, dRho, nR, dR, cutoff) = r.readln(int, real, int, real, real);
    this.cutoff = cutoff;
    this.cutoff2 = cutoff * cutoff;

    var bufSize = max(nRho, nR);
    var values : [0..bufSize-1] real;
    var x0 : real = 0.0;

    // Read embedding energy F(rhobar)
    for ii in 0..nRho-1 do values(ii) = r.read(real);
    this.fIO   = new unmanaged InterpolationObject(nRho, x0, dRho, values);

    // Read Z(r) and convert to phi(r)
    for ii in 0..nR-1 do values(ii) = r.read(real);
    for ii in 1..nR-1 {
      var rx : real = x0 + ii*dR;
      values(ii) *= values(ii)/rx;
      values(ii) *= (hartreeToEv * bohrToAngs);
    }
    values(0) = values(1) + (values(1) - values(2));
    this.phiIO = new unmanaged InterpolationObject(nR, x0, dR, values);

    // Read electron density rho(r)
    for ii in 0..nR-1 do values(ii) = r.read(real);
    this.rhoIO = new unmanaged InterpolationObject(nR, x0, dR, values);
  }

  inline proc eamReadSetfl(fchan) {
    var r = fchan.reader();

    // line 3 -- comments
    r.readln();
    r.readln();
    r.readln();

    // line 4
    var species = r.readln(int, string);
    if(species(0) != 1) then {
      var errMsg : string = "This CoMD version does not support alloys and cannot read setfl files with multiple species. Fatal Error.";
      throwError(errMsg);
    }

    this.name = species(1);

    // line 5
    var nRho, nR : int;
    var dRho, dR : real;

    var cutoff : real;

    (nRho, dRho, nR, dR, cutoff) = r.readln(int, real, int, real, real);

    this.cutoff = cutoff;
    this.cutoff2 = cutoff * cutoff;

    // **** THIS CODE IS RESTRICTED TO ONE ELEMENT
    // Per-atom header

    var mass_amu : real;

    (this.atomicNumber, mass_amu, this.lat, this.latticeType) = r.readln(int, real, real, string);
    this.mass = mass_amu * amuToInternalMass;

    var bufSize = max(nRho, nR);
    var values : [0..bufSize-1] real;
    var x0 : real = 0.0;

    // Read embedding energy F(rhobar)
    for ii in 0..nRho-1 do values(ii) = r.readln(real);
    this.fIO   = new unmanaged InterpolationObject(nRho, x0, dRho, values);

    // Read electron density rho(r)
    for ii in 0..nR-1 do values(ii) = r.readln(real);
    this.rhoIO = new unmanaged InterpolationObject(nR, x0, dR, values);

    // Read phi(r)*r and convert to phi(r)
    for ii in 0..nR-1 do values(ii) = r.readln(real);
    for ii in 1..nR-1 do {
      var rx : real = x0 + ii*dR;
      values(ii) /= rx;
    }
    values(0) = values(1) + (values(1) - values(2));
    this.phiIO = new unmanaged InterpolationObject(nR, x0, dR, values);
  }

  inline proc haloExchange(const ref MyEAMDom : unmanaged EAMDomain, const ref eamDom : [] unmanaged EAMDomain, const in face : int) {
    const ref dest = MyEAMDom.destSlice;
    const ref src = MyEAMDom.srcSlice;
    const nf = MyEAMDom.neighs[face];
    ref nDest = MyEAMDom.dfEmbed[dest[face]];
    on locGrid[nf] {
      const sf = src[face];
      const ref ec = eamDom[nf].dfEmbed[sf];
      nDest = ec;
      //nDest._value.doiBulkTransferStride(ec._value);
      //const e = eamDom[nf];
      //const sf = src[face];
      //ref ec = e.temps[face].a;
      //local ec = e.dfEmbed[sf];
      //nDest._value.doiBulkTransferStride(ec._value);
    }
  }

  // exchange data along fm and fp faces
  inline proc exchangeData(const ref MyEAMDom : unmanaged EAMDomain, const ref eamDom : [] unmanaged EAMDomain, const in i : int) {
    cobegin {
      { haloExchange(MyEAMDom, eamDom, i); }
      { haloExchange(MyEAMDom, eamDom, i+1); }
    }
  }

  proc exchangeData() {
    // halo exchange
    tArray[timerEnum.EAMHALO].start();
    const ref eamDom = this.eamPot!.eamDom;
    for i in 1..6 by 2 {
      coforall ijk in locDom {
        on locGrid[ijk] {
          exchangeData(eamDom[ijk], eamDom, i);
        }
      }
    }
    tArray[timerEnum.EAMHALO].stop();
  }

  inline proc compute1(a:real3, b:real3, inout fij:real3, inout pij:real, inout rij:real) {
    var dr = a - b;
    var r2 = dot(dr, dr);
    if( r2 > cutoff2 || r2 <= 0.0 ) then return;
    var r = sqrt(r2);
    var phiTmp, dPhi:real; phiIO!.interpolate(r, phiTmp, dPhi);
    var rhoTmp, dRho:real; rhoIO!.interpolate(r, rhoTmp, dRho);
    fij += (dPhi/r)*dr;
    pij += phiTmp/2;
    rij += rhoTmp;
  }

  inline proc compute2(a:real3, b:real3, dfEmbed:real, inout fij:real3) {
    var dr = a - b;
    var r2 = dot(dr, dr);
    if( r2 > cutoff2 || r2 <= 0.0 ) then return;
    var r = sqrt(r2);
    var rhoTmp, dRho:real; rhoIO!.interpolate(r, rhoTmp, dRho);
    fij += (dfEmbed*dRho/r)*dr;
  }

  override proc compute() : void {
    tArray[timerEnum.FORCE1].start();
if useChplVis then tagVdebug("computeEAMForce");
    const ref eamDom = this.eamPot!.eamDom;
    coforall ijk in locDom {
      on locGrid[ijk] {
        const MyDom = Grid[ijk]!;
        const MyEAMDom = eamDom[ijk];
        const force = MyDom.force : ForceEAM;
        const neighs = {-1..1, -1..1, -1..1};
        coforall (box, f, pe, dfEmbed, rhoBar, boxIdx) in zip(MyDom.cells[MyDom.localDom], MyDom.f, MyDom.pe, MyEAMDom.dfEmbed[MyDom.localDom], MyEAMDom.rhoBar[MyDom.localDom], MyDom.localDom) {
          for i in 1..box.count {
            f(i)  = (0.0, 0.0, 0.0);
            pe(i) = 0.0;
            rhoBar(i) = 0.0;
          }

          for n in neighs {
            const ref nBox = MyDom.cells[boxIdx + n];
            for i in 1..box.count {
              var fij:real3, pij:real, rij:real;
              for j in 1..nBox.count {
                force.compute1(box.atoms(i).r, nBox.atoms(j).r, fij, pij, rij);
              }
              f(i) -= fij;
              pe(i) += pij;
              rhoBar(i) += rij;
            }
          }

          for i in 1..box.count {
            var fEmbedTmp, dfEmbedTmp:real; force.fIO!.interpolate(rhoBar(i), fEmbedTmp, dfEmbedTmp);
            dfEmbed(i) = dfEmbedTmp;
            pe(i) += fEmbedTmp;
          }
        }
      }
    }
    tArray[timerEnum.FORCE1].stop();
if useChplVis then pauseVdebug();
if useChplVis then tagVdebug("exchangeEAMHalo");
    exchangeData();
if useChplVis then pauseVdebug();
    tArray[timerEnum.FORCE2].start();
if useChplVis then tagVdebug("computeEAMForce");
    coforall ijk in locDom {
      on locGrid[ijk] {
        const MyDom = Grid[ijk]!;
        const MyEAMDom = eamDom[ijk];
        const force = MyDom.force : ForceEAM;
        const neighs = {-1..1, -1..1, -1..1};
        coforall (box, f, dfEmbed, boxIdx) in zip(MyDom.cells[MyDom.localDom], MyDom.f, MyEAMDom.dfEmbed[MyDom.localDom], MyDom.localDom) {
          for n in neighs {
            const ref nBox = MyDom.cells[boxIdx + n];
            const ref nDfEmbed = MyEAMDom.dfEmbed[boxIdx + n];
            for i in 1..box.count {
              var fij:real3;
              for j in 1..nBox.count {
                force.compute2(box.atoms(i).r, nBox.atoms(j).r, dfEmbed(i)+nDfEmbed(j), fij);
              }
              f(i) -= fij;
            }
          }
        }
      }
    }
    tArray[timerEnum.FORCE2].stop();
if useChplVis then pauseVdebug();
  }

  override proc computeLocal() : void {
    tArray[timerEnum.FORCE1].start();
if useChplVis then tagVdebug("computeEAMForce");
    const ref eamDom = this.eamPot!.eamDom;
    coforall ijk in locDom {
      on locGrid[ijk] {
        const MyDom = Grid[ijk]!;
        const MyEAMDom = eamDom[ijk];
        const force = MyDom.force : ForceEAM;
local {
        const neighs = {-1..1, -1..1, -1..1};
        coforall (box, f, pe, dfEmbed, rhoBar, boxIdx) in zip(MyDom.cells[MyDom.localDom], MyDom.f, MyDom.pe, MyEAMDom.dfEmbed[MyDom.localDom], MyEAMDom.rhoBar[MyDom.localDom], MyDom.localDom) {
          for i in 1..box.count {
            f(i)  = (0.0, 0.0, 0.0);
            pe(i) = 0.0;
            rhoBar(i) = 0.0;
          }

          for n in neighs {
            const ref nBox = MyDom.cells[boxIdx + n];
            for i in 1..box.count {
              var fij:real3, pij:real, rij:real;
              for j in 1..nBox.count {
                force.compute1(box.atoms(i).r, nBox.atoms(j).r, fij, pij, rij);
              }
              f(i) -= fij;
              pe(i) += pij;
              rhoBar(i) += rij;
            }
          }

          for i in 1..box.count {
            var fEmbedTmp, dfEmbedTmp:real; force.fIO!.interpolate(rhoBar(i), fEmbedTmp, dfEmbedTmp);
            dfEmbed(i) = dfEmbedTmp;
            pe(i) += fEmbedTmp;
          }
        }
      }
}
    }
    tArray[timerEnum.FORCE1].stop();
if useChplVis then pauseVdebug();
if useChplVis then tagVdebug("exchangeEAMHalo");
    exchangeData();
if useChplVis then pauseVdebug();
    tArray[timerEnum.FORCE2].start();
if useChplVis then tagVdebug("computeEAMForce");
    coforall ijk in locDom {
      on locGrid[ijk] {
        const MyDom = Grid[ijk]!;
        const MyEAMDom = eamDom[ijk];
        const force = MyDom.force : ForceEAM;
local {
        const neighs = {-1..1, -1..1, -1..1};
        coforall (box, f, dfEmbed, boxIdx) in zip(MyDom.cells[MyDom.localDom], MyDom.f, MyEAMDom.dfEmbed[MyDom.localDom], MyDom.localDom) {
          for n in neighs {
            const ref nBox = MyDom.cells[boxIdx + n];
            const ref nDfEmbed = MyEAMDom.dfEmbed[boxIdx + n];
            for i in 1..box.count {
              var fij:real3;
              for j in 1..nBox.count {
                force.compute2(box.atoms(i).r, nBox.atoms(j).r, dfEmbed(i)+nDfEmbed(j), fij);
              }
              f(i) -= fij;
            }
          }
        }
}
      }
    }
    tArray[timerEnum.FORCE2].stop();
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
  }

  override proc replicate() : unmanaged ForceEAM? {
    var temp = new unmanaged ForceEAM();
    temp.cutoff = this.cutoff;
    temp.mass = this.mass;
    temp.lat = this.lat;
    temp.latticeType = this.latticeType;
    temp.name = this.name;
    temp.atomicNumber = this.atomicNumber;
    temp.potName = this.potName;
    temp.cutoff2 = this.cutoff2;
    temp.eamPot = nil;
    temp.phiIO = this.phiIO!.replicate();
    temp.rhoIO = this.rhoIO!.replicate();
    temp.fIO   = this.fIO!.replicate();
    return temp;
  }

  proc deinit() {
    delete phiIO;
    delete rhoIO;
    delete fIO;
  }
}
